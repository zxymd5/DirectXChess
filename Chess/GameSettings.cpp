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
 *  File:       GameSettings.cpp
 *  Author:     Richard Zou
 *  Created on: 2015-03-01
 */

#include "GameSettings.h"
#include "CommDef.h"
#include "../Include/SimpleIni.h"

CGameSettings g_GameSettings;

CGameSettings::CGameSettings(void)
{
}

CGameSettings::~CGameSettings(void)
{
}

void CGameSettings::LoadSettings(const char *pFileName)
{
    CSimpleIni clIni;
    clIni.LoadFile(pFileName);
    m_nGameType = clIni.GetLongValue("Game", "GameType", COMPITITOR_HUMAN);
    m_nCompetitorSide = clIni.GetLongValue("Game", "CompetitorSide", BLACK);
    m_nAhead = clIni.GetLongValue("Game", "Ahead", BLACK);
    m_nStepTime = clIni.GetLongValue("Game", "StepTime", 0);
    m_nServerOrClient = clIni.GetLongValue("Game", "ServerOrClient", CLIENT_SIDE);
    const char *pIpAddr = clIni.GetValue("Game", "IpAddr", "127.0.0.1");
    strcpy(m_szIpAddr, pIpAddr);
    m_nPort = clIni.GetLongValue("Game", "Port", 8000);
}

void CGameSettings::SaveSettings( const char *pFileName )
{
    CSimpleIni clIni;
    clIni.LoadFile(pFileName);
    clIni.SetLongValue("Game", "GameType", m_nGameType);
    clIni.SetLongValue("Game", "CompetitorSide", m_nCompetitorSide);
    clIni.SetLongValue("Game", "Ahead", m_nAhead);
    clIni.SetLongValue("Game", "StepTime", m_nStepTime);
    clIni.SetLongValue("Game", "ServerOrClient", m_nServerOrClient);
    clIni.SetValue("Game", "IpAddr", m_szIpAddr);
    clIni.SetLongValue("Game", "Port", m_nPort);

    clIni.SaveFile(pFileName);
}
