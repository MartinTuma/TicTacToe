/*
 * Board.h
 *
 *  Created on: 4. 3. 2016
 *      Author: Martin Tuma
 *
 * Board - class definition.
 * The board class is responsible for storing the status of the tic-tac-toe board. The whole game logic/rules is implemented within this class.
 * Methods to manipulate the board (setting/removing marks) move and board evaluation are implemented within this class.
 */

#ifndef BOARD_H_
#define BOARD_H_

#include "TUI.h"

class Board {
public:
	enum BoardStatus {PLAY,DRAW,WINX,WINO};	//enumeration of the board status
	static const int kRows = 3; 			//number of rows
	static const int kCols = 3; 			//number of cols
	static const int kWinLine = 3; 			//define winning situation (default 3 in a row)
											//an interesting game setup is a 8x8 board with 5 in a row to win, and AiPlayer kLookAhead set to 6
	static const char kEmpty = ' ';			//define empty char to avoid mistakes

	Board();															//constructor - create a board for the game
	virtual ~Board();													//destructor

	void resetBoard();													//clear the board/reset available move count - prepare it for a game

	void makeMove(const int row, const int column, const char mark);	//put a mark on the board
	void removeMove(const int row, const int column);					//remove a mark from the board - for simulations
	bool validMove(const int row, const int column) const;				//is move valid?

	BoardStatus evaluateBoard() const;									//return the board status as per enum Board_Status
	char getWinner(const int marks_in_row) const;						//return mark of the player reaching number of marks_in_row or empty

	void printBoard(TUI& ui) const;										//print the board to screen
private:
	char fields_[kRows][kCols];		//create 2d array to store the marks in
	int available_moves_;			//track the number of available moves for board status evaluation

	void setMark(const int row, const int column, const char mark);		//set a mark on the board
};

#endif /* BOARD_H_ */
