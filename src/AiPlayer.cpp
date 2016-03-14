/*
 * AiPlayer.cpp
 *
 *  Created on: 4. 3. 2016
 *      Author: Martin Tuma
 *
 * AiPlayer class implementation.
 * The AiPlayer class is responsible for generating the moves for the Computer player for the Tic-Tac-Toe game.
 * The AiPlayer class is a child class of the Player class.
 * The AiPlayer class overrides the implementation of the Players class performMove() method.
 */

#include "AiPlayer.h"

#include <vector>
#include <climits>
#include <sstream>
#include <ctime>

/*
 * AiPlayer constructor, calls the Player constructor.
 * Constructor argument is the mark of the player to be created.
 */
AiPlayer::AiPlayer(const char mark) : Player(mark) {
}

/*
 * AiPlayer destructor, calls the Player destructor.
 */
 AiPlayer::~AiPlayer() {
	// TODO implement destructor if necessary
 }

/*
 * performMove - is responsible for the interaction with the tic-tac-toe board (placing the mark of the Computer on the board).
 * The method performMove expects a pointer to the tic-tac-toe board and a pointer to an instance of the TUI class as inputs.
 * The TUI class is used to display messages of the AiPlayer on the screen.
 * performMove calls the AiPlayer class private function miniMaxAB() in order to generate the best move and uses the method makeMove
 * from the Board class to place the mark on the board.
 */
void AiPlayer::performMove(Board& board, TUI& ui){
	char mark = getMark();
	AiMove best_move = miniMaxAB(board, 0, kLookAhead, INT_MIN, INT_MAX, mark);

	std::ostringstream turn_msg;
	turn_msg << "\nIts the turn of Player " <<  mark << ". \n"
			 << "Moving to [column|row]: [" << best_move.col << "|" << best_move.row <<"]";
	ui.message(turn_msg.str());

	board.makeMove(best_move.row, best_move.col, mark);
}

/*
* scoreMove - method to score the terminalMoves
* Expected input is a pointer to the tic-tac-toe board for winner evaluation and an integer representing the number of turns
* to reach the evaluated board.
* Output is the score of the current move.
*/
int	AiPlayer::scoreMove(Board& board, const int turn) const{
	int score = 0;
	char my_mark = getMark();

	// score the winning situation
	char winner = board.getWinner(Board::kWinLine);
	if ( winner == Board::kEmpty){	//Draw situation
		score = 0;					//Score = 0
	} else if ( winner == my_mark){	//Current player wins
		score = 100 - turn;			//Score = +100 + turn
	} else {						//Opponent wins
		score = -100 + turn;		//Score = -100 + turn
	}
return score;
}

/*
* generateMoves - method to generate all possible moves in a particular game situation
* Expected input is a pointer to the tic-tac-toe board.
* Output is a pointer address of a vector including all possible moves (without score) in a particular game situation.
* Returning just the pointer provides optimization as we dont need to copy the whole vector just pass by ref.
*/
std::vector<AiMove> AiPlayer::generateMoves(Board& board) const{
	std::vector<AiMove> moves;
	for (int i=1; i<board.kRows+1; i++){
		for (int j=1; j<board.kCols+1; j++){
		 if (board.validMove(i,j)){
			 AiMove move = AiMove(i,j);
			 moves.push_back(move);
		 }
		}
	}
	return moves;
}

/*
 * getOppMark() - returns the mark of the other player (the opponent)
 */
 char AiPlayer::getOppMark() const{
 	if ( getMark()== 'X'){
 		return 'O';
 	}else{
 		return 'X';
 	}
 }

/*
 * miniMaxAB() - (recursive) MiniMax algorithm with cut-off.
 * Expected inputs are a pointer to the tic-tac-toe board, the current turn (used for scoring),
 * the current look ahead level, current alpha and beta value for cut-off, the mark of the currently moving player.
 * Output is the best possible move for the current board within the lookahead limit.
 *
 * as introduced here: http://www3.ntu.edu.sg/home/ehchua/programming/java/javagame_tictactoe_ai.html
 * and here: http://neverstopbuilding.com/minimax
 */
AiMove AiPlayer::miniMaxAB(Board& board, int turn, int look_ahead, int alpha, int beta, char mark) {
	std::vector<AiMove> moves = generateMoves (board);
	int best_move = 0;

	if ( board.evaluateBoard() != Board::PLAY || look_ahead == 0){
		return AiMove(scoreMove(board, turn));
	}
	for (int i=0,max=moves.size(); i<max; i++){
		board.makeMove(moves[i].row,moves[i].col,mark);		// simulate move on board
		if (mark == getMark()){								// if its my turn I am maximizing
			moves[i].score = miniMaxAB(board, turn+1, look_ahead-1, alpha, beta, getOppMark()).score; // call minMaxAB recursively to generate score (switching players)
			if (moves[i].score > alpha){					// do we have a higher score than alpha then
				alpha = moves[i].score;						// assign higher score to alpha
				best_move = i;								// change index of best move
			}
		}else{												// if its the turn of my opponent he is minimizing
			moves[i].score = miniMaxAB(board, turn+1, look_ahead-1, alpha, beta, getMark()).score; // call minMaxAB recursively to generate score (switching players)
			if (moves[i].score < beta){						// is there a lower score than beta then
				beta = moves[i].score;						// assign lower score to beta
				best_move = i;								// change index of best move
			}
		}
		board.removeMove(moves[i].row,moves[i].col);		// remove move from board
		if (alpha >= beta){									// cut-off move generation and scoring if alpha is greater or equal to beta
			break;											// as a perfect player will not choose this path
		}
	}
return moves[best_move];
}
