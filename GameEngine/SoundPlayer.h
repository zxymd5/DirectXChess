#ifndef SOUND_PLAYER_H
#define SOUND_PLAYER_H

#include "Common.h"

#include <dsound.h>
#include <deque>
#include <MMSystem.h>

using namespace std;

struct AudioInfo
{
    char m_strFileName[nMaxFilePathLen];
    LPDIRECTSOUNDBUFFER m_pPrimaryBuffer;  //主缓冲接口
    LPDIRECTSOUNDBUFFER m_pSecondaryBuffer;  //辅助缓冲接口

    AudioInfo()
    {
        memset(m_strFileName, 0, nMaxFilePathLen);
        m_pPrimaryBuffer = NULL;
        m_pSecondaryBuffer = NULL;
    }
};

class CSoundPlayer
{
public:
    CSoundPlayer(void);
    ~CSoundPlayer(void);
    bool Init(HWND hWnd);
    void Shutdown();
    void Play(const char *strFileName, bool bLoop = false);
    void Stop(const char *strFileName);
    void AddAudioFile(const char *strFileName);
    bool ParseAudioFile(FILE *pFile, WAVEFORMATEX &stWaveFmt);
    void RemoveAll();

private:
    LPDIRECTSOUND       m_pSound;
    deque<AudioInfo *>  m_deqAudio;
};

#endif
