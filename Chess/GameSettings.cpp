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
 *  File:		GameSettings.cpp
 *  Author:		Richard Zou
 *	Created on:	2015-03-01
 */

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

    m_nGameType = clIni.GetLongValue("Game", "GameType", 1);
    m_nCompetitorSide = clIni.GetLongValue("Game", "CompetitorSide", s_nBlackSide);
    m_nAhead = clIni.GetLongValue("Game", "Ahead", BLACK);
    m_nStepTime = clIni.GetLongValue("Game", "StepTime", 0);
}

void CGameSettings::SaveSettings( const char *clFileName )
{
    CSimpleIni clIni;
    clIni.LoadFile(clFileName);
    clIni.SetLongValue("Game", "GameType", m_nGameType);
    clIni.SetLongValue("Game", "CompetitorSide", m_nCompetitorSide);
    clIni.SetLongValue("Game", "Ahead", m_nAhead);
    clIni.SetLongValue("Game", "StepTime", m_nStepTime);
    clIni.SaveFile(clFileName);
}
