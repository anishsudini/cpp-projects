#include <assert.h>
#include "Chess.h"
#include "ChessBoard.hh"
#include "ChessPiece.hh"
#include "KingPiece.hh"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

void runTestSequence(Player::ChessBoard &sBoard, const std::string &sequence, std::ofstream &logFile) {
    std::istringstream ss(sequence);
    std::string command;

    while (ss >> command) {
        if (command == "movePiece") {
            int fromRow, fromCol, toRow, toCol;
            ss >> fromRow >> fromCol >> toRow >> toCol;
            logFile << "\nAttempting to move piece from (" << fromRow << ", " << fromCol << ") to (" << toRow << ", " << toCol << "):\n";
            if (sBoard.movePiece(fromRow, fromCol, toRow, toCol)) {
                logFile << "Move successful\n";
            } else {
                logFile << "Move failed\n";
            }
            logFile << sBoard.displayBoard().str() << std::endl;
        } else if (command == "isValidScan") {
            logFile << "\nRunning isValidScan for all pieces:\n";
            for (int fromRow = 0; fromRow < sBoard.getNumRows(); ++fromRow) {
                for (int fromCol = 0; fromCol < sBoard.getNumCols(); ++fromCol) {
                    Player::ChessPiece *piece = sBoard.getPiece(fromRow, fromCol);
                    if (piece == nullptr) {
                        continue;
                    }
                    logFile << "Testing moves for piece at (" << fromRow << ", " << fromCol << "): " << piece->toString() << "\n";
                    for (int toRow = 0; toRow < sBoard.getNumRows(); ++toRow) {
                        for (int toCol = 0; toCol < sBoard.getNumCols(); ++toCol) {
                            bool valid = sBoard.isValidMove(fromRow, fromCol, toRow, toCol);
                            logFile << "Move from (" << fromRow << ", " << fromCol << ") to (" << toRow << ", " << toCol << ") is "
                                      << (valid ? "valid" : "invalid") << "\n";
                        }
                    }
                }
            }
        } else if (command == "underThreatScan") {
            logFile << "\nRunning underThreatScan for all pieces:\n";
            for (int row = 0; row < sBoard.getNumRows(); ++row) {
                for (int col = 0; col < sBoard.getNumCols(); ++col) {
                    Player::ChessPiece *piece = sBoard.getPiece(row, col);
                    if (piece != nullptr) {
                        bool underThreat = sBoard.isPieceUnderThreat(row, col);
                        logFile << "Piece at (" << row << ", " << col << "): " << piece->toString()
                                  << " is " << (underThreat ? "under threat" : "not under threat") << "\n";
                    }
                }
            }
        }
    }
}

int main() {
    Player::ChessBoard sBoard(4, 4);
    sBoard.createChessPiece(Black, Bishop, 0, 3);
    sBoard.createChessPiece(Black, Rook, 0, 3);
    sBoard.createChessPiece(White, Pawn, 2, 3);
    sBoard.createChessPiece(Black, Pawn, 3, 0);
    sBoard.createChessPiece(Black, King, 1, 2);
    sBoard.createChessPiece(White, King, 1, 0);

    std::ofstream logFile("chess_log1.txt");
    if (!logFile.is_open()) {
        std::cerr << "Failed to open log file." << std::endl;
        return EXIT_FAILURE;
    }

    logFile << "Initial Board Setup:\n";
    logFile << sBoard.displayBoard().str() << std::endl;

    // The sequence provided to be executed
    std::string testSequence = R"(
        isValidScan
        underThreatScan
        movePiece 2 3 1 3
        isValidScan
        movePiece 0 3 0 2
        isValidScan
        movePiece 1 0 2 0
        isValidScan
        movePiece 0 2 0 3
        isValidScan
        movePiece 0 0 3 3
        isValidScan
        underThreatScan
        movePiece 2 0 3 1
        isValidScan
        movePiece 1 2 2 3
        isValidScan
        movePiece 3 1 2 0
        isValidScan
        movePiece 2 3 3 3
        isValidScan
        movePiece 2 3 1 2
        isValidScan
        underThreatScan
        movePiece 2 0 3 1
        isValidScan
        movePiece 0 3 0 1
        isValidScan
        movePiece 3 1 2 0
        isValidScan
        movePiece 0 1 0 3
        isValidScan
        movePiece 0 0 0 1
        isValidScan
        underThreatScan
        movePiece 2 0 3 0
        isValidScan
        movePiece 0 3 0 2
        isValidScan
        movePiece 3 0 2 0
        isValidScan
        movePiece 0 2 1 2
        isValidScan
        movePiece 2 2 3 1
        isValidScan
        underThreatScan
        movePiece 2 0 3 0
        isValidScan
        movePiece 3 3 2 3
        isValidScan
        movePiece 3 0 2 0
        isValidScan
        movePiece 2 3 2 2
        isValidScan
        movePiece 1 1 3 1
        isValidScan
        underThreatScan
        movePiece 2 0 3 0
        isValidScan
        movePiece 2 2 1 3
        isValidScan
        movePiece 3 0 3 1
        isValidScan
        movePiece 1 3 2 3
        isValidScan
        movePiece 0 0 1 3
        isValidScan
        underThreatScan
        movePiece 3 1 2 0
        isValidScan
        movePiece 2 3 3 3
        isValidScan
        movePiece 2 0 3 1
        isValidScan
        movePiece 3 3 2 3
        isValidScan
        movePiece 2 2 1 0
        isValidScan
        underThreatScan
        movePiece 3 1 2 1
        isValidScan
        movePiece 1 2 3 2
        isValidScan
        movePiece 2 1 2 0
        isValidScan
        movePiece 3 2 2 2
        isValidScan
        movePiece 0 1 0 0
        isValidScan
        underThreatScan
        movePiece 2 0 1 0
        isValidScan
        movePiece 2 2 3 2
        isValidScan
        movePiece 1 0 2 0
        isValidScan
        movePiece 3 2 3 1
        isValidScan
        movePiece 3 2 0 3
        isValidScan
        underThreatScan
        movePiece 2 0 3 1
        isValidScan
        movePiece 2 3 3 3
        isValidScan
        movePiece 3 1 2 0
        isValidScan
        movePiece 3 3 3 2
        isValidScan
        movePiece 1 2 3 1
        isValidScan
        underThreatScan
        movePiece 2 0 1 0
        isValidScan
        movePiece 3 2 3 3
        isValidScan
        movePiece 1 0 0 0
        isValidScan
        movePiece 3 3 3 2
        isValidScan
        movePiece 0 3 0 1
        isValidScan
        underThreatScan
        movePiece 0 0 1 1
        isValidScan
        movePiece 3 2 3 1
        isValidScan
        movePiece 1 1 0 0
        isValidScan
        movePiece 3 1 2 0
        isValidScan
        movePiece 2 3 1 0
        isValidScan
        underThreatScan
        movePiece 0 0 0 1
        isValidScan
        movePiece 2 0 3 0
        isValidScan
        movePiece 0 1 1 2
        isValidScan
        movePiece 3 0 2 0
        isValidScan
    )";

    runTestSequence(sBoard, testSequence, logFile);

    logFile.close();
    return EXIT_SUCCESS;
}