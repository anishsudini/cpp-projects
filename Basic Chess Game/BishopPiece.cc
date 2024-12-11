#include "BishopPiece.hh"
#include "ChessBoard.hh"

using Player::BishopPiece;

BishopPiece::BishopPiece(ChessBoard &board, Color color, int row, int column) : ChessPiece(board, color, row, column) {}

bool BishopPiece::canMoveToLocation(int toRow, int toColumn) {
    if (abs(toRow - getRow()) == abs(toColumn - getColumn())) {
        int x = (toRow > getRow()) ? 1 : -1;
        int y = (toColumn > getColumn()) ? 1 : -1;
        int nextx = getRow() + x;
        int nexty = getColumn() + y;
        while(nextx != toRow || nexty != toColumn) {
            if(board.getPiece(nextx, nexty) != nullptr) {
                return false;
            }
            nextx += x;
            nexty += y;
        }
        ChessPiece *destination = board.getPiece(toRow, toColumn);
        if (destination == nullptr || destination->getColor() != getColor()) {
            return true;
        }
    }
    return false;
}

const char *BishopPiece::toString() {
    return (getColor() == White) ? "\u2657" : "\u265D";
}

Type BishopPiece::getType() {
    return Type::Bishop;
}