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
 *  File:		MoveRouteGenerator.cpp
 *  Author:		Richard Zou
 *	Created on:	2015-03-01
 */

#include "MoveRouteGenerator.h"
#include "GameSettings.h"

CMoveRouteGenerator::CMoveRouteGenerator(void)
{
}

CMoveRouteGenerator::~CMoveRouteGenerator(void)
{
}

bool CMoveRouteGenerator::ValidateMoveRoute(int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, 
                                           int nFromColumn, int nToRow, int nToColumn)
{
    int nMovingChessMan = szChessMan[nFromRow][nFromColumn];
    int nKilledChessMan = szChessMan[nToRow][nToColumn];
    
    if (nFromRow == nToRow && nFromColumn == nToColumn)
    {
        return false;
    }

    if (IsSameSide(nMovingChessMan, nKilledChessMan))
    {
        return false;
    }
    
    bool bRet = false;

    switch(nMovingChessMan)
    {
    case BLACK_GENERAL:
        {
            bRet = ValidateBlackGeneralMoveRoute(szChessMan, nFromRow, nFromColumn, nToRow, nToColumn);
        }
        break;

    case RED_GENERAL:
        {
            bRet = ValidateRedGeneralMoveRoute(szChessMan, nFromRow, nFromColumn, nToRow, nToColumn);
        }
        break;

    case BLACK_ADVISOR:
        {
           bRet = ValidateBlackAdvisorMoveRoute(szChessMan, nFromRow, nFromColumn, nToRow, nToColumn);
        }
        break;

    case RED_ADVISOR:
        {
            bRet = ValidateRedAdvisorMoveRoute(szChessMan, nFromRow, nFromColumn, nToRow, nToColumn);
        }
        break;

    case BLACK_MINISTER:
        {
            bRet = ValidateBlackMinisterMoveRoute(szChessMan, nFromRow, nFromColumn, nToRow, nToColumn);
        }
        break;

    case RED_MINISTER:
        {
            bRet = ValidateRedMinisterMoveRoute(szChessMan, nFromRow, nFromColumn, nToRow, nToColumn);
        }
        break;
    
    case BLACK_SOLDIER:
        {
            bRet = ValidateBlackSoldierMoveRoute(szChessMan, nFromRow, nFromColumn, nToRow, nToColumn);
        }
        break;

    case RED_SOLDIER:
        {
            bRet = ValidateRedSoldierMoveRoute(szChessMan, nFromRow, nFromColumn, nToRow, nToColumn);
        }
        break;

    case BLACK_CHARIOT:
    case RED_CHARIOT:
        {
            bRet = ValidateChariotMoveRoute(szChessMan, nFromRow, nFromColumn, nToRow, nToColumn);
        }
        break;

    case BLACK_HORSE:
    case RED_HORSE:
        {
            bRet = ValidateHorseMoveRoute(szChessMan, nFromRow, nFromColumn, nToRow, nToColumn);
        }
        break;

    case BLACK_CANON:
    case RED_CANNON:
        {
            bRet = ValidateCannonMoveRoute(szChessMan, nFromRow, nFromColumn, nToRow, nToColumn);
        }
        break;

    default:
        break;
    }

    return bRet;
}

bool CMoveRouteGenerator::ValidateBlackGeneralMoveRoute( int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn )
{
    int nKilledChessMan = szChessMan[nToRow][nToColumn];
    if (nKilledChessMan == RED_GENERAL)  //��˧��ͬһ��ֱ���ϣ��м䲻�ܸ�����
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
            return false;           //Ŀ�겻���ھŹ�֮��
        }

        if (abs(nFromRow - nToRow) + abs(nToColumn - nFromColumn) > 1)
        {
            return false;           //��˧ֻ��һ��ֱ��
        }
    }

    return true;
}

bool CMoveRouteGenerator::ValidateRedGeneralMoveRoute( int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn )
{
    int nKilledChessMan = szChessMan[nToRow][nToColumn];
    if (nKilledChessMan == BLACK_GENERAL)  //��˧��ͬһ��ֱ���ϣ��м䲻�ܸ�����
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
            return false;           //Ŀ�겻���ھŹ�֮��
        }

        if (abs(nFromRow - nToRow) + abs(nToColumn - nFromColumn) > 1)
        {
            return false;           //��˧ֻ��һ��ֱ��
        }
    }

    return true;
}

bool CMoveRouteGenerator::ValidateBlackAdvisorMoveRoute( int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn )
{
    if (nToRow > 2 || nToColumn > 5 || nToColumn < 3)  //ʿ���Ź�
    {
        return false;
    }

    if (abs(nFromRow - nToRow) != 1 || abs(nFromColumn - nToColumn) != 1) //ʿ��б��
    {
        return false;
    }

    return true;
}

bool CMoveRouteGenerator::ValidateRedAdvisorMoveRoute( int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn )
{
    if (nToRow < 7 || nToColumn > 5 || nToColumn < 3)  //ʿ���Ź�
    {
        return false;
    }

    if (abs(nFromRow - nToRow) != 1 || abs(nFromColumn - nToColumn) != 1) //ʿ��б��
    {
        return false;
    }

    return true;
}

bool CMoveRouteGenerator::ValidateBlackMinisterMoveRoute( int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn )
{
    if (nToRow > 4)
    {
        return false;       //���ܹ���
    }

    if (abs(nFromRow - nToRow) != 2 || abs(nFromColumn - nToColumn) != 2)
    {
        return false;       //������
    }

    if (szChessMan[(nFromRow + nToRow) / 2][(nFromColumn + nToColumn) / 2] != 0)
    {
        return false;       //���۱���
    }

    return true;
}

bool CMoveRouteGenerator::ValidateRedMinisterMoveRoute( int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn )
{
    if (nToRow < 5)
    {
        return false;       //���ܹ���
    }

    if (abs(nFromRow - nToRow) != 2 || abs(nFromColumn - nToColumn) != 2)
    {
        return false;       //������
    }

    if (szChessMan[(nFromRow + nToRow) / 2][(nFromColumn + nToColumn) / 2] != 0)
    {
        return false;       //���۱���
    }

    return true;
}

bool CMoveRouteGenerator::ValidateBlackSoldierMoveRoute( int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn )
{
    if (nToRow < nFromRow)
    {
        return false;   //������ͷ
    }
    
    if (nFromRow < 5 && nFromRow == nToRow)
    {
        return false;   //������ǰֻ��ֱ��
    }

    if (nToRow - nFromRow + abs(nToColumn - nFromColumn) > 1)
    {
        return false;   //��ÿ��ֻ��һ��
    }

    return true;
}

bool CMoveRouteGenerator::ValidateRedSoldierMoveRoute( int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn )
{
    if (nToRow > nFromRow)
    {
        return false;
    }

    if (nFromRow > 4 && nFromRow == nToRow)
    {
        return false;    //������ǰֻ��ֱ��
    }

    if (nFromRow - nToRow + abs(nToColumn - nFromColumn) > 1)
    {
        return false;   //��ÿ��ֻ��һ��
    }

    return true;
}

bool CMoveRouteGenerator::ValidateChariotMoveRoute( int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn )
{
    if (nFromRow != nToRow && nFromColumn != nToColumn)
    {
        return false;
    }

    if (nFromRow == nToRow)   //ˮƽ����
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
    else                    //��ֱ����
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

    return true;
}

bool CMoveRouteGenerator::ValidateHorseMoveRoute( int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn )
{
    if (!((abs(nFromRow - nToRow) == 1 && abs(nFromColumn - nToColumn) == 2) ||
        (abs(nFromRow - nToRow) == 2 && abs(nFromColumn - nToColumn) == 1)))
    {
        return false;       //������
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
        return false;   //������
    }

    return true;
}

bool CMoveRouteGenerator::ValidateCannonMoveRoute( int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn )
{
    if (nFromRow != nToRow && nFromColumn != nToColumn)
    {
        return false;       //����ֱ��
    }

    //�ڲ�����ʱ������·���в���������
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
    else        //�ڳ���ʱ
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

int CMoveRouteGenerator::GetChessManPosition( int szChessMan[][CHESSBOARD_COLUMN], int nSpecChessMan, ChessManPosition stPos[] )
{
    int nIndex = 0;

    for (int i = 0; i < CHESSBOARD_ROW; i++)
    {
        for (int j = 0; j < CHESSBOARD_COLUMN; j++)
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

bool CMoveRouteGenerator::IsAttackGeneral( int szChessMan[][CHESSBOARD_COLUMN], int nGeneral )
{
    bool bRet = false;

    ChessManPosition stPos[5];
    GetChessManPosition(szChessMan, nGeneral, stPos);

    if (nGeneral == BLACK_GENERAL)
    {
        bRet = AttackGeneral(szChessMan, stPos[0].nRow, stPos[0].nColumn, RED_CHARIOT) || 
            AttackGeneral(szChessMan, stPos[0].nRow, stPos[0].nColumn, RED_CANNON) ||
            AttackGeneral(szChessMan, stPos[0].nRow, stPos[0].nColumn, RED_SOLDIER) ||
            AttackGeneral(szChessMan, stPos[0].nRow, stPos[0].nColumn, RED_HORSE) ||
            AttackGeneral(szChessMan, stPos[0].nRow, stPos[0].nColumn, RED_GENERAL);
    }
    else
    {
        bRet = AttackGeneral(szChessMan, stPos[0].nRow, stPos[0].nColumn, BLACK_CHARIOT) || 
            AttackGeneral(szChessMan, stPos[0].nRow, stPos[0].nColumn, BLACK_CANON) ||
            AttackGeneral(szChessMan, stPos[0].nRow, stPos[0].nColumn, BLACK_SOLDIER) ||
            AttackGeneral(szChessMan, stPos[0].nRow, stPos[0].nColumn, BLACK_HORSE) ||
            AttackGeneral(szChessMan, stPos[0].nRow, stPos[0].nColumn, BLACK_GENERAL);
    }

    return bRet;
}

bool CMoveRouteGenerator::AttackGeneral( int szChessMan[][CHESSBOARD_COLUMN], int nRow, int nColumn, int nSpecChessMan )
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

void CMoveRouteGenerator::GetChessManMoveStepAlpha( int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char strAlphaFmt[] )
{
    int nMovingChessMan = szChessMan[nFromRow][nFromColumn];
    strAlphaFmt[0] = s_pChessManCode[nMovingChessMan - 1];

    switch(nMovingChessMan)
    {
    case BLACK_GENERAL:
        {
            GetBlackGeneralMoveStepAlpha(szChessMan, nFromRow, nFromColumn, nToRow, nToColumn, strAlphaFmt);
        }
        break;
    case BLACK_ADVISOR:
    case BLACK_MINISTER:
        {
            GetBlackAdvisorMinisterMoveStepAlpha(szChessMan, nFromRow, nFromColumn, nToRow, nToColumn, strAlphaFmt);
        }
        break;
    case BLACK_CHARIOT:
    case BLACK_CANON:
        {
            GetBlackChariotCannonMoveStepAlpha(szChessMan, nFromRow, nFromColumn, nToRow, nToColumn, strAlphaFmt, nMovingChessMan);
        }
        break;
    case BLACK_HORSE:
        {
            GetBlackHorseMoveStepAlpha(szChessMan, nFromRow, nFromColumn, nToRow, nToColumn, strAlphaFmt);
        }
        break;
    case BLACK_SOLDIER:
        {
            GetBlackSoldierMoveStepAlpha(szChessMan, nFromRow, nFromColumn, nToRow, nToColumn, strAlphaFmt);
        }
        break;
    case RED_GENERAL:
        {
            GetRedGeneralMoveStepAlpha(szChessMan, nFromRow, nFromColumn, nToRow, nToColumn, strAlphaFmt);
        }
        break;
    case RED_ADVISOR:
    case RED_MINISTER:
        {
            GetRedAdvisorMinisterMoveStepAlpha(szChessMan, nFromRow, nFromColumn, nToRow, nToColumn, strAlphaFmt);
        }
        break;
    case RED_CHARIOT:
    case RED_CANNON:
        {
            GetRedChariotCannonMoveStepAlpha(szChessMan, nFromRow, nFromColumn, nToRow, nToColumn, strAlphaFmt, nMovingChessMan);
        }
        break;
    case RED_HORSE:
        {
            GetRedHorseMoveStepAlpha(szChessMan, nFromRow, nFromColumn, nToRow, nToColumn, strAlphaFmt);
        }
        break;
    case RED_SOLDIER:
        {
            GetRedSoldierMoveStepAlpha(szChessMan, nFromRow, nFromColumn, nToRow, nToColumn, strAlphaFmt);
        }
        break;
    default:
        break;
    }

}

void CMoveRouteGenerator::GetBlackGeneralMoveStepAlpha( int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char strAlphaFmt[] )
{
    strAlphaFmt[1] = nFromColumn + 1 + '0';
    //�������ȣ���ƽ��
    if (nFromRow == nToRow)
    {
        strAlphaFmt[2] = '.';
        strAlphaFmt[3] = nToColumn + 1 + '0';
    }
    else
    {
        strAlphaFmt[2] = nFromRow > nToRow ? '-' : '+';
        strAlphaFmt[3] = abs(nFromRow - nToRow) + '0';
    }
}

void CMoveRouteGenerator::GetRedGeneralMoveStepAlpha( int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char strAlphaFmt[] )
{
    strAlphaFmt[1] = CHESSBOARD_COLUMN - nFromColumn + '0';
    if (nFromRow == nToRow)
    {
        strAlphaFmt[2] = '.';
        strAlphaFmt[3] = CHESSBOARD_COLUMN - nToColumn + '0';
    }
    else
    {
        strAlphaFmt[2] = nFromRow > nToRow ? '+' : '-';
        strAlphaFmt[3] = abs(nFromRow - nToRow) + '0';
    }
}

void CMoveRouteGenerator::GetBlackAdvisorMinisterMoveStepAlpha( int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char strAlphaFmt[] )
{
    strAlphaFmt[1] = nFromColumn + 1 + '0';
    strAlphaFmt[2] = nFromRow > nToRow ? '-' : '+';
    strAlphaFmt[3] = nToColumn + 1 + '0';
}

void CMoveRouteGenerator::GetRedAdvisorMinisterMoveStepAlpha( int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char strAlphaFmt[] )
{
    strAlphaFmt[1] = CHESSBOARD_COLUMN - nFromColumn + '0';
    strAlphaFmt[2] = nFromRow > nToRow ? '+' : '-';
    strAlphaFmt[3] = CHESSBOARD_COLUMN - nToColumn + '0';
}

//������ͬ
void CMoveRouteGenerator::GetBlackChariotCannonMoveStepAlpha( int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char strAlphaFmt[], int nChessMan )
{
    //��ȡ��/�ڵ�λ�ã��������������ͬһ����Ҫ��ǰ��
    ChessManPosition stPos[2];
    int nBlackChariotCount = GetChessManPosition(szChessMan, nChessMan, stPos);
    //���������/����ͬһ����
    if (stPos[0].nColumn == stPos[1].nColumn)
    {
        if (stPos[0].nRow > stPos[1].nRow)
        {
            if (nFromRow == stPos[0].nRow)
            {
                strAlphaFmt[1] = 'a';
            }
            else
            {
                strAlphaFmt[1] = 'b';
            }
        }
        else
        {
            if (nFromRow == stPos[0].nRow)
            {
                strAlphaFmt[1] = 'b';
            }
            else
            {
                strAlphaFmt[1] = 'a';
            }
        }
    }
    else
    {
        strAlphaFmt[1] = nFromColumn + 1 + '0';
    }

    if (nFromRow == nToRow)
    {
        strAlphaFmt[2] = '.';
        strAlphaFmt[3] = nToColumn + 1 + '0';
    }
    else
    {
        strAlphaFmt[2] = nFromRow > nToRow ? '-' : '+';
        strAlphaFmt[3] = abs(nFromRow - nToRow) + '0';
    }
}

//������ͬ
void CMoveRouteGenerator::GetRedChariotCannonMoveStepAlpha( int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char strAlphaFmt[], int nChessMan )
{
    //��ȡ����λ�ã��������������ͬһ����Ҫ��ǰ��
    ChessManPosition stPos[2];
    GetChessManPosition(szChessMan, nChessMan, stPos);
    //�����������ͬһ����
    if (stPos[0].nColumn == stPos[1].nColumn)
    {
        if (stPos[0].nRow > stPos[1].nRow)
        {
            if (nFromRow == stPos[0].nRow)
            {
                strAlphaFmt[1] = 'b';
            }
            else
            {
                strAlphaFmt[1] = 'a';
            }
        }
        else
        {
            if (nFromRow == stPos[0].nRow)
            {
                strAlphaFmt[1] = 'a';
            }
            else
            {
                strAlphaFmt[1] = 'b';
            }
        }
    }
    else
    {
        strAlphaFmt[1] = CHESSBOARD_COLUMN - nFromColumn + '0';
    }

    if (nFromRow == nToRow)
    {
        strAlphaFmt[2] = '.';
        strAlphaFmt[3] = CHESSBOARD_COLUMN - nToColumn + '0';
    }
    else
    {
        strAlphaFmt[2] = nFromRow > nToRow ? '+' : '-';
        strAlphaFmt[3] = abs(nFromRow - nToRow) + '0';
    }
}

void CMoveRouteGenerator::GetBlackHorseMoveStepAlpha( int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char strAlphaFmt[] )
{
    //��ȡ���λ�ã��������������ͬһ����Ҫ��ǰ��
    ChessManPosition stPos[2];
    GetChessManPosition(szChessMan, BLACK_HORSE, stPos);
    //�����������ͬһ����
    if (stPos[0].nColumn == stPos[1].nColumn)
    {
        if (stPos[0].nRow > stPos[1].nRow)
        {
            if (nFromRow == stPos[0].nRow)
            {
                strAlphaFmt[1] = 'a';
            }
            else
            {
                strAlphaFmt[1] = 'b';
            }
        }
        else
        {
            if (nFromRow == stPos[0].nRow)
            {
                strAlphaFmt[1] = 'b';
            }
            else
            {
                strAlphaFmt[1] = 'a';
            }
        }
    }
    else
    {
        strAlphaFmt[1] = nFromColumn + 1 + '0';
    }

    strAlphaFmt[2] = nFromRow > nToRow ? '-' : '+';
    strAlphaFmt[3] = nToColumn + 1 + '0';
}

void CMoveRouteGenerator::GetRedHorseMoveStepAlpha( int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char strAlphaFmt[] )
{
    //��ȡ���λ�ã��������������ͬһ����Ҫ��ǰ��
    ChessManPosition stPos[2];
    GetChessManPosition(szChessMan, RED_HORSE, stPos);
    //�����������ͬһ����
    if (stPos[0].nColumn == stPos[1].nColumn)
    {
        if (stPos[0].nRow > stPos[1].nRow)
        {
            if (nFromRow == stPos[0].nRow)
            {
                strAlphaFmt[1] = 'b';
            }
            else
            {
                strAlphaFmt[1] = 'a';
            }
        }
        else
        {
            if (nFromRow == stPos[0].nRow)
            {
                strAlphaFmt[1] = 'a';
            }
            else
            {
                strAlphaFmt[1] = 'b';
            }
        }
    }
    else
    {
        strAlphaFmt[1] = CHESSBOARD_COLUMN - nFromColumn + '0';
    }

    strAlphaFmt[2] = nFromRow > nToRow ? '+' : '-';
    strAlphaFmt[3] = CHESSBOARD_COLUMN - nToColumn + '0';
}


void CMoveRouteGenerator::GetBlackSoldierMoveStepAlpha( int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char strAlphaFmt[] )
{
    strAlphaFmt[1] = nFromColumn + 1 + '0';
    //��ȡ����λ��
    ChessManPosition stPos[5];
    int SoldierCount = GetChessManPosition(szChessMan, BLACK_SOLDIER, stPos);

    int nMark = 0;
    for (int i = 0; i < SoldierCount; i++)
    {
        if (stPos[i].nColumn == nFromColumn)
        {
            nMark++;
        }
    }

    if (nMark > 1)
    {
        int szSoldierCountOnEachColumn[CHESSBOARD_COLUMN] = {0};                                                  //���������ϣ�ÿ�����ϱ��ĸ���
        int szColumnForEachSoldier[5] = {-1, -1, -1, -1, -1};                                                       //5����������Щ������
        int n = 0;
        int nStart = 0;

        for (int i = 0; i < SoldierCount; i++)
        {
            szSoldierCountOnEachColumn[stPos[i].nColumn]++;
        }

        for (int i = 0; i < CHESSBOARD_COLUMN; i++)
        {
            if (szSoldierCountOnEachColumn[i] > 1)
            {
                //���������������ϵı�
                for (int j = 0; j < SoldierCount; j++)
                {
                    if (i == stPos[j].nColumn)
                    {
                        szColumnForEachSoldier[n] = j;
                        n++;
                    }
                }

                //Ϊ�����ϵ���������
                for (int k = nStart; k < n - 1; k++)
                {
                    for (int j = n - 2; j >= k; j--)
                    {
                        if (stPos[szColumnForEachSoldier[j]].nRow < stPos[szColumnForEachSoldier[j + 1]].nRow)
                        {
                            int nTmp = szColumnForEachSoldier[j];
                            szColumnForEachSoldier[j] = szColumnForEachSoldier[j + 1];
                            szColumnForEachSoldier[j + 1] = nTmp;
                        }
                    }
                }
                nStart = n;
            }
        }

        char chOrder = 'c'; //ǰ
        for (int i = 0; i < SoldierCount; i++)
        {
            if (stPos[szColumnForEachSoldier[i]].nRow == nToRow)
            {
                strAlphaFmt[1] = chOrder;
            }
            chOrder++;
        }

        //ת����ǰ���ǰ�к�
        if (nStart == 2)
        {
            strAlphaFmt[1] = strAlphaFmt[1] == 'c' ? 'q' : 'h';
        }
        else if (nStart == 3)
        {
            strAlphaFmt[1] = strAlphaFmt[1] == 'c' ? 'q' : (strAlphaFmt[1] == 'd' ? 'z' : 'h');
        }
    }

    if (nFromRow == nToRow)
    {
        strAlphaFmt[2] = '.';
        strAlphaFmt[3] = nToColumn + 1 + '0';
    }
    else
    {
        strAlphaFmt[2] = nFromRow > nToRow ? '-' : '+';
        strAlphaFmt[3] = abs(nFromRow - nToRow) + '0';
    }

}

void CMoveRouteGenerator::GetRedSoldierMoveStepAlpha( int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char strAlphaFmt[] )
{
    strAlphaFmt[1] = CHESSBOARD_COLUMN - nFromColumn + '0';

    //��ȡ����λ��
    ChessManPosition stPos[5];
    int SoldierCount = GetChessManPosition(szChessMan, RED_SOLDIER, stPos);

    int nMark = 0;
    for (int i = 0; i < SoldierCount; i++)
    {
        if (stPos[i].nColumn == nFromColumn)
        {
            nMark++;
        }
    }

    if (nMark > 1)
    {
        int szSoldierCountOnEachColumn[CHESSBOARD_COLUMN] = {0};                                                  //���������ϣ�ÿ�����ϱ��ĸ���
        int szColumnForEachSoldier[5] = {-1, -1, -1, -1, -1};                                                       //5��������ӦstPos����Щ�±�
        int n = 0;
        int nStart = 0;

        for (int i = 0; i < SoldierCount; i++)
        {
            szSoldierCountOnEachColumn[stPos[i].nColumn]++;
        }

        for (int i = CHESSBOARD_COLUMN - 1; i >= 0; i--)
        {
            if (szSoldierCountOnEachColumn[i] > 1)
            {
                //���������������ϵı�
                for (int j = 0; j < SoldierCount; j++)
                {
                    if (i == stPos[j].nColumn)
                    {
                        szColumnForEachSoldier[n] = j;
                        n++;
                    }
                }

                //Ϊ�����ϵ���������
                for (int k = nStart; k < n - 1; k++)
                {
                    for (int j = n - 2; j >= k; j--)
                    {
                        if (stPos[szColumnForEachSoldier[j]].nRow > stPos[szColumnForEachSoldier[j + 1]].nRow)
                        {
                            int nTmp = szColumnForEachSoldier[j];
                            szColumnForEachSoldier[j] = szColumnForEachSoldier[j + 1];
                            szColumnForEachSoldier[j + 1] = nTmp;
                        }
                    }
                }
                nStart = n;
            }
        }

        char chOrder = 'c'; //ǰ
        for (int i = 0; i < SoldierCount; i++)
        {
            if (stPos[szColumnForEachSoldier[i]].nRow == nToRow)
            {
                strAlphaFmt[1] = chOrder;
            }
            chOrder++;
        }

        //ת����ǰ���ǰ�к�
        if (nStart == 2)
        {
            strAlphaFmt[1] = strAlphaFmt[1] == 'c' ? 'q' : 'h';
        }
        else if (nStart == 3)
        {
            strAlphaFmt[1] = strAlphaFmt[1] == 'c' ? 'q' : (strAlphaFmt[1] == 'd' ? 'z' : 'h');
        }
    }

    if (nFromRow == nToRow)
    {
        strAlphaFmt[2] = '.';
        strAlphaFmt[3] = CHESSBOARD_COLUMN - nToColumn + '0';
    }
    else
    {
        strAlphaFmt[2] = nFromRow > nToRow ? '+' : '-';
        strAlphaFmt[3] = abs(nFromRow - nToRow) + '0';
    }
}

void CMoveRouteGenerator::AlphaFmtToChiness(const char *strAlphaFmt, char strChinesFmt[], bool bBlack )
{
    string strDst;
    if (strAlphaFmt[1] >= 'a')
    {
        strDst = DigitToChinese(strAlphaFmt[1], bBlack) + 
                 ChessManCodetoChinese(strAlphaFmt[0], bBlack) + 
                 ActionToChinese(strAlphaFmt[2]) + 
                 DigitToChinese(strAlphaFmt[3], bBlack);
    }
    else
    {
        strDst = ChessManCodetoChinese(strAlphaFmt[0], bBlack) +
                 DigitToChinese(strAlphaFmt[1], bBlack) + 
                 ActionToChinese(strAlphaFmt[2]) + 
                 DigitToChinese(strAlphaFmt[3], bBlack);
    }

    strcpy(strChinesFmt, strDst.c_str());
}

std::string CMoveRouteGenerator::DigitToChinese( char chDigit, bool bBlack )
{
    string strDigit;
    if (bBlack)
    {
        switch(chDigit)
        {
        case '1':
            strDigit = "1";
            break;
        case '2':
            strDigit = "2";
            break;
        case '3':
            strDigit = "3";
            break;
        case '4':
            strDigit = "4";
            break;
        case '5':
            strDigit = "5";
            break;
        case '6':
            strDigit = "6";
            break;
        case '7':
            strDigit = "7";
            break;
        case '8':
            strDigit = "8";
            break;
        case '9':
            strDigit = "9";
            break;
        case 'a':
            strDigit = "ǰ";
            break;
        case 'b':
            strDigit = "��";
            break;
        case 'c':
            strDigit = "һ";
            break;
        case 'd':
            strDigit = "��";
            break;
        case 'e':
            strDigit = "��";
            break;
        case 'f':
            strDigit = "��";
            break;
        case 'g':
            strDigit = "��";
            break;
        case 'q':
            strDigit = "ǰ";
            break;
        case 'z':
            strDigit = "��";
            break;
        case 'h':
            strDigit = "��";
            break;
        default:
            break;
        }
    }
    else
    {
        switch(chDigit)
        {
        case '1':
            strDigit = "һ";
            break;
        case '2':
            strDigit = "��";
            break;
        case '3':
            strDigit = "��";
            break;
        case '4':
            strDigit = "��";
            break;
        case '5':
            strDigit = "��";
            break;
        case '6':
            strDigit = "��";
            break;
        case '7':
            strDigit = "��";
            break;
        case '8':
            strDigit = "��";
            break;
        case '9':
            strDigit = "��";
            break;
        case 'a':
            strDigit = "ǰ";
            break;
        case 'b':
            strDigit = "��";
            break;
        case 'c':
            strDigit = "һ";
            break;
        case 'd':
            strDigit = "��";
            break;
        case 'e':
            strDigit = "��";
            break;
        case 'f':
            strDigit = "��";
            break;
        case 'g':
            strDigit = "��";
            break;
        case 'q':
            strDigit = "ǰ";
            break;
        case 'z':
            strDigit = "��";
            break;
        case 'h':
            strDigit = "��";
            break;
        default:
            break;
        }
    }

    return strDigit;
}

std::string CMoveRouteGenerator::ChessManCodetoChinese( char chCode, bool bBlack )
{
    string strCode;
    switch(chCode)
    {
    case 'k':
        strCode = "��";
        break;
    case 'r':
        strCode = "��";
        break;
    case 'n':
        strCode = "��";
        break;
    case 'c':
        strCode = "��";
        break;
    case 'a':
        strCode = "ʿ";
        break;
    case 'b':
        strCode = "��";
        break;
    case 'p':
        strCode = "��";
        break;
    case 'K':
        strCode = "˧";
        break;
    case 'R':
        strCode = "��";
        break;
    case 'N':
        strCode = "��";
        break;
    case 'C':
        strCode = "��";
        break;
    case 'A':
        strCode = "��";
        break;
    case 'B':
        strCode = "��";
        break;
    case 'P':
        strCode = "��";
        break;
    default:
        break;
    }

    return strCode;
}

std::string CMoveRouteGenerator::ActionToChinese( char chAction)
{
    string strAction;
    switch(chAction)
    {
    case '.':
        strAction = "ƽ";
        break;
    case '+':
        strAction = "��";
        break;
    case '-':
        strAction = "��";
        break;
    default:
        break;
    }
    
    return strAction;
}

//�����߷�
void CMoveRouteGenerator::GenerateAllMoveRoute( int szChessMan[][CHESSBOARD_COLUMN], int nSide, 
                                            list<MoveRoute> &lstMoveRoute )
{
    for (int i = 0; i < CHESSBOARD_ROW; i++)
    {
        for (int j = 0; j < CHESSBOARD_COLUMN; j++)
        {
            if ((nSide == BLACK && IsBlackSide(szChessMan[i][j])) 
                || (nSide == RED && IsRedSide(szChessMan[i][j])))
            {
                GenerateMoveRoute(szChessMan, i, j, lstMoveRoute);
            }
        }
    }
}

void CMoveRouteGenerator::GenerateMoveRoute( int szChessMan[][CHESSBOARD_COLUMN], int nRow, int nColumn, list<MoveRoute> &lstMoveRoute )
{
    int nKilledChessMan = 0;
    int nMovingChessMan = szChessMan[nRow][nColumn];
    int nGeneral = IsBlackSide(nMovingChessMan) ? BLACK_GENERAL : RED_GENERAL;
    int nOppGeneral = IsBlackSide(nMovingChessMan) ? RED_GENERAL : BLACK_GENERAL;

    MoveRoute stMoveRoute;
    stMoveRoute.stFromPos.nRow = nRow;
    stMoveRoute.stFromPos.nColumn = nColumn;
    stMoveRoute.nMovingChessMan = nMovingChessMan;

    for (int i = 0; i < CHESSBOARD_ROW; i++)
    {
        for (int j = 0; j < CHESSBOARD_COLUMN; j++)
        {
            if (ValidateMoveRoute(szChessMan, nRow, nColumn, i, j))
            {
                nKilledChessMan = szChessMan[i][j];

                //���ж�������Լ��Ƿ񱻶Է�����������Լ����Է����������߷�������
                szChessMan[nRow][nColumn] = 0;
                szChessMan[i][j] = nMovingChessMan;

                if (!IsAttackGeneral(szChessMan, nGeneral))
                {
                    stMoveRoute.bAttackGeneral = IsAttackGeneral(szChessMan, nOppGeneral);
                    stMoveRoute.stToPos.nRow = i;
                    stMoveRoute.stToPos.nColumn = j;
                    stMoveRoute.nKilledChessMan = nKilledChessMan;
                    lstMoveRoute.push_back(stMoveRoute);
                }

                szChessMan[nRow][nColumn] = nMovingChessMan;
                szChessMan[i][j] = nKilledChessMan;
            }
        }
    }
}

//�����������ж�
bool CMoveRouteGenerator::IsGeneralDead( int szChessMan[][CHESSBOARD_COLUMN], int nSide)
{
    bool bDead = true;
    int nGeneral = nSide == BLACK ? BLACK_GENERAL : RED_GENERAL;
    
    //�ж��ܷ��������ľ���
    for (int i = 0; i < CHESSBOARD_ROW; i++)
    {
        for (int j = 0; j < CHESSBOARD_COLUMN; j++)
        {
            if ((nSide == BLACK && IsBlackSide(szChessMan[i][j])) 
                || (nSide == RED && IsRedSide(szChessMan[i][j])))
            {
                if(CanGenerateMove(szChessMan, i, j, nGeneral))
                {
                    bDead = false;
                    break;
                }
            }
        }
    }

    return bDead;
}

bool CMoveRouteGenerator::CanGenerateMove( int szChessMan[][CHESSBOARD_COLUMN], int nRow, int nColumn, int nGeneral )
{
    bool bGenerate = false;
    int nKilledChessMan = 0;
    int nMovingChessMan = szChessMan[nRow][nColumn];

    for (int i = 0; i < CHESSBOARD_ROW; i++)
    {
        for (int j = 0; j < CHESSBOARD_COLUMN; j++)
        {
            if (ValidateMoveRoute(szChessMan, nRow, nColumn, i, j))
            {
                nKilledChessMan = szChessMan[i][j];

                //���ж�������Լ��Ƿ񱻶Է�����������Լ����Է����������߷�������
                szChessMan[nRow][nColumn] = 0;
                szChessMan[i][j] = nMovingChessMan;

                if (!IsAttackGeneral(szChessMan, nGeneral))
                {
                    bGenerate = true;
                }

                szChessMan[nRow][nColumn] = nMovingChessMan;
                szChessMan[i][j] = nKilledChessMan;

                if (bGenerate)
                {
                    break;
                }
            }
        }
    }

    return bGenerate;
}
