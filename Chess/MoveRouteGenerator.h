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
 *  File:		MoveRouteGenerator.h
 *  Author:		Richard Zou
 *	Created on:	2015-03-01
 */

#ifndef MOVE_ROUTE_GENERATOR_H
#define MOVE_ROUTE_GENERATOR_H

#include "CommDef.h"
#include <string>

using namespace std;

class CMoveRouteGenerator
{
public:
    CMoveRouteGenerator(void);
    ~CMoveRouteGenerator(void);
    bool ValidateMoveRoute(int szChessMan[][s_nChessBoardColumn], int nFromRow, 
                        int nFromColumn, int nToRow, int nToColumn);
    bool ValidateBlackGeneralMoveRoute(int szChessMan[][s_nChessBoardColumn], int nFromRow, 
                                        int nFromColumn, int nToRow, int nToColumn);
    bool ValidateRedGeneralMoveRoute(int szChessMan[][s_nChessBoardColumn], int nFromRow, 
                                    int nFromColumn, int nToRow, int nToColumn);
    bool ValidateBlackAdvisorMoveRoute(int szChessMan[][s_nChessBoardColumn], int nFromRow, 
                                        int nFromColumn, int nToRow, int nToColumn);
    bool ValidateRedAdvisorMoveRoute(int szChessMan[][s_nChessBoardColumn], int nFromRow, 
                                    int nFromColumn, int nToRow, int nToColumn);
    bool ValidateBlackMinisterMoveRoute(int szChessMan[][s_nChessBoardColumn], int nFromRow, 
                                        int nFromColumn, int nToRow, int nToColumn);
    bool ValidateRedMinisterMoveRoute(int szChessMan[][s_nChessBoardColumn], int nFromRow, 
                                        int nFromColumn, int nToRow, int nToColumn);
    bool ValidateBlackSoldierMoveRoute(int szChessMan[][s_nChessBoardColumn], int nFromRow, 
                                        int nFromColumn, int nToRow, int nToColumn);
    bool ValidateRedSoldierMoveRoute(int szChessMan[][s_nChessBoardColumn], int nFromRow, 
                                    int nFromColumn, int nToRow, int nToColumn);
    bool ValidateChariotMoveRoute(int szChessMan[][s_nChessBoardColumn], int nFromRow, 
                                int nFromColumn, int nToRow, int nToColumn);
    bool ValidateHorseMoveRoute(int szChessMan[][s_nChessBoardColumn], int nFromRow, 
                                int nFromColumn, int nToRow, int nToColumn);
    bool ValidateCannonMoveRoute(int szChessMan[][s_nChessBoardColumn], int nFromRow, 
                                int nFromColumn, int nToRow, int nToColumn);
    bool IsAttackGeneral( int szChessMan[][s_nChessBoardColumn], int nGeneral); //是否将军
    bool AttackGeneral(int szChessMan[][s_nChessBoardColumn], int nRow, int nColumn, int nSpecChessMan);
    bool MeetWithGeneral( int szChessMan[][s_nChessBoardColumn], int nGeneral);   //是否将帅相见

    int GetChessManPosition(int szChessMan[][s_nChessBoardColumn], int nSpecChessMan, ChessManPosition szPos[] );

    //获取着法的数字坐标格式，如k5+1
    void GetChessManMoveStepAlpha(int szChessMan[][s_nChessBoardColumn], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char strAlphaFmt[]);
    void GetBlackGeneralMoveStepAlpha(int szChessMan[][s_nChessBoardColumn], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char strAlphaFmt[]);
    void GetBlackAdvisorMinisterMoveStepAlpha(int szChessMan[][s_nChessBoardColumn], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char strAlphaFmt[]);
    void GetBlackChariotCannonMoveStepAlpha(int szChessMan[][s_nChessBoardColumn], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char strAlphaFmt[], int nChessMan);
    void GetBlackHorseMoveStepAlpha(int szChessMan[][s_nChessBoardColumn], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char strAlphaFmt[]);
    void GetBlackSoldierMoveStepAlpha(int szChessMan[][s_nChessBoardColumn], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char strAlphaFmt[]);
    void GetRedGeneralMoveStepAlpha(int szChessMan[][s_nChessBoardColumn], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char strAlphaFmt[]);
    void GetRedAdvisorMinisterMoveStepAlpha(int szChessMan[][s_nChessBoardColumn], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char strAlphaFmt[]);
    void GetRedChariotCannonMoveStepAlpha(int szChessMan[][s_nChessBoardColumn], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char strAlphaFmt[], int nChessMan);
    void GetRedHorseMoveStepAlpha(int szChessMan[][s_nChessBoardColumn], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char strAlphaFmt[]);
    void GetRedSoldierMoveStepAlpha(int szChessMan[][s_nChessBoardColumn], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char strAlphaFmt[]);

    //将数字坐标格式转换成中文格式如将5进1
    static string DigitToChinese(char chDigit, bool bBlack);
    static string ChessManCodetoChinese(char chCode, bool bBlack);
    static string ActionToChinese(char chAction);
    static void AlphaFmtToChiness(const char *strAlphaFmt, char strChineseFmt[], bool bBlack);
    
};

#endif
