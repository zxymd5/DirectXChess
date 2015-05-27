/*      
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  File:       MoveRouteGenerator.h
 *  Author:     Richard Zou
 *  Created on: 2015-03-01
 */

#ifndef MOVE_ROUTE_GENERATOR_H
#define MOVE_ROUTE_GENERATOR_H

#include "CommDef.h"
#include <string>
#include <list>

using namespace std;

class CMoveRouteGenerator
{
public:
    CMoveRouteGenerator(void);
    ~CMoveRouteGenerator(void);
    bool ValidateMoveRoute(int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, 
                        int nFromColumn, int nToRow, int nToColumn);
    bool ValidateBlackGeneralMoveRoute(int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, 
                                        int nFromColumn, int nToRow, int nToColumn);
    bool ValidateRedGeneralMoveRoute(int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, 
                                    int nFromColumn, int nToRow, int nToColumn);
    bool ValidateBlackAdvisorMoveRoute(int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, 
                                        int nFromColumn, int nToRow, int nToColumn);
    bool ValidateRedAdvisorMoveRoute(int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, 
                                    int nFromColumn, int nToRow, int nToColumn);
    bool ValidateBlackMinisterMoveRoute(int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, 
                                        int nFromColumn, int nToRow, int nToColumn);
    bool ValidateRedMinisterMoveRoute(int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, 
                                        int nFromColumn, int nToRow, int nToColumn);
    bool ValidateBlackSoldierMoveRoute(int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, 
                                        int nFromColumn, int nToRow, int nToColumn);
    bool ValidateRedSoldierMoveRoute(int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, 
                                    int nFromColumn, int nToRow, int nToColumn);
    bool ValidateChariotMoveRoute(int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, 
                                int nFromColumn, int nToRow, int nToColumn);
    bool ValidateHorseMoveRoute(int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, 
                                int nFromColumn, int nToRow, int nToColumn);
    bool ValidateCannonMoveRoute(int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, 
                                int nFromColumn, int nToRow, int nToColumn);
    bool IsAttackGeneral( int arrChessMan[][CHESSBOARD_COLUMN], int nGeneral); //是否将军
    bool IsAttackGeneral( int arrChessMan[][CHESSBOARD_COLUMN], int nRow, int nColumn, int nGeneral); //是否将军
    bool AttackGeneral(int arrChessMan[][CHESSBOARD_COLUMN], int nRow, int nColumn, int nSpecChessMan);

    int GetChessManPosition(int arrChessMan[][CHESSBOARD_COLUMN], int nSpecChessMan, ChessManPosition arrPos[] );
    void GetGeneralPosition(int arrChessMan[][CHESSBOARD_COLUMN], int nGeneral, int &nRow, int &nColumn);

    //获取着法的数字坐标格式，如k5+1
    void GetChessManMoveStepAlpha(int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char szAlphaFmt[]);
    void GetBlackGeneralMoveStepAlpha(int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char szAlphaFmt[]);
    void GetBlackAdvisorMinisterMoveStepAlpha(int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char szAlphaFmt[]);
    void GetBlackChariotCannonMoveStepAlpha(int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char szAlphaFmt[], int nChessMan);
    void GetBlackHorseMoveStepAlpha(int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char szAlphaFmt[]);
    void GetBlackSoldierMoveStepAlpha(int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char szAlphaFmt[]);
    void GetRedGeneralMoveStepAlpha(int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char szAlphaFmt[]);
    void GetRedAdvisorMinisterMoveStepAlpha(int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char szAlphaFmt[]);
    void GetRedChariotCannonMoveStepAlpha(int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char szAlphaFmt[], int nChessMan);
    void GetRedHorseMoveStepAlpha(int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char szAlphaFmt[]);
    void GetRedSoldierMoveStepAlpha(int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char szAlphaFmt[]);

    //将数字坐标格式转换成中文格式如将5进1
    static string DigitToChinese(char chDigit, bool bBlack);
    static string ChessManCodetoChinese(char chCode, bool bBlack);
    static string ActionToChinese(char chAction);
    static void AlphaFmtToChiness(const char *pAlphaFmt, char strChineseFmt[], bool bBlack);

    void GenerateAllMoveRoute(int arrChessMan[][CHESSBOARD_COLUMN], int nSide, list<MoveRoute> &lstMoveRoute);
    bool CanSaveGeneral(int arrChessMan[][CHESSBOARD_COLUMN], int nRow, int nColumn, int nGeneral);
    bool IsGeneralDead(int arrChessMan[][CHESSBOARD_COLUMN], int nSide);

    void GenerateGeneralMoveRoute( int arrChessMan[][CHESSBOARD_COLUMN], int nRow, int nColumn, list<MoveRoute> &lstMoveRoute, MoveRoute &stMoveRoute );
    void GenerateChariotCannonMoveRoute( int arrChessMan[][CHESSBOARD_COLUMN], int nRow, int nColumn, list<MoveRoute> &lstMoveRoute, MoveRoute &stMoveRoute  );
    void GenerateHorseMoveRoute( int arrChessMan[][CHESSBOARD_COLUMN], int nRow, int nColumn, list<MoveRoute> &lstMoveRoute, MoveRoute &stMoveRoute  );
    void GenerateAdvisorMoveRoute( int arrChessMan[][CHESSBOARD_COLUMN], int nRow, int nColumn, list<MoveRoute> &lstMoveRoute, MoveRoute &stMoveRoute  );
    void GenerateMinisterMoveRoute( int arrChessMan[][CHESSBOARD_COLUMN], int nRow, int nColumn, list<MoveRoute> &lstMoveRoute, MoveRoute &stMoveRoute  );
    void GenerateSoldierMoveRoute( int arrChessMan[][CHESSBOARD_COLUMN], int nRow, int nColumn, list<MoveRoute> &lstMoveRoute, MoveRoute &stMoveRoute  );

    void AddMoveRoute(  int arrChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, list<MoveRoute> &lstMoveRoute, MoveRoute &stMoveRoute );

};

#endif
