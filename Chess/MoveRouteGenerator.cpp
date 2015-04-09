#include "MoveRouteGenerator.h"

CMoveRouteGenerator::CMoveRouteGenerator(void)
{
}

CMoveRouteGenerator::~CMoveRouteGenerator(void)
{
}

bool CMoveRouteGenerator::ValidateMoveRoute(int szChessMan[][s_nChessBoardColumn], int nFromRow, 
                                           int nFromColumn, int nToRow, int nToColumn)
{
    int nSrcChessMan = szChessMan[nFromRow][nFromColumn];
    int nDstChessMan = szChessMan[nToRow][nToColumn];
    
    if (nFromRow == nToRow && nFromColumn == nToColumn)
    {
        return false;
    }

    if (IsSameSide(nSrcChessMan, nDstChessMan))
    {
        return false;
    }
    
    bool bRet = false;

    switch(nSrcChessMan)
    {
    case s_nBlackGeneral:
        {
            bRet = ValidateBlackGeneralMoveRoute(szChessMan, nFromRow, nFromColumn, nToRow, nToColumn);
        }
        break;

    case s_nRedGeneral:
        {
            bRet = ValidateRedGeneralMoveRoute(szChessMan, nFromRow, nFromColumn, nToRow, nToColumn);
        }
        break;

    case s_nBlackAdvisor:
        {
           bRet = ValidateBlackAdvisorMoveRoute(szChessMan, nFromRow, nFromColumn, nToRow, nToColumn);
        }
        break;

    case s_nRedAdvisor:
        {
            bRet = ValidateRedAdvisorMoveRoute(szChessMan, nFromRow, nFromColumn, nToRow, nToColumn);
        }
        break;

    case s_nBlackMinister:
        {
            bRet = ValidateBlackMinisterMoveRoute(szChessMan, nFromRow, nFromColumn, nToRow, nToColumn);
        }
        break;

    case s_nRedMinister:
        {
            bRet = ValidateRedMinisterMoveRoute(szChessMan, nFromRow, nFromColumn, nToRow, nToColumn);
        }
        break;
    
    case s_nBlackSoldier:
        {
            bRet = ValidateBlackSoldierMoveRoute(szChessMan, nFromRow, nFromColumn, nToRow, nToColumn);
        }
        break;

    case s_nRedSoldier:
        {
            bRet = ValidateRedSoldierMoveRoute(szChessMan, nFromRow, nFromColumn, nToRow, nToColumn);
        }
        break;

    case s_nBlackChariot:
    case s_nRedChariot:
        {
            bRet = ValidateChariotMoveRoute(szChessMan, nFromRow, nFromColumn, nToRow, nToColumn);
        }
        break;

    case s_nBlackHorse:
    case s_nRedHorse:
        {
            bRet = ValidateHorseMoveRoute(szChessMan, nFromRow, nFromColumn, nToRow, nToColumn);
        }
        break;

    case s_nBlackCannon:
    case s_nRedCannon:
        {
            bRet = ValidateCannonMoveRoute(szChessMan, nFromRow, nFromColumn, nToRow, nToColumn);
        }
        break;

    default:
        break;
    }

    return bRet;
}

bool CMoveRouteGenerator::ValidateBlackGeneralMoveRoute( int szChessMan[][s_nChessBoardColumn], int nFromRow, int nFromColumn, int nToRow, int nToColumn )
{
    int nDstChessMan = szChessMan[nToRow][nToColumn];
    if (nDstChessMan == s_nRedGeneral)  //将帅在同一条直线上，中间不能隔棋子
    {
        if (nFromColumn != nToColumn)
        {
            return false;
        }

        for (int i = nFromRow + 1; i < nToRow; i++)
        {
            if (szChessMan[i][nFromColumn] != 0)
            {
                return false;
            }
        }
    }
    else
    {
        if (nToRow > 2 || nToColumn > 5 || nToColumn < 3)
        {
            return false;           //目标不能在九宫之外
        }

        if (abs(nFromRow - nToRow) + abs(nToColumn - nFromColumn) > 1)
        {
            return false;           //将帅只走一条直线
        }
    }

    return true;
}

bool CMoveRouteGenerator::ValidateRedGeneralMoveRoute( int szChessMan[][s_nChessBoardColumn], int nFromRow, int nFromColumn, int nToRow, int nToColumn )
{
    int nDstChessMan = szChessMan[nToRow][nToColumn];
    if (nDstChessMan == s_nBlackGeneral)  //将帅在同一条直线上，中间不能隔棋子
    {
        if (nFromColumn != nToColumn)
        {
            return false;
        }

        for (int i = nFromRow - 1; i > nToRow; i--)
        {
            if (szChessMan[i][nFromColumn] != 0)
            {
                return false;
            }
        }
    }
    else
    {
        if (nToRow < 7 || nToColumn > 5 || nToColumn < 3)
        {
            return false;           //目标不能在九宫之外
        }

        if (abs(nFromRow - nToRow) + abs(nToColumn - nFromColumn) > 1)
        {
            return false;           //将帅只走一条直线
        }
    }

    return true;
}

bool CMoveRouteGenerator::ValidateBlackAdvisorMoveRoute( int szChessMan[][s_nChessBoardColumn], int nFromRow, int nFromColumn, int nToRow, int nToColumn )
{
    if (nToRow > 2 || nToColumn > 5 || nToColumn < 3)  //士出九宫
    {
        return false;
    }

    if (abs(nFromRow - nToRow) != 1 || abs(nFromColumn - nToColumn) != 1) //士走斜线
    {
        return false;
    }

    return true;
}

bool CMoveRouteGenerator::ValidateRedAdvisorMoveRoute( int szChessMan[][s_nChessBoardColumn], int nFromRow, int nFromColumn, int nToRow, int nToColumn )
{
    if (nToRow < 7 || nToColumn > 5 || nToColumn < 3)  //士出九宫
    {
        return false;
    }

    if (abs(nFromRow - nToRow) != 1 || abs(nFromColumn - nToColumn) != 1) //士走斜线
    {
        return false;
    }

    return true;
}

bool CMoveRouteGenerator::ValidateBlackMinisterMoveRoute( int szChessMan[][s_nChessBoardColumn], int nFromRow, int nFromColumn, int nToRow, int nToColumn )
{
    if (nToRow > 4)
    {
        return false;       //象不能过河
    }

    if (abs(nFromRow - nToRow) != 2 || abs(nFromColumn - nToColumn) != 2)
    {
        return false;       //象走田
    }

    if (szChessMan[(nFromRow + nToRow) / 2][(nFromColumn + nToColumn) / 2] != 0)
    {
        return false;       //象眼被塞
    }

    return true;
}

bool CMoveRouteGenerator::ValidateRedMinisterMoveRoute( int szChessMan[][s_nChessBoardColumn], int nFromRow, int nFromColumn, int nToRow, int nToColumn )
{
    if (nToRow < 5)
    {
        return false;       //象不能过河
    }

    if (abs(nFromRow - nToRow) != 2 || abs(nFromColumn - nToColumn) != 2)
    {
        return false;       //象走田
    }

    if (szChessMan[(nFromRow + nToRow) / 2][(nFromColumn + nToColumn) / 2] != 0)
    {
        return false;       //象眼被塞
    }

    return true;
}

bool CMoveRouteGenerator::ValidateBlackSoldierMoveRoute( int szChessMan[][s_nChessBoardColumn], int nFromRow, int nFromColumn, int nToRow, int nToColumn )
{
    if (nToRow < nFromRow)
    {
        return false;   //兵不回头
    }
    
    if (nFromRow < 5 && nFromRow == nToRow)
    {
        return false;   //兵过河前只能直走
    }

    if (nToRow - nFromRow + abs(nToColumn - nFromColumn) > 1)
    {
        return false;   //兵每次只走一步
    }

    return true;
}

bool CMoveRouteGenerator::ValidateRedSoldierMoveRoute( int szChessMan[][s_nChessBoardColumn], int nFromRow, int nFromColumn, int nToRow, int nToColumn )
{
    if (nToRow > nFromRow)
    {
        return false;
    }

    if (nFromRow > 4 && nFromRow == nToRow)
    {
        return false;    //兵过河前只能直走
    }

    if (nFromRow - nToRow + abs(nToColumn - nFromColumn) > 1)
    {
        return false;   //兵每次只走一步
    }

    return true;
}

bool CMoveRouteGenerator::ValidateChariotMoveRoute( int szChessMan[][s_nChessBoardColumn], int nFromRow, int nFromColumn, int nToRow, int nToColumn )
{
    if (nFromRow != nToRow && nFromColumn != nToColumn)
    {
        return false;
    }

    if (nFromRow == nToRow)   //水平方向
    {
        if (nFromColumn < nToColumn)
        {
            for (int i = nFromColumn + 1; i < nToColumn; i++)
            {
                if (szChessMan[nFromRow][i] != 0)
                {
                    return false;
                }
            }
        }
        else
        {
            for (int i = nToColumn + 1; i < nFromColumn; i++)
            {
                if (szChessMan[nFromRow][i] != 0)
                {
                    return false;
                }
            }
        }
    }
    else                    //竖直方向
    {
        if (nFromRow < nToRow)
        {
            for (int i = nFromRow + 1; i < nToRow; i++)
            {
                if (szChessMan[i][nFromColumn] != false)
                {
                    return false;
                }
            }
        }
        else
        {
            for (int i = nToRow + 1; i < nFromRow; i++)
            {
                if (szChessMan[i][nFromColumn] != false)
                {
                    return false;
                }
            }
        }
    }

    return true;
}

bool CMoveRouteGenerator::ValidateHorseMoveRoute( int szChessMan[][s_nChessBoardColumn], int nFromRow, int nFromColumn, int nToRow, int nToColumn )
{
    if (!((abs(nFromRow - nToRow) == 1 && abs(nFromColumn - nToColumn) == 2) ||
        (abs(nFromRow - nToRow) == 2 && abs(nFromColumn - nToColumn) == 1)))
    {
        return false;       //马走日
    }

    int i = 0;
    int j = 0;

    if (nToColumn - nFromColumn == 2)
    {
        i = nFromColumn + 1;
        j = nFromRow;
    }
    else if (nFromColumn - nToColumn == 2)
    {
        i = nFromColumn - 1;
        j = nFromRow;
    }
    else if (nToRow - nFromRow == 2)
    {
        i = nFromColumn;
        j = nFromRow + 1;
    }
    else if (nFromRow - nToRow == 2)
    {
        i = nFromColumn;
        j = nFromRow - 1;
    }

    if (szChessMan[j][i] != 0)
    {
        return false;   //蹩脚马
    }

    return true;
}

bool CMoveRouteGenerator::ValidateCannonMoveRoute( int szChessMan[][s_nChessBoardColumn], int nFromRow, int nFromColumn, int nToRow, int nToColumn )
{
    if (nFromRow != nToRow && nFromColumn != nToColumn)
    {
        return false;       //炮走直线
    }

    //炮不吃子时经过的路线中不能有棋子
    if (szChessMan[nToRow][nToColumn] == 0)
    {
        if (nFromRow == nToRow)
        {
            if (nFromColumn < nToColumn)
            {
                for (int i = nFromColumn + 1; i < nToColumn; i++)
                {
                    if (szChessMan[nFromRow][i] != 0)
                    {
                        return false;
                    }
                }
            }
            else
            {
                for (int i = nToColumn + 1; i < nFromColumn; i++)
                {
                    if (szChessMan[nFromRow][i] != 0)
                    {
                        return false;
                    }
                }
            }
        }
        else
        {
            if (nFromRow < nToRow)
            {
                for (int i = nFromRow + 1; i < nToRow; i++)
                {
                    if (szChessMan[i][nFromColumn] != 0)
                    {
                        return false;
                    }
                }
            }
            else
            {
                for (int i = nToRow + 1; i < nFromRow; i++)
                {
                    if (szChessMan[i][nFromColumn] != 0)
                    {
                        return false;
                    }
                }
            }
        }
    }
    else        //炮吃子时
    {
        int nCount = 0;
        if (nFromRow == nToRow)
        {
            if (nFromColumn < nToColumn)
            {
                for (int i = nFromColumn + 1; i < nToColumn; i++)
                {
                    if (szChessMan[nFromRow][i] != 0)
                    {
                        nCount++;
                    }
                }
            }
            else
            {
                for (int i = nToColumn + 1; i < nFromColumn; i++)
                {
                    if (szChessMan[nFromRow][i] != 0)
                    {
                        nCount++;
                    }
                }
            }
        }
        else
        {
            if (nFromRow < nToRow)
            {
                for (int i = nFromRow + 1; i < nToRow; i++)
                {
                    if (szChessMan[i][nFromColumn] != 0)
                    {
                        nCount++;
                    }
                }
            }
            else
            {
                for (int i = nToRow + 1; i < nFromRow; i++)
                {
                    if (szChessMan[i][nFromColumn] != 0)
                    {
                        nCount++;
                    }
                }
            }
        }

        if (nCount != 1)
        {
            return false;
        }
    }

    return true;
}

int CMoveRouteGenerator::GetChessManPosition( int szChessMan[][s_nChessBoardColumn], int nSpecChessMan, ChessManPosition stPos[] )
{
    int nIndex = 0;

    for (int i = 0; i < s_nChessBoardRow; i++)
    {
        for (int j = 0; j < s_nChessBoardColumn; j++)
        {
            if (szChessMan[i][j] == nSpecChessMan)
            {
                stPos[nIndex].nRow = i;
                stPos[nIndex].nColumn = j;
                nIndex++;
            }
        }
    }

    return nIndex;
}

bool CMoveRouteGenerator::IsAttackGeneral( int szChessMan[][s_nChessBoardColumn], int nGeneral )
{
    bool bRet = false;

    ChessManPosition stPos[5];
    GetChessManPosition(szChessMan, nGeneral, stPos);

    if (nGeneral == s_nBlackGeneral)
    {
        bRet = AttackGeneral(szChessMan, stPos[0].nRow, stPos[0].nColumn, s_nRedChariot) || 
            AttackGeneral(szChessMan, stPos[0].nRow, stPos[0].nColumn, s_nRedCannon) ||
            AttackGeneral(szChessMan, stPos[0].nRow, stPos[0].nColumn, s_nRedSoldier);
    }
    else
    {
        bRet = AttackGeneral(szChessMan, stPos[0].nRow, stPos[0].nColumn, s_nBlackChariot) || 
            AttackGeneral(szChessMan, stPos[0].nRow, stPos[0].nColumn, s_nBlackCannon) ||
            AttackGeneral(szChessMan, stPos[0].nRow, stPos[0].nColumn, s_nBlackSoldier);
    }

    return bRet;
}

bool CMoveRouteGenerator::AttackGeneral( int szChessMan[][s_nChessBoardColumn], int nRow, int nColumn, int nSpecChessMan )
{
    ChessManPosition stPos[5];
    bool bIsValid[5];
    memset(bIsValid, 0, 5);

    int nChessManCount = GetChessManPosition(szChessMan, nSpecChessMan, stPos);
    for (int i = 0; i < nChessManCount; i++)
    {
        bIsValid[i] = ValidateMoveRoute(szChessMan, stPos[i].nRow, stPos[i].nColumn, nRow, nColumn);
    }

    bool bCanAttack = false;
    for (int i = 0; i < 5; i++)
    {
        if (bIsValid[i])
        {
            bCanAttack = true;
            break;
        }
    }

    return bCanAttack;
}

bool CMoveRouteGenerator::MeetWithGeneral( int szChessMan[][s_nChessBoardColumn], int nGeneral )
{
    bool bRet = false;

    ChessManPosition stPos[5];
    GetChessManPosition(szChessMan, nGeneral, stPos);

    if (nGeneral == s_nBlackGeneral)
    {
        bRet = AttackGeneral(szChessMan, stPos[0].nRow, stPos[0].nColumn, s_nRedGeneral);
    }
    else
    {
        bRet = AttackGeneral(szChessMan, stPos[0].nRow, stPos[0].nColumn, s_nBlackGeneral);
    }

    return bRet;
}
