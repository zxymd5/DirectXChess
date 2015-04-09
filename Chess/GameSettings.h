#ifndef GAME_SETTINGS
#define GAME_SETTINGS

class CGameSettings
{
public:
    CGameSettings(void);
    ~CGameSettings(void);
    void LoadSettings(const char *strFileName);
    void SaveSettings(const char *strFileName);

    int m_nCompetitor;
    int m_nCompetitorSide;
    int m_nAhead;

};

extern CGameSettings g_GameSettings;

#endif