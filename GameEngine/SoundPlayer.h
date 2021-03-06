/*      
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  File:       SoundPlayer.h
 *  Author:     Richard Zou
 *  Created on: 2015-03-01
 */

#ifndef SOUND_PLAYER_H
#define SOUND_PLAYER_H

#include "Common.h"

#include <dsound.h>
#include <deque>
#include <MMSystem.h>

using namespace std;

struct AudioInfo
{
    char m_szFileName[MAX_FILE_PATH_LEN];
    LPDIRECTSOUNDBUFFER m_lpPrimaryBuffer;  //主缓冲接口
    LPDIRECTSOUNDBUFFER m_lpSecondaryBuffer;  //辅助缓冲接口

    AudioInfo()
    {
        memset(m_szFileName, 0, MAX_FILE_PATH_LEN);
        m_lpPrimaryBuffer = NULL;
        m_lpSecondaryBuffer = NULL;
    }
};

class CSoundPlayer
{
public:
    CSoundPlayer(void);
    ~CSoundPlayer(void);
    bool Init(HWND hWnd);
    void Shutdown();
    void Play(const char *pFileName, bool bLoop = false);
    void Stop(const char *pFileName);
    void AddAudioFile(const char *pFileName);
    bool ParseAudioFile(FILE *pFile, WAVEFORMATEX &stWaveFmt);
    void RemoveAll();

private:
    LPDIRECTSOUND       m_lpSound;
    deque<AudioInfo *>  m_deqAudio;
};

#endif
