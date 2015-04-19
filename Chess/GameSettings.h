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
 *  File:		GameSettings.h
 *  Author:		Richard Zou
 *	Created on:	2015-03-01
 */

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