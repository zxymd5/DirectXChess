#include "GameSettings.h"
#include "../Include/SimpleIni.h"
#include "CommDef.h"

CGameSettings g_GameSettings;

CGameSettings::CGameSettings(void)
{
}

CGameSettings::~CGameSettings(void)
{
}

void CGameSettings::LoadSettings(const char *strFileName)
{
    CSimpleIni clIni;
    clIni.LoadFile(strFileName);

    m_nCompetitor = clIni.GetLongValue("ManMachineGame", "Competitor", 1);
    m_nCompetitorSide = clIni.GetLongValue("ManMachineGame", "CompetitorSide", s_nBlackSide);
    m_nAhead = clIni.GetLongValue("ManMachineGame", "Ahead", s_nTurnBlack);
}

void CGameSettings::SaveSettings( const char *clFileName )
{
    CSimpleIni clIni;
    clIni.LoadFile(clFileName);
    clIni.SetLongValue("ManMachineGame", "Competitor", m_nCompetitor);
    clIni.SetLongValue("ManMachineGame", "CompetitorSide", m_nCompetitorSide);
    clIni.SetLongValue("ManMachineGame", "Ahead", m_nAhead);
    clIni.SaveFile(clFileName);
}
