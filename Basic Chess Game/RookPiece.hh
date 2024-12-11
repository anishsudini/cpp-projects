#ifndef _ROOKPIECE_H__
#define _ROOKPIECE_H__

#include "ChessPiece.hh"

namespace Player
{
    class RookPiece : public ChessPiece
    {
        public:
            RookPiece(ChessBoard &board, Color color, int row, int column);
            virtual ~RookPiece() {}
            bool canMoveToLocation(int toRow, int toColumn) override;
            const char *toString() override;
            Type getType() override;
    };
}

#endif
