#ifndef MOVE_ROUTE_GENERATOR_H
#define MOVE_ROUTE_GENERATOR_H

#include "CommDef.h"

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

};

#endif
