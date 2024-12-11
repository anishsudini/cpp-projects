#include "ChessBoard.hh"
#include "PawnPiece.hh"
#include "RookPiece.hh"
#include "BishopPiece.hh"
#include "KingPiece.hh"

using Player::ChessBoard;

std::ostringstream ChessBoard::displayBoard()
{
    std::ostringstream outputString;
    // top scale
    outputString << "  ";
    for (int i = 0; i < numCols; i++)
    {
        outputString << i;
    }
    outputString << std::endl
                 << "  ";
    // top border
    for (int i = 0; i < numCols; i++)
    {
        outputString << "-";
    }
    outputString << std::endl;

    for (int row = 0; row < numRows; row++)
    {
        outputString << row << "|";
        for (int column = 0; column < numCols; column++)
        {
            ChessPiece *piece = board.at(row).at(column);
            outputString << (piece == nullptr ? " " : piece->toString());
        }
        outputString << "|" << std::endl;
    }

    // bottom border
    outputString << "  ";
    for (int i = 0; i < numCols; i++)
    {
        outputString << "-";
    }
    outputString << std::endl
                 << std::endl;

    return outputString;
}

ChessBoard::ChessBoard(int numRow, int numCol) : numRows(numRow), numCols(numCol) {
    board = std::vector<std::vector<ChessPiece *>>(numRows, std::vector<ChessPiece *>(numCols, nullptr));
}

ChessBoard::~ChessBoard() {
    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            if (board[row][col] != nullptr) {
                delete board[row][col];
                board[row][col] = nullptr;
            }
        }
    }
}

void ChessBoard::createChessPiece(Color col, Type ty, int startRow, int startColumn)
{
    if (board.at(startRow).at(startColumn) != nullptr) {
        delete board.at(startRow).at(startColumn);
        board.at(startRow).at(startColumn) = nullptr;
    }

    ChessPiece *newPiece = nullptr;
    switch(ty) {
        case Type::Pawn:
            newPiece = new PawnPiece(*this, col, startRow, startColumn);
            break;
        case Type::Bishop:
            newPiece = new BishopPiece(*this, col, startRow, startColumn);
            break;
        case Type::Rook:
            newPiece = new RookPiece(*this, col, startRow, startColumn);
            break;
        case Type::King:
            newPiece = new KingPiece(*this, col, startRow, startColumn);
            break;
    }

    if (newPiece != nullptr) {
        board.at(startRow).at(startColumn) = newPiece;
    }

}

bool ChessBoard::isValidMove(int fromRow, int fromColumn, int toRow, int toColumn)
{
    if (fromRow < 0 || fromRow >= numRows || fromColumn < 0 || fromColumn >= numCols || toRow < 0 || toRow >= numRows || toColumn < 0 || toColumn >= numCols) {
        return false; 
    }
    ChessPiece *curr = getPiece(fromRow, fromColumn);
    if (curr == nullptr) {
        return false;
    }
    if (fromRow == toRow && fromColumn == toColumn) {
        return false;
    }
    ChessPiece *destination = getPiece(toRow, toColumn);
    if (!curr->canMoveToLocation(toRow, toColumn)) {
        return false;
    }
    setPiece(curr, toRow, toColumn);
    setPiece(nullptr, fromRow, fromColumn);
    bool validMove = true;
    for (int i = 0; i < numRows && validMove; ++i) {
        for (int j = 0; j < numCols && validMove; ++j) {
            KingPiece *king = dynamic_cast<KingPiece*>(getPiece(i, j));
            if (king != nullptr && king->getColor() == curr->getColor()) {
                if (isPieceUnderThreat(i, j)) {
                    validMove = false;
                }
                break;
            }
        }
    }
    setPiece(curr, fromRow, fromColumn);
    setPiece(destination, toRow, toColumn);
    return validMove;
}


bool ChessBoard::movePiece(int fromRow, int fromColumn, int toRow, int toColumn) {
    //check if valid move
    if(!isValidMove(fromRow, fromColumn, toRow, toColumn)) {
        return false;
    }
    //check turn
    ChessPiece *curr = getPiece(fromRow, fromColumn);
    if (curr == nullptr || curr->getColor() != turn) {
        return false;
    }
    //check if move is capturing another piece, if so delete it
    ChessPiece *destination = getPiece(toRow, toColumn);
    if (destination != nullptr) {
        delete destination; 
        board[toRow][toColumn] = nullptr;
    }
    // Move the piece
    board[toRow][toColumn] = curr;
    board[fromRow][fromColumn] = nullptr;
    curr->setPosition(toRow, toColumn);
    // Switch turns
    turn = (turn == White) ? Black : White;
    return true;
}

bool ChessBoard::isPieceUnderThreat(int row, int column) {
    ChessPiece *curr = getPiece(row, column);
    if (curr == nullptr) {
        return false;
    }

    Color enemyColor = (curr->getColor() == White) ? Black : White;

    for (int i = 0; i < numRows; ++i) {
        for (int j = 0; j < numCols; ++j) {
            ChessPiece *enemyPiece = getPiece(i, j);
            if (enemyPiece != nullptr && enemyPiece->getColor() == enemyColor) {
                KingPiece *enemyKing = dynamic_cast<KingPiece*>(enemyPiece);
                if (enemyKing != nullptr) {
                    int kingRow = enemyKing->getRow();
                    int kingCol = enemyKing->getColumn();
                    if (ChessPiece::abs(kingRow - row) <= 1 && ChessPiece::abs(kingCol - column) <= 1) {
                        return true;
                    }
                } else {
                    if (enemyPiece->canMoveToLocation(row, column)) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void ChessBoard::setPiece(ChessPiece* piece, int row, int column) {
    if (row >= 0 && row < numRows && column >= 0 && column < numCols) {
        board[row][column] = piece;
    } else {
        throw std::out_of_range("Position is out of board bounds.");
    }
}
