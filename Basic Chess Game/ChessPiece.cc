#include "ChessPiece.hh"

using Player::ChessPiece;

// Constructor
ChessPiece::ChessPiece(ChessBoard &board, Color color, int row, int column) : board(board), color(color), row(row), column(column) {}

// Get color of the chess piece
Color ChessPiece::getColor() {
    return color;
}

// Get current row of the piece
int ChessPiece::getRow() {
    return row;
}

// Get current column of the piece
int ChessPiece::getColumn() {
    return column;
}

// Set the position of the piece
void ChessPiece::setPosition(int row, int column) {
    this->row = row;
    this->column = column;
}

int ChessPiece::abs(int value) {
    return (value < 0) ? -value : value;
}


