#include "KingPiece.hh"
#include "ChessBoard.hh"

using Player::KingPiece;

KingPiece::KingPiece(ChessBoard &board, Color color, int row, int column) : ChessPiece(board, color, row, column) {}

bool KingPiece::canMoveToLocation(int toRow, int toColumn) {
    int x = abs(toRow - getRow());
    int y = abs(toColumn - getColumn());

    if ((x <= 1 && y <= 1) && !(x == 0 && y == 0)) {
        ChessPiece *destination = board.getPiece(toRow, toColumn);
        int originalRow = getRow();
        int originalColumn = getColumn();
        // simulate moving king to destination position
        board.setPiece(this, toRow, toColumn);             
        board.setPiece(nullptr, originalRow, originalColumn); 
        setPosition(toRow, toColumn);
        // find out if King would be under Check at destination position
        bool underCheck = board.isPieceUnderThreat(toRow, toColumn);
        // revert/reset simulation to original state
        board.setPiece(this, originalRow, originalColumn);    
        board.setPiece(destination, toRow, toColumn);        
        setPosition(originalRow, originalColumn);
        if (underCheck) {
            return false;
        }
        if(destination == nullptr || destination->getColor() != getColor()) {
            return true;
        }
    }
    return false;
}

const char* KingPiece::toString() {
    return (getColor() == White) ? "\u2654" : "\u265A";
}

Type KingPiece::getType() {
    return Type::King;
}