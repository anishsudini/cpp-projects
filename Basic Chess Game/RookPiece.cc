#include "RookPiece.hh"
#include "ChessBoard.hh"

using Player::RookPiece;

RookPiece::RookPiece(ChessBoard &board, Color color, int row, int column) : ChessPiece(board, color, row, column) {}

bool RookPiece::canMoveToLocation(int toRow, int toColumn) {
    if (toRow != getRow() && toColumn != getColumn()) {
        return false;  
    }

    int x = (toRow > getRow()) ? 1 : (toRow < getRow() ? -1 : 0);
    int y = (toColumn > getColumn()) ? 1 : (toColumn < getColumn() ? -1 : 0);

    int nextx = getRow() + x;
    int nexty = getColumn() + y;

    while (nextx != toRow || nexty != toColumn) {
        if (board.getPiece(nextx, nexty) != nullptr) {
            return false; 
        }
        nextx += x;
        nexty += y;
    }

    ChessPiece *destination = board.getPiece(toRow, toColumn);
    if (destination == nullptr || destination->getColor() != getColor()) {
        return true; 
    }

    return false;
}

const char *RookPiece::toString() {
    return (getColor() == White) ? "\u2656" : "\u265C";
}

Type RookPiece::getType() {
    return Type::Rook;
}