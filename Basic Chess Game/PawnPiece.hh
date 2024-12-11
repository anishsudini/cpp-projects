#ifndef __PAWNPIECE_H__
#define __PAWNPIECE_H__

#include "ChessPiece.hh"

namespace Player
{
    class PawnPiece : public ChessPiece
    {
        public: 
            PawnPiece(ChessBoard &board, Color color, int row, int column);
            ~PawnPiece() {}
            bool canMoveToLocation(int toRow, int toColumn);
            const char *toString();
            Type getType();
    };
}

#endif
