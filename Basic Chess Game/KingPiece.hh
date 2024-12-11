#ifndef __KINGPIECE_H__
#define __KINGPIECE_H__

#include "ChessPiece.hh"

namespace Player
{
    class KingPiece : public ChessPiece
    {
        public:
            KingPiece(ChessBoard &board, Color color, int row, int column);
            ~KingPiece() {}
            bool canMoveToLocation(int toRow, int toColumn);
            const char *toString();
            Type getType();
    };
}

#endif
