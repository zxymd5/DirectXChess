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
    void FallBack(int nWhichSide);

    void FallBackOneStep();

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
