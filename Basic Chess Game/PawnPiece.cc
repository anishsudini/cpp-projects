#include "PawnPiece.hh"
#include "ChessBoard.hh"

using Player::PawnPiece;

PawnPiece::PawnPiece(ChessBoard &board, Color color, int row, int column) : ChessPiece(board, color, row, column) {}

bool PawnPiece::canMoveToLocation(int toRow, int toColumn) {
    int y = (getColor() == White) ? -1 : 1;

    if (toColumn == getColumn() && toRow == getRow() + y) {
        if (board.getPiece(toRow, toColumn) == nullptr) {
            return true;
        }
    }

    if(toColumn == getColumn() && ((getRow() == 1 && getColor() == Black) || (getRow() == board.getNumRows() - 2 && getColor() == White))) {
        if(toRow == getRow() + 2 * y) {
            if(board.getPiece(toRow, toColumn) == nullptr && board.getPiece(getRow() + y, toColumn) == nullptr) {
                return true;
            }
        }
    }
    
    if((toColumn == getColumn() + 1 || toColumn == getColumn() - 1) && toRow == getRow() + y) {
        ChessPiece *destination = board.getPiece(toRow, toColumn);
        if(destination != nullptr && destination->getColor() != getColor()) {
            return true;
        }
    }
    return false;
}

const char *PawnPiece::toString() {
    return (getColor() == White) ? "\u2659" : "\u265F";
}

Type PawnPiece::getType() {
    return Type::Pawn;
}