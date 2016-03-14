/*
 * Board.cpp
 *
 *  Created on: 4. 3. 2016
 *      Author: Martin Tuma
 *
 * Board - class implementation.
 * The board class is responsible for storing the status of the tic-tac-toe board. The whole game logic/rules is implemented within this class.
 * Methods to manipulate the board (setting/removing marks) move and board evaluation are implemented within this class.
 */

#include "Board.h"

#include <sstream>

/*
 * Board() - Constructor
 * reset the board and move counter
 */
Board::Board() {
	resetBoard();
}

/*
 * ~Board() - Destructor
 */
Board::~Board() {
	// TODO implement destructor if necessary
}

/*
 * resetBoard()
 * Clears the entire board (fill array with kEmpty) and resets the move count according to the board dimensions
 */
void Board::resetBoard(){
	for (int i=0; i<kRows; i++){
		for (int j=0; j<kCols; j++){
			fields_[i][j]=kEmpty;
		}
	}
	available_moves_ = kRows*kCols;
}

/*
 * makeMove() - Sets a mark on the board, reduces available_moves_ by 1
 * inputs are row, column, mark (X or O)
 */
void Board::makeMove(const int row, const int column, const char mark){
	setMark(row, column, mark);			//call setMark() method
	available_moves_--;					//reduce number of available moves;
}

/*
 * removeMove() - clears a mark on the board, increases available_moves_ by 1
 * inputs are row, column
 * Used by AiPlayer remove simulated moves
 */
void Board::removeMove(const int row, const int column){
	setMark(row, column, kEmpty);		//call setMark() method
	available_moves_++;					//increase number of available moves;
}

/*
 * validMove() - check if move is valid (check if field is inside of the array and is empty)
 * Inputss are row, column
 * Output value is true = move is valid or false = move invalid
 */
bool Board::validMove( const int row, const int column ) const{
	int i = row - 1;					//reduce row by 1 to get the array index
	int j = column - 1;					//reduce column by 1 to get the array index
	bool valid_move = false;

	if (i > kRows-1 || j > kCols-1){	//invalid move, field outside of array
		valid_move = false;
	}
	if (fields_[i][j] != kEmpty){		//invalid move, field already occupied
		valid_move = false;
	} else {							//valid move
		valid_move = true;
	}
	return valid_move;
}

/*
 * setMark() - sets a mark in the required field
 * Inputs are row, column, mark (X, O or kEmpty)
 */
void Board::setMark(const int row, const int column, const char mark){
	int i = row - 1;						//reduce row by 1 to get the array index
	int j = column - 1;						//reduce column by 1 to get the array index
	fields_[i][j]=mark;						//set mark at index
}

/*
 * evaluateBoard() - returns the board status
 * Return values: 	PLAY - game goes on,
 * 					DRAW - game over noone wins,
 * 					WINX - game over X wins,
 * 					WINO - game over O wins
 */
Board::BoardStatus Board::evaluateBoard() const {

	char winner = getWinner(kWinLine);				// get the winner if there is one
	Board::BoardStatus status = PLAY;				// assume we can still play the board

	if (winner == 'X'){ 							//player X wins
		status = WINX;
	}
	if (winner == 'O'){								//player O wins
		status = WINO;
	}
	if (winner == kEmpty && available_moves_ <= 0){ 	//no winner and no more moves left - DRAW
		status = DRAW;
	}
	if (winner == kEmpty && available_moves_ > 0){  	//no winner and moves are still left - PLAY
    	status = PLAY;
	}
	return status;
}

/*
 * getWinner() - evaluate winner and return his mark
 */
char Board::getWinner( const int marks_in_row ) const{

	char winner = kEmpty;						//initialize winner with Empty

	int maxrow = kRows-marks_in_row+1;				//get maxrow - to stay within the array when searching
	int maxcol = kCols-marks_in_row+1;				//get maxcol


	//  if winner is not found, evaluate rows
	for (int i=0; i<kRows && winner==kEmpty; i++){			//for each row
		for (int j=0; j<maxcol && winner==kEmpty; j++){  	//for each column-marks_in_row+1
			char current_mark = kEmpty;						//reset current_mark
			int counter = 0;								//reset counter
			if (fields_[i][j] != kEmpty){     				//skip empty fields
				current_mark = fields_[i][j];				//set current_mark to first non empty field contents
				for (int k=0; k<marks_in_row; k++){
					if (fields_[i][j+k] == current_mark){ 	//go right - if the next is the same as current
						counter++;							//increase count
						if (counter == marks_in_row) {			//if we have a win situation
							winner = current_mark;			//set the winner for this board
						}
					}else{
						counter = 0;				//reset counter if the next field is not the same as current_mark
					}
				}
			}else{
				counter = 0;						//reset counter if the field is Empty
			}
		}
	}

	//  if winner is not found, evaluate columns
	for (int i=0; i<maxrow && winner==kEmpty; i++){			//for each row-marks_in_row+1
		for (int j=0; j<kCols && winner==kEmpty; j++){  	//for each column
			char current_mark = kEmpty;						//reset current_mark
			int counter = 0;								//reset counter
			if (fields_[i][j] != kEmpty){     				//skip empty fields
				current_mark = fields_[i][j];				//set current_mark to first non empty field contents
				for (int k=0; k<marks_in_row; k++){
					if (fields_[i+k][j] == current_mark){ 	//go down - if the next is the same as current
						counter++;							//increase count
						if (counter == marks_in_row) {			//if we have a win situation
							winner = current_mark;			//set the winner for this board
						}
					}else{
						counter = 0;				//reset counter if the next field is not the same as current_mark
					}
				}
			}else{
				counter = 0;						//reset counter if the field is Empty
			}
		}
	}

	//  if winner is not found, evaluate diagonals Left To Right
	for (int i=0; i<maxrow && winner==kEmpty; i++){					//for each row-marks_in_row+1
			for (int j=0; j<maxcol && winner==kEmpty; j++){  		//for each column-marks_in_row+1
				char current_mark = kEmpty;							//reset current_mark
				int counter = 0;									//reset counter
				if (fields_[i][j] != kEmpty){     					//skip empty fields
					current_mark = fields_[i][j];					//set current mark to first non empty field contents
					for (int k=0; k<marks_in_row; k++){
						if (fields_[i+k][j+k] == current_mark){ 	//go down and right - if the next is the same as current_mark
							counter++;								//increase counter
							if (counter == marks_in_row) {				//if we have a win situation
								winner = current_mark;				//set the winner for this board
							}
						}else{
							counter = 0;					//reset counter if the next field is not the same as current_mark
						}
					}
				}else{
					counter = 0;							//reset counter if the field is Empty
				}
			}
		}

	//  if winner is not found, evaluate diagonals Right To Left
	for (int i=0; i<maxrow && winner==kEmpty; i++){						//for each row 0 ... row-marks_in_row+1
			for (int j=marks_in_row-1; j<kCols && winner==kEmpty; j++){ 	//for each column marks_in_row ... COLS
				char current_mark = kEmpty;								//initialize current_mark
				int counter = 0;										//we start counting with 0
				if (fields_[i][j] != kEmpty){     						//skip empty fields
					current_mark = fields_[i][j];						//set current mark to first non empty field contents
					for (int k=0; k<marks_in_row; k++){
						if (fields_[i+k][j-k] == current_mark){ 		//go down and left - if the next is the same as current_mark
							counter++;									//increase count
							if (counter == marks_in_row) {					//if we have a win situation
								winner = current_mark;					//set the winner for this board
							}
						}else{
							counter = 0;	//reset counter if the next field is not the same as current_mark
						}
					}
				}else{
					counter = 0;	//reset counter if the field is Empty
				}
			}
		}
	return winner;
}

/*
 * printBoard() - displays the game board on the screen
 * Expected parameter: pointer to an instance of the TUI (Textual User Interface) class
 *
 */
void Board::printBoard(TUI& ui) const {

	//create the header row of the board
	std::ostringstream header_row;
	header_row << "\n   ";
    for (int i=0; i<kCols; i++){
    	header_row << "| " << i+1 << " ";
    }

	ui.message(header_row.str());			//display the header row

    //create the table
    //create the separating line
	std::ostringstream separator;
    for (int i=0; i<kCols; i++){
			separator << "----";
		}
	separator << "---";

    //create the table rows
	for (int i=0; i<kRows; i++ ){
		std::ostringstream table_row;
		table_row << " " << i+1 << " ";
		for (int j=0; j<kCols; j++){
			table_row << "| ";
				table_row << fields_[i][j] << " ";
		}
		ui.message(separator.str());		//display separator
		ui.message(table_row.str());		//display the table row
	}

}
