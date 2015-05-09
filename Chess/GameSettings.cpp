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

    m_nCompetitor = clIni.GetLongValue("ManMachineGame", "Competitor", 1);
    m_nCompetitorSide = clIni.GetLongValue("ManMachineGame", "CompetitorSide", s_nBlackSide);
    m_nAhead = clIni.GetLongValue("ManMachineGame", "Ahead", BLACK);
    m_nStepTime = clIni.GetLongValue("ManMachineGame", "StepTime", 0);
}

void CGameSettings::SaveSettings( const char *clFileName )
{
    CSimpleIni clIni;
    clIni.LoadFile(clFileName);
    clIni.SetLongValue("ManMachineGame", "Competitor", m_nCompetitor);
    clIni.SetLongValue("ManMachineGame", "CompetitorSide", m_nCompetitorSide);
    clIni.SetLongValue("ManMachineGame", "Ahead", m_nAhead);
    clIni.SetLongValue("ManMachineGame", "StepTime", m_nStepTime);
    clIni.SaveFile(clFileName);
}
