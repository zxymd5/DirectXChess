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
#include <process.h>

#include <MMSystem.h>
using namespace std;

class CGameHandle : public CSubject
{
public:
    CGameHandle(void);
    virtual ~CGameHandle(void);
    void Init();
    void NewGame();
    void ResetChessManLayout();
    void GetChessMan(int szChessMan[][CHESSBOARD_COLUMN]);
    void DoMove(int nRow, int nColumn);

    void ApplyCompleteMove();

    bool BlackDoMove(int nRow, int nColumn);
    bool RedDoMove(int nRow, int nColumn);
    void ComputerMove();
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
    __int64 GetCurrentStepStartTime();
    void StepTimeOver();
    void OnTie();
    void OnLose();

    static unsigned int __stdcall SaveGameFunc(void *pParam);

private:
    int m_nCurrentTurn;
    int m_nGameResult;
    int m_nWhoIsDead;
    int m_szChessMan[CHESSBOARD_ROW][CHESSBOARD_COLUMN];
    list<MoveRoute> m_lstMoveRoute;
    MoveRoute m_stCurrentMoveRoute;
    CMoveRouteGenerator m_clGenerator;
    char m_szGameInfoSaveFile[MAX_PATH];
    HANDLE m_hEventSaveGame;    //写数据库的Event
    HANDLE m_hEventGameSaved;
    HANDLE m_hThreadSaveGame;   //写数据库的线程
    __int64 m_llCurrentStepStartTime;
};

extern CGameHandle g_GameHandle;

#endif
