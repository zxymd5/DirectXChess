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

bool CMoveRouteGenerator::ValidateMoveRoute(int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, 
                                           int nFromColumn, int nToRow, int nToColumn)
{
    int nMovingChessMan = arrChessMan[nFromRow][nFromColumn];
    int nKilledChessMan = arrChessMan[nToRow][nToColumn];
    
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
            bRet = ValidateBlackGeneralMoveRoute(arrChessMan, nFromRow, nFromColumn, nToRow, nToColumn);
        }
        break;

    case RED_GENERAL:
        {
            bRet = ValidateRedGeneralMoveRoute(arrChessMan, nFromRow, nFromColumn, nToRow, nToColumn);
        }
        break;

    case BLACK_ADVISOR:
        {
           bRet = ValidateBlackAdvisorMoveRoute(arrChessMan, nFromRow, nFromColumn, nToRow, nToColumn);
        }
        break;

    case RED_ADVISOR:
        {
            bRet = ValidateRedAdvisorMoveRoute(arrChessMan, nFromRow, nFromColumn, nToRow, nToColumn);
        }
        break;

    case BLACK_MINISTER:
        {
            bRet = ValidateBlackMinisterMoveRoute(arrChessMan, nFromRow, nFromColumn, nToRow, nToColumn);
        }
        break;

    case RED_MINISTER:
        {
            bRet = ValidateRedMinisterMoveRoute(arrChessMan, nFromRow, nFromColumn, nToRow, nToColumn);
        }
        break;
    
    case BLACK_SOLDIER:
        {
            bRet = ValidateBlackSoldierMoveRoute(arrChessMan, nFromRow, nFromColumn, nToRow, nToColumn);
        }
        break;

    case RED_SOLDIER:
        {
            bRet = ValidateRedSoldierMoveRoute(arrChessMan, nFromRow, nFromColumn, nToRow, nToColumn);
        }
        break;

    case BLACK_CHARIOT:
    case RED_CHARIOT:
        {
            bRet = ValidateChariotMoveRoute(arrChessMan, nFromRow, nFromColumn, nToRow, nToColumn);
        }
        break;

    case BLACK_HORSE:
    case RED_HORSE:
        {
            bRet = ValidateHorseMoveRoute(arrChessMan, nFromRow, nFromColumn, nToRow, nToColumn);
        }
        break;

    case BLACK_CANON:
    case RED_CANNON:
        {
            bRet = ValidateCannonMoveRoute(arrChessMan, nFromRow, nFromColumn, nToRow, nToColumn);
        }
        break;

    default:
        break;
    }

    return bRet;
}

bool CMoveRouteGenerator::ValidateBlackGeneralMoveRoute( int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn )
{
    int nKilledChessMan = arrChessMan[nToRow][nToColumn];
    if (nKilledChessMan == RED_GENERAL)  //��˧��ͬһ��ֱ���ϣ��м䲻�ܸ�����
    {
        if (nFromColumn != nToColumn)
        {
            return false;
        }

        for (int i = nFromRow + 1; i < nToRow; i++)
        {
            if (arrChessMan[i][nFromColumn] != 0)
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

bool CMoveRouteGenerator::ValidateRedGeneralMoveRoute( int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn )
{
    int nKilledChessMan = arrChessMan[nToRow][nToColumn];
    if (nKilledChessMan == BLACK_GENERAL)  //��˧��ͬһ��ֱ���ϣ��м䲻�ܸ�����
    {
        if (nFromColumn != nToColumn)
        {
            return false;
        }

        for (int i = nFromRow - 1; i > nToRow; i--)
        {
            if (arrChessMan[i][nFromColumn] != 0)
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

bool CMoveRouteGenerator::ValidateBlackAdvisorMoveRoute( int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn )
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

bool CMoveRouteGenerator::ValidateRedAdvisorMoveRoute( int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn )
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

bool CMoveRouteGenerator::ValidateBlackMinisterMoveRoute( int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn )
{
    if (nToRow > 4)
    {
        return false;       //���ܹ���
    }

    if (abs(nFromRow - nToRow) != 2 || abs(nFromColumn - nToColumn) != 2)
    {
        return false;       //������
    }

    if (arrChessMan[(nFromRow + nToRow) / 2][(nFromColumn + nToColumn) / 2] != 0)
    {
        return false;       //���۱���
    }

    return true;
}

bool CMoveRouteGenerator::ValidateRedMinisterMoveRoute( int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn )
{
    if (nToRow < 5)
    {
        return false;       //���ܹ���
    }

    if (abs(nFromRow - nToRow) != 2 || abs(nFromColumn - nToColumn) != 2)
    {
        return false;       //������
    }

    if (arrChessMan[(nFromRow + nToRow) / 2][(nFromColumn + nToColumn) / 2] != 0)
    {
        return false;       //���۱���
    }

    return true;
}

bool CMoveRouteGenerator::ValidateBlackSoldierMoveRoute( int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn )
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

bool CMoveRouteGenerator::ValidateRedSoldierMoveRoute( int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn )
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

bool CMoveRouteGenerator::ValidateChariotMoveRoute( int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn )
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
                if (arrChessMan[nFromRow][i] != 0)
                {
                    return false;
                }
            }
        }
        else
        {
            for (int i = nToColumn + 1; i < nFromColumn; i++)
            {
                if (arrChessMan[nFromRow][i] != 0)
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
                if (arrChessMan[i][nFromColumn] != 0)
                {
                    return false;
                }
            }
        }
        else
        {
            for (int i = nToRow + 1; i < nFromRow; i++)
            {
                if (arrChessMan[i][nFromColumn] != 0)
                {
                    return false;
                }
            }
        }
    }

    return true;
}

bool CMoveRouteGenerator::ValidateHorseMoveRoute( int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn )
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

    if (arrChessMan[j][i] != 0)
    {
        return false;   //������
    }

    return true;
}

bool CMoveRouteGenerator::ValidateCannonMoveRoute( int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn )
{
    if (nFromRow != nToRow && nFromColumn != nToColumn)
    {
        return false;       //����ֱ��
    }

    //�ڲ�����ʱ������·���в���������
    if (arrChessMan[nToRow][nToColumn] == 0)
    {
        if (nFromRow == nToRow)
        {
            if (nFromColumn < nToColumn)
            {
                for (int i = nFromColumn + 1; i < nToColumn; i++)
                {
                    if (arrChessMan[nFromRow][i] != 0)
                    {
                        return false;
                    }
                }
            }
            else
            {
                for (int i = nToColumn + 1; i < nFromColumn; i++)
                {
                    if (arrChessMan[nFromRow][i] != 0)
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
                    if (arrChessMan[i][nFromColumn] != 0)
                    {
                        return false;
                    }
                }
            }
            else
            {
                for (int i = nToRow + 1; i < nFromRow; i++)
                {
                    if (arrChessMan[i][nFromColumn] != 0)
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
                    if (arrChessMan[nFromRow][i] != 0)
                    {
                        nCount++;
                    }
                }
            }
            else
            {
                for (int i = nToColumn + 1; i < nFromColumn; i++)
                {
                    if (arrChessMan[nFromRow][i] != 0)
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
                    if (arrChessMan[i][nFromColumn] != 0)
                    {
                        nCount++;
                    }
                }
            }
            else
            {
                for (int i = nToRow + 1; i < nFromRow; i++)
                {
                    if (arrChessMan[i][nFromColumn] != 0)
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

int CMoveRouteGenerator::GetChessManPosition( int arrChessMan[][CHESSBOARD_COLUMN], int nSpecChessMan, ChessManPosition stPos[] )
{
    int nIndex = 0;

    for (int i = 0; i < CHESSBOARD_ROW; i++)
    {
        for (int j = 0; j < CHESSBOARD_COLUMN; j++)
        {
            if (arrChessMan[i][j] == nSpecChessMan)
            {
                stPos[nIndex].nRow = i;
                stPos[nIndex].nColumn = j;
                nIndex++;
            }
        }
    }

    return nIndex;
}

bool CMoveRouteGenerator::IsAttackGeneral( int arrChessMan[][CHESSBOARD_COLUMN], int nGeneral )
{
    bool bAttack = false;

    ChessManPosition arrPos[5];
    GetChessManPosition(arrChessMan, nGeneral, arrPos);

    if (nGeneral == BLACK_GENERAL)
    {
        bAttack = AttackGeneral(arrChessMan, arrPos[0].nRow, arrPos[0].nColumn, RED_CHARIOT) || 
            AttackGeneral(arrChessMan, arrPos[0].nRow, arrPos[0].nColumn, RED_CANNON) ||
            AttackGeneral(arrChessMan, arrPos[0].nRow, arrPos[0].nColumn, RED_SOLDIER) ||
            AttackGeneral(arrChessMan, arrPos[0].nRow, arrPos[0].nColumn, RED_HORSE) ||
            AttackGeneral(arrChessMan, arrPos[0].nRow, arrPos[0].nColumn, RED_GENERAL);
    }
    else
    {
        bAttack = AttackGeneral(arrChessMan, arrPos[0].nRow, arrPos[0].nColumn, BLACK_CHARIOT) || 
            AttackGeneral(arrChessMan, arrPos[0].nRow, arrPos[0].nColumn, BLACK_CANON) ||
            AttackGeneral(arrChessMan, arrPos[0].nRow, arrPos[0].nColumn, BLACK_SOLDIER) ||
            AttackGeneral(arrChessMan, arrPos[0].nRow, arrPos[0].nColumn, BLACK_HORSE) ||
            AttackGeneral(arrChessMan, arrPos[0].nRow, arrPos[0].nColumn, BLACK_GENERAL);
    }

    return bAttack;
}

bool CMoveRouteGenerator::AttackGeneral( int arrChessMan[][CHESSBOARD_COLUMN], int nRow, int nColumn, int nSpecChessMan )
{
    ChessManPosition arrPos[5];
    bool arrValid[5];
    memset(arrValid, 0, 5);

    int nChessManCount = GetChessManPosition(arrChessMan, nSpecChessMan, arrPos);
    for (int i = 0; i < nChessManCount; i++)
    {
        arrValid[i] = ValidateMoveRoute(arrChessMan, arrPos[i].nRow, arrPos[i].nColumn, nRow, nColumn);
    }

    bool bCanAttack = false;
    for (int i = 0; i < 5; i++)
    {
        if (arrValid[i])
        {
            bCanAttack = true;
            break;
        }
    }

    return bCanAttack;
}

void CMoveRouteGenerator::GetChessManMoveStepAlpha( int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char szAlphaFmt[] )
{
    int nMovingChessMan = arrChessMan[nFromRow][nFromColumn];
    szAlphaFmt[0] = CHESSMAN_CODE[nMovingChessMan - 1];

    switch(nMovingChessMan)
    {
    case BLACK_GENERAL:
        {
            GetBlackGeneralMoveStepAlpha(arrChessMan, nFromRow, nFromColumn, nToRow, nToColumn, szAlphaFmt);
        }
        break;
    case BLACK_ADVISOR:
    case BLACK_MINISTER:
        {
            GetBlackAdvisorMinisterMoveStepAlpha(arrChessMan, nFromRow, nFromColumn, nToRow, nToColumn, szAlphaFmt);
        }
        break;
    case BLACK_CHARIOT:
    case BLACK_CANON:
        {
            GetBlackChariotCannonMoveStepAlpha(arrChessMan, nFromRow, nFromColumn, nToRow, nToColumn, szAlphaFmt, nMovingChessMan);
        }
        break;
    case BLACK_HORSE:
        {
            GetBlackHorseMoveStepAlpha(arrChessMan, nFromRow, nFromColumn, nToRow, nToColumn, szAlphaFmt);
        }
        break;
    case BLACK_SOLDIER:
        {
            GetBlackSoldierMoveStepAlpha(arrChessMan, nFromRow, nFromColumn, nToRow, nToColumn, szAlphaFmt);
        }
        break;
    case RED_GENERAL:
        {
            GetRedGeneralMoveStepAlpha(arrChessMan, nFromRow, nFromColumn, nToRow, nToColumn, szAlphaFmt);
        }
        break;
    case RED_ADVISOR:
    case RED_MINISTER:
        {
            GetRedAdvisorMinisterMoveStepAlpha(arrChessMan, nFromRow, nFromColumn, nToRow, nToColumn, szAlphaFmt);
        }
        break;
    case RED_CHARIOT:
    case RED_CANNON:
        {
            GetRedChariotCannonMoveStepAlpha(arrChessMan, nFromRow, nFromColumn, nToRow, nToColumn, szAlphaFmt, nMovingChessMan);
        }
        break;
    case RED_HORSE:
        {
            GetRedHorseMoveStepAlpha(arrChessMan, nFromRow, nFromColumn, nToRow, nToColumn, szAlphaFmt);
        }
        break;
    case RED_SOLDIER:
        {
            GetRedSoldierMoveStepAlpha(arrChessMan, nFromRow, nFromColumn, nToRow, nToColumn, szAlphaFmt);
        }
        break;
    default:
        break;
    }

}

void CMoveRouteGenerator::GetBlackGeneralMoveStepAlpha( int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char szAlphaFmt[] )
{
    szAlphaFmt[1] = nFromColumn + 1 + '0';
    //�������ȣ���ƽ��
    if (nFromRow == nToRow)
    {
        szAlphaFmt[2] = '.';
        szAlphaFmt[3] = nToColumn + 1 + '0';
    }
    else
    {
        szAlphaFmt[2] = nFromRow > nToRow ? '-' : '+';
        szAlphaFmt[3] = abs(nFromRow - nToRow) + '0';
    }
}

void CMoveRouteGenerator::GetRedGeneralMoveStepAlpha( int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char szAlphaFmt[] )
{
    szAlphaFmt[1] = CHESSBOARD_COLUMN - nFromColumn + '0';
    if (nFromRow == nToRow)
    {
        szAlphaFmt[2] = '.';
        szAlphaFmt[3] = CHESSBOARD_COLUMN - nToColumn + '0';
    }
    else
    {
        szAlphaFmt[2] = nFromRow > nToRow ? '+' : '-';
        szAlphaFmt[3] = abs(nFromRow - nToRow) + '0';
    }
}

void CMoveRouteGenerator::GetBlackAdvisorMinisterMoveStepAlpha( int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char szAlphaFmt[] )
{
    szAlphaFmt[1] = nFromColumn + 1 + '0';
    szAlphaFmt[2] = nFromRow > nToRow ? '-' : '+';
    szAlphaFmt[3] = nToColumn + 1 + '0';
}

void CMoveRouteGenerator::GetRedAdvisorMinisterMoveStepAlpha( int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char szAlphaFmt[] )
{
    szAlphaFmt[1] = CHESSBOARD_COLUMN - nFromColumn + '0';
    szAlphaFmt[2] = nFromRow > nToRow ? '+' : '-';
    szAlphaFmt[3] = CHESSBOARD_COLUMN - nToColumn + '0';
}

//������ͬ
void CMoveRouteGenerator::GetBlackChariotCannonMoveStepAlpha( int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char szAlphaFmt[], int nChessMan )
{
    //��ȡ��/�ڵ�λ�ã��������������ͬһ����Ҫ��ǰ��
    ChessManPosition arrPos[2];
    int nBlackChariotCount = GetChessManPosition(arrChessMan, nChessMan, arrPos);
    //���������/����ͬһ����
    if (arrPos[0].nColumn == arrPos[1].nColumn)
    {
        if (arrPos[0].nRow > arrPos[1].nRow)
        {
            if (nFromRow == arrPos[0].nRow)
            {
                szAlphaFmt[1] = 'a';
            }
            else
            {
                szAlphaFmt[1] = 'b';
            }
        }
        else
        {
            if (nFromRow == arrPos[0].nRow)
            {
                szAlphaFmt[1] = 'b';
            }
            else
            {
                szAlphaFmt[1] = 'a';
            }
        }
    }
    else
    {
        szAlphaFmt[1] = nFromColumn + 1 + '0';
    }

    if (nFromRow == nToRow)
    {
        szAlphaFmt[2] = '.';
        szAlphaFmt[3] = nToColumn + 1 + '0';
    }
    else
    {
        szAlphaFmt[2] = nFromRow > nToRow ? '-' : '+';
        szAlphaFmt[3] = abs(nFromRow - nToRow) + '0';
    }
}

//������ͬ
void CMoveRouteGenerator::GetRedChariotCannonMoveStepAlpha( int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char szAlphaFmt[], int nChessMan )
{
    //��ȡ����λ�ã��������������ͬһ����Ҫ��ǰ��
    ChessManPosition arrPos[2];
    GetChessManPosition(arrChessMan, nChessMan, arrPos);
    //�����������ͬһ����
    if (arrPos[0].nColumn == arrPos[1].nColumn)
    {
        if (arrPos[0].nRow > arrPos[1].nRow)
        {
            if (nFromRow == arrPos[0].nRow)
            {
                szAlphaFmt[1] = 'b';
            }
            else
            {
                szAlphaFmt[1] = 'a';
            }
        }
        else
        {
            if (nFromRow == arrPos[0].nRow)
            {
                szAlphaFmt[1] = 'a';
            }
            else
            {
                szAlphaFmt[1] = 'b';
            }
        }
    }
    else
    {
        szAlphaFmt[1] = CHESSBOARD_COLUMN - nFromColumn + '0';
    }

    if (nFromRow == nToRow)
    {
        szAlphaFmt[2] = '.';
        szAlphaFmt[3] = CHESSBOARD_COLUMN - nToColumn + '0';
    }
    else
    {
        szAlphaFmt[2] = nFromRow > nToRow ? '+' : '-';
        szAlphaFmt[3] = abs(nFromRow - nToRow) + '0';
    }
}

void CMoveRouteGenerator::GetBlackHorseMoveStepAlpha( int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char szAlphaFmt[] )
{
    //��ȡ���λ�ã��������������ͬһ����Ҫ��ǰ��
    ChessManPosition arrPos[2];
    GetChessManPosition(arrChessMan, BLACK_HORSE, arrPos);
    //�����������ͬһ����
    if (arrPos[0].nColumn == arrPos[1].nColumn)
    {
        if (arrPos[0].nRow > arrPos[1].nRow)
        {
            if (nFromRow == arrPos[0].nRow)
            {
                szAlphaFmt[1] = 'a';
            }
            else
            {
                szAlphaFmt[1] = 'b';
            }
        }
        else
        {
            if (nFromRow == arrPos[0].nRow)
            {
                szAlphaFmt[1] = 'b';
            }
            else
            {
                szAlphaFmt[1] = 'a';
            }
        }
    }
    else
    {
        szAlphaFmt[1] = nFromColumn + 1 + '0';
    }

    szAlphaFmt[2] = nFromRow > nToRow ? '-' : '+';
    szAlphaFmt[3] = nToColumn + 1 + '0';
}

void CMoveRouteGenerator::GetRedHorseMoveStepAlpha( int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char szAlphaFmt[] )
{
    //��ȡ���λ�ã��������������ͬһ����Ҫ��ǰ��
    ChessManPosition arrPos[2];
    GetChessManPosition(arrChessMan, RED_HORSE, arrPos);
    //�����������ͬһ����
    if (arrPos[0].nColumn == arrPos[1].nColumn)
    {
        if (arrPos[0].nRow > arrPos[1].nRow)
        {
            if (nFromRow == arrPos[0].nRow)
            {
                szAlphaFmt[1] = 'b';
            }
            else
            {
                szAlphaFmt[1] = 'a';
            }
        }
        else
        {
            if (nFromRow == arrPos[0].nRow)
            {
                szAlphaFmt[1] = 'a';
            }
            else
            {
                szAlphaFmt[1] = 'b';
            }
        }
    }
    else
    {
        szAlphaFmt[1] = CHESSBOARD_COLUMN - nFromColumn + '0';
    }

    szAlphaFmt[2] = nFromRow > nToRow ? '+' : '-';
    szAlphaFmt[3] = CHESSBOARD_COLUMN - nToColumn + '0';
}


void CMoveRouteGenerator::GetBlackSoldierMoveStepAlpha( int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char szAlphaFmt[] )
{
    szAlphaFmt[1] = nFromColumn + 1 + '0';
    //��ȡ����λ��
    ChessManPosition arrPos[5];
    int SoldierCount = GetChessManPosition(arrChessMan, BLACK_SOLDIER, arrPos);

    int nMark = 0;
    for (int i = 0; i < SoldierCount; i++)
    {
        if (arrPos[i].nColumn == nFromColumn)
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
            szSoldierCountOnEachColumn[arrPos[i].nColumn]++;
        }

        for (int i = 0; i < CHESSBOARD_COLUMN; i++)
        {
            if (szSoldierCountOnEachColumn[i] > 1)
            {
                //���������������ϵı�
                for (int j = 0; j < SoldierCount; j++)
                {
                    if (i == arrPos[j].nColumn)
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
                        if (arrPos[szColumnForEachSoldier[j]].nRow < arrPos[szColumnForEachSoldier[j + 1]].nRow)
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
            if (arrPos[szColumnForEachSoldier[i]].nRow == nToRow)
            {
                szAlphaFmt[1] = chOrder;
            }
            chOrder++;
        }

        //ת����ǰ���ǰ�к�
        if (nStart == 2)
        {
            szAlphaFmt[1] = szAlphaFmt[1] == 'c' ? 'q' : 'h';
        }
        else if (nStart == 3)
        {
            szAlphaFmt[1] = szAlphaFmt[1] == 'c' ? 'q' : (szAlphaFmt[1] == 'd' ? 'z' : 'h');
        }
    }

    if (nFromRow == nToRow)
    {
        szAlphaFmt[2] = '.';
        szAlphaFmt[3] = nToColumn + 1 + '0';
    }
    else
    {
        szAlphaFmt[2] = nFromRow > nToRow ? '-' : '+';
        szAlphaFmt[3] = abs(nFromRow - nToRow) + '0';
    }

}

void CMoveRouteGenerator::GetRedSoldierMoveStepAlpha( int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char szAlphaFmt[] )
{
    szAlphaFmt[1] = CHESSBOARD_COLUMN - nFromColumn + '0';

    //��ȡ����λ��
    ChessManPosition arrPos[5];
    int SoldierCount = GetChessManPosition(arrChessMan, RED_SOLDIER, arrPos);

    int nMark = 0;
    for (int i = 0; i < SoldierCount; i++)
    {
        if (arrPos[i].nColumn == nFromColumn)
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
            szSoldierCountOnEachColumn[arrPos[i].nColumn]++;
        }

        for (int i = CHESSBOARD_COLUMN - 1; i >= 0; i--)
        {
            if (szSoldierCountOnEachColumn[i] > 1)
            {
                //���������������ϵı�
                for (int j = 0; j < SoldierCount; j++)
                {
                    if (i == arrPos[j].nColumn)
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
                        if (arrPos[szColumnForEachSoldier[j]].nRow > arrPos[szColumnForEachSoldier[j + 1]].nRow)
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
            if (arrPos[szColumnForEachSoldier[i]].nRow == nToRow)
            {
                szAlphaFmt[1] = chOrder;
            }
            chOrder++;
        }

        //ת����ǰ���ǰ�к�
        if (nStart == 2)
        {
            szAlphaFmt[1] = szAlphaFmt[1] == 'c' ? 'q' : 'h';
        }
        else if (nStart == 3)
        {
            szAlphaFmt[1] = szAlphaFmt[1] == 'c' ? 'q' : (szAlphaFmt[1] == 'd' ? 'z' : 'h');
        }
    }

    if (nFromRow == nToRow)
    {
        szAlphaFmt[2] = '.';
        szAlphaFmt[3] = CHESSBOARD_COLUMN - nToColumn + '0';
    }
    else
    {
        szAlphaFmt[2] = nFromRow > nToRow ? '+' : '-';
        szAlphaFmt[3] = abs(nFromRow - nToRow) + '0';
    }
}

void CMoveRouteGenerator::AlphaFmtToChiness(const char *szAlphaFmt, char szChinesFmt[], bool bBlack )
{
    string strDst;
    if (szAlphaFmt[1] >= 'a')
    {
        strDst = DigitToChinese(szAlphaFmt[1], bBlack) + 
                 ChessManCodetoChinese(szAlphaFmt[0], bBlack) + 
                 ActionToChinese(szAlphaFmt[2]) + 
                 DigitToChinese(szAlphaFmt[3], bBlack);
    }
    else
    {
        strDst = ChessManCodetoChinese(szAlphaFmt[0], bBlack) +
                 DigitToChinese(szAlphaFmt[1], bBlack) + 
                 ActionToChinese(szAlphaFmt[2]) + 
                 DigitToChinese(szAlphaFmt[3], bBlack);
    }

    strcpy(szChinesFmt, strDst.c_str());
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
void CMoveRouteGenerator::GenerateAllMoveRoute( int arrChessMan[][CHESSBOARD_COLUMN], int nSide, 
                                            list<MoveRoute> &lstMoveRoute )
{
    for (int i = 0; i < CHESSBOARD_ROW; i++)
    {
        for (int j = 0; j < CHESSBOARD_COLUMN; j++)
        {
            if ((nSide == BLACK && IsBlackSide(arrChessMan[i][j])) 
                || (nSide == RED && IsRedSide(arrChessMan[i][j])))
            {
                GenerateMoveRoute(arrChessMan, i, j, lstMoveRoute);
            }
        }
    }
}

void CMoveRouteGenerator::GenerateMoveRoute( int arrChessMan[][CHESSBOARD_COLUMN], int nRow, int nColumn, list<MoveRoute> &lstMoveRoute )
{
    int nKilledChessMan = 0;
    int nMovingChessMan = arrChessMan[nRow][nColumn];
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
            if (ValidateMoveRoute(arrChessMan, nRow, nColumn, i, j))
            {
                nKilledChessMan = arrChessMan[i][j];

                //���ж�������Լ��Ƿ񱻶Է�����������Լ����Է����������߷�������
                arrChessMan[nRow][nColumn] = 0;
                arrChessMan[i][j] = nMovingChessMan;

                if (!IsAttackGeneral(arrChessMan, nGeneral))
                {
                    stMoveRoute.bAttackGeneral = IsAttackGeneral(arrChessMan, nOppGeneral);
                    stMoveRoute.stToPos.nRow = i;
                    stMoveRoute.stToPos.nColumn = j;
                    stMoveRoute.nKilledChessMan = nKilledChessMan;
                    lstMoveRoute.push_back(stMoveRoute);
                }

                arrChessMan[nRow][nColumn] = nMovingChessMan;
                arrChessMan[i][j] = nKilledChessMan;
            }
        }
    }
}

//�����������ж�
bool CMoveRouteGenerator::IsGeneralDead( int arrChessMan[][CHESSBOARD_COLUMN], int nSide)
{
    bool bDead = true;
    int nGeneral = nSide == BLACK ? BLACK_GENERAL : RED_GENERAL;
    
    //�ж��ܷ��������ľ���
    for (int i = 0; i < CHESSBOARD_ROW; i++)
    {
        for (int j = 0; j < CHESSBOARD_COLUMN; j++)
        {
            if ((nSide == BLACK && IsBlackSide(arrChessMan[i][j])) 
                || (nSide == RED && IsRedSide(arrChessMan[i][j])))
            {
                if(CanSaveGeneral(arrChessMan, i, j, nGeneral))
                {
                    bDead = false;
                    break;
                }
            }
        }
    }

    return bDead;
}

bool CMoveRouteGenerator::CanSaveGeneral( int arrChessMan[][CHESSBOARD_COLUMN], int nRow, int nColumn, int nGeneral )
{
    bool bSave = false;
    int nKilledChessMan = 0;
    int nMovingChessMan = arrChessMan[nRow][nColumn];

    for (int i = 0; i < CHESSBOARD_ROW; i++)
    {
        for (int j = 0; j < CHESSBOARD_COLUMN; j++)
        {
            if (ValidateMoveRoute(arrChessMan, nRow, nColumn, i, j))
            {
                nKilledChessMan = arrChessMan[i][j];

                //���ж�������Լ��Ƿ񱻶Է�����������Լ����Է����������߷�������
                arrChessMan[nRow][nColumn] = 0;
                arrChessMan[i][j] = nMovingChessMan;

                if (!IsAttackGeneral(arrChessMan, nGeneral))
                {
                    bSave = true;
                }

                arrChessMan[nRow][nColumn] = nMovingChessMan;
                arrChessMan[i][j] = nKilledChessMan;

                if (bSave)
                {
                    break;
                }
            }
        }
    }

    return bSave;
}
