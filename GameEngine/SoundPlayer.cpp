/* 		
 * 	This program is free software: you can redistribute it and/or modify
 * 	it under the terms of the GNU General Public License as published by
 * 	the Free Software Foundation, either version 3 of the License, or
 * 	(at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 * 	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  File:		SounderPlayer.cpp
 *  Author:		Richard Zou
 *	Created on:	2015-03-01
 */

#include "SoundPlayer.h"

#include <MMSystem.h>

CSoundPlayer::CSoundPlayer(void)
{
    m_pSound = NULL;
}

CSoundPlayer::~CSoundPlayer(void)
{

}

bool CSoundPlayer::Init( HWND hWnd )
{
    HRESULT Ret = DirectSoundCreate(NULL, &m_pSound, NULL);
    if (Ret != DS_OK)
    {
        return false;
    }

    Ret = m_pSound->SetCooperativeLevel(hWnd, DSSCL_NORMAL);
    if (Ret != DS_OK)
    {
        return false;
    }

    return true;
}

void CSoundPlayer::AddAudioFile( const char *strFileName )
{
    FILE *pFile = fopen(strFileName, "rb");
    if (pFile == NULL)
    {
        return;
    }

    WAVEFORMATEX stWaveFmt;
    ZeroMemory(&stWaveFmt, sizeof(WAVEFORMATEX));
    stWaveFmt.wFormatTag = WAVE_FORMAT_PCM;

    if (ParseAudioFile(pFile, stWaveFmt) == false)
    {
        fclose(pFile);
        return;
    }

    DSBUFFERDESC BufferDesc;
    ZeroMemory(&BufferDesc, sizeof(DSBUFFERDESC));
    BufferDesc.dwSize = sizeof(DSBUFFERDESC);
    BufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
    BufferDesc.dwBufferBytes = stWaveFmt.cbSize;
    BufferDesc.lpwfxFormat = &stWaveFmt;


    AudioInfo *pInfo = new AudioInfo;
    HRESULT hRet = m_pSound->CreateSoundBuffer(&BufferDesc, &pInfo->m_pPrimaryBuffer, NULL);
    pInfo->m_pPrimaryBuffer->QueryInterface(IID_IDirectSoundBuffer, (void**)&pInfo->m_pSecondaryBuffer);

    char *pAudioData;
    DWORD dwAudioDataSize;

    pInfo->m_pSecondaryBuffer->Lock(0, stWaveFmt.cbSize, 
        (void **)&pAudioData, &dwAudioDataSize, NULL, NULL, 0);

    if (pAudioData)
    {
        fread(pAudioData, 1, dwAudioDataSize, pFile);
    }

    pInfo->m_pSecondaryBuffer->Unlock(pAudioData, dwAudioDataSize, NULL, 0);

    fclose(pFile);
    strcpy(pInfo->m_strFileName, strFileName);

    m_deqAudio.push_back(pInfo);
}

void CSoundPlayer::Play( const char *strFileName, bool bLoop /*= false*/ )
{
    deque<AudioInfo *>::iterator it = m_deqAudio.begin();
    bool bFind = false;
    bool bPlaying = false;

    for (; it != m_deqAudio.end(); ++it)
    {
        if (strcmp((*it)->m_strFileName, strFileName) == 0)
        {
            bFind = true;
            if ((*it)->m_pSecondaryBuffer)
            {
                DWORD Status;
                (*it)->m_pSecondaryBuffer->GetStatus(&Status);
                bPlaying |= ((Status & DSBSTATUS_PLAYING) != 0);
            }

            break;
        }
    }

    if (bFind && !bPlaying)
    {
        (*it)->m_pSecondaryBuffer->Play(0, 0, bLoop ? DSBPLAY_LOOPING : 0);
    }
}

void CSoundPlayer::RemoveAll()
{
    deque<AudioInfo *>::iterator it = m_deqAudio.begin();

    for (; it != m_deqAudio.end(); ++it)
    {
        (*it)->m_pSecondaryBuffer->Stop();
        SAFE_RELEASE((*it)->m_pPrimaryBuffer);
        SAFE_RELEASE((*it)->m_pSecondaryBuffer);
        delete (*it);
        (*it) = NULL;
    }
}

void CSoundPlayer::Stop( const char *strFileName )
{
    deque<AudioInfo *>::iterator it = m_deqAudio.begin();
    bool bFind = false;

    for (; it != m_deqAudio.end(); ++it)
    {
        if (strcmp((*it)->m_strFileName, strFileName) == 0)
        {
            bFind = true;
            break;
        }
    }

    if (bFind)
    {
        (*it)->m_pSecondaryBuffer->Stop();
    }
}

//     WAVE文件格式
//     RIFF WAVE Chunk 12 Bytes
//     ==================================
//     |      |所占字节数| 具体内容 |
//     ==================================
//     |  ID  |  4 Bytes | 'RIFF'   |
//     ----------------------------------
//     | Size |  4 Bytes |          |
//     ----------------------------------
//     | Type |  4 Bytes | 'WAVE'   |
//     ----------------------------------
//
//     Format Chunk  24/26 Bytes
//     ====================================================================
//     |               | 字节数  | 具体内容 |
//     ====================================================================
//     |       ID      | 4 Bytes | 'fmt '   |
//     --------------------------------------------------------------------
//     |       Size    | 4 Bytes | 数值为16或18，18则最后又附加信息 |
//     -------------------------------------------------------------------- 
//     |    FormatTag  | 2 Bytes | 编码方式，一般为0x0001 | 
//     -------------------------------------------------------------------- 
//     |    Channels   | 2 Bytes | 声道数目，1--单声道；2--双声道 | 
//     -------------------------------------------------------------------- 
//     | SamplesPerSec | 4 Bytes | 采样频率 | 
//     -------------------------------------------------------------------- 
//     | AvgBytesPerSec| 4 Bytes | 每秒所需字节数 |===> WAVE_FORMAT
//     -------------------------------------------------------------------- 
//     |   BlockAlign  | 2 Bytes | 数据块对齐单位(每个采样需要的字节数) | 
//     -------------------------------------------------------------------- 
//     | BitsPerSample | 2 Bytes | 每个采样需要的bit数 | 
//     -------------------------------------------------------------------- 
//     |               | 2 Bytes | 附加信息（可选，通过Size来判断有无） | 
//     -------------------------------------------------------------------- 
//
//     Fact Chunk 12 Bytes（可选）
//     ==================================
//     |      |所占字节数| 具体内容 |
//     ==================================
//     |  ID  |  4 Bytes |  'fact'  |
//     ----------------------------------
//     | Size |  4 Bytes | 数值为4  |
//     ----------------------------------
//     | data |  4 Bytes |          |
//     ----------------------------------
//
//     Data Chunk 8 Bytes
//     ==================================
//     |      |所占字节数| 具体内容 |
//     ==================================
//     |   ID |  4 Bytes |  'data'  |
//     ----------------------------------
//     | Size |  4 Bytes |          |
//     ----------------------------------
//     | data |          |          |
//     ----------------------------------

bool CSoundPlayer::ParseAudioFile( FILE *pFile, WAVEFORMATEX &stWaveFmt )
{
    char strBuff[5];
    memset(strBuff, 0, 5);

    fread(strBuff, 4, 1, pFile);
    if (strcmp(strBuff, "RIFF") != 0)
    {
        return false;
    }
    fseek(pFile, 4, SEEK_CUR); 
    fread(strBuff, 4, 1, pFile);
    if (strcmp(strBuff, "WAVE") != 0)
    {
        return false;
    }
    
    fread(strBuff, 4, 1, pFile);
    if (strcmp(strBuff, "fmt ") != 0) 
    {
        return false;
    }
    int nSize = 0;
    fread(&nSize, 4, 1, pFile);
    fread(&stWaveFmt.wFormatTag, 2, 1, pFile);       //格式类型
    fread(&stWaveFmt.nChannels, 2, 1, pFile);        //通道数
    fread(&stWaveFmt.nSamplesPerSec, 4, 1, pFile);   //样本频率
    fread(&stWaveFmt.nAvgBytesPerSec, 4, 1, pFile);  //音频传输速率
    fread(&stWaveFmt.nBlockAlign, 2, 1, pFile);      //块对齐
    fread(&stWaveFmt.wBitsPerSample, 2, 1, pFile);   //数据位深
    if (nSize == 18)
    {
        fseek(pFile, 2, SEEK_CUR);
    }

    fread(strBuff, 4, 1, pFile);
    if (strcmp(strBuff, "fact") == 0)
    {
        fseek(pFile, 8, SEEK_CUR);
        fread(strBuff, 4, 1, pFile);
    }

    if (strcmp(strBuff, "data") != 0)
    {
        return false;
    }

    fread(&stWaveFmt.cbSize, 2, 1, pFile);
    fseek(pFile, 2, SEEK_CUR);

    return true;
}

void CSoundPlayer::Shutdown()
{
    RemoveAll();
    SAFE_RELEASE(m_pSound);
}
