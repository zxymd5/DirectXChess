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
 *  File:		GameHandle.h
 *  Author:		Richard Zou
 *	Created on:	2015-03-01
 */

#ifndef GAME_HANDLE_H
#define GAME_HANDLE_H

#include "Subject.h"
#include "MoveRouteGenerator.h"

#include <list>
using namespace std;

class CGameHandle : public CSubject
{
public:
    CGameHandle(void);
    virtual ~CGameHandle(void);
    void Init();
    void ResetChessManLayout();
    void GetChessMan(int szChessMan[][s_nChessBoardColumn]);
    void SetCurrentMoveRoute(int nRow, int nColumn);
    const MoveRoute &GetCurrentMoveRoute();
    void ResetMoveRoute(MoveRoute &stRoute);
    void SetGameResult(int nGameResult);
    int GetGameResult() const;
    int GetCurrentTurn() const;
    void SetDeadOne(int nWhoIsDead);
    int GetDeadOne() const;
    list<MoveRoute> &GetLstMoveRoute();
    int GetGeneralPosition(int nChessMan, int &nRow, int &nColumn);
    void FallBack();
    void FallBackOneStep();
    void SaveToFile(const char *pFileName, int nFileType);
    void LoadFromFile(const char *pFileName, int nFileType);

private:
    int m_nCurrentTurn;
    int m_nGameResult;
    int m_nWhoIsDead;
    int m_szChessMan[s_nChessBoardRow][s_nChessBoardColumn];
    list<MoveRoute> m_lstMoveRoute;
    MoveRoute m_stCurrentMoveRoute;
    CMoveRouteGenerator m_clGenerator;
};

extern CGameHandle g_GameHandle;

#endif
