/*
 * Player.cpp
 *
 *  Created on: 4. 3. 2016
 *      Author: Martin Tuma
 *
 * Player class implementation.
 * The Player class is responsible for storing the data of the particular player (players mark) and the interaction
 * with the human player for the Tic-Tac-Toe game.
 * The method performMove() is responsible for interaction with the human player and the placement of the mark on the
 * board. The implementation of the method performMove() is overriden in the subclass AiPlayer to generate the moves.
 */

#include "Player.h"

#include <sstream>
#include <stdexcept>

/*
 * Constructor - creates a new instance and sets the mark of the player.
 */
Player::Player(const char mark) {
	Player::setMark(mark);
}

 /*
  * Destructor
  */
Player::~Player() {
	// TODO implement destructor if necessary
	// std::cout << "Good bye player: " << getMark() << std::endl;
}

/*
 * setMark() - sets the mark of the player.
 */
void Player::setMark( const char mark ){
	players_mark_ = mark;
}

/*
 * getMark() - returns the mark of the player
 */
char Player::getMark() const{
	return players_mark_;
}

/*
 * performMove() - is responsible for the interaction with the tic-tac-toe board (placing the mark of the User on the board).
 * The method performMove() expects a pointer to the tic-tac-toe board and a pointer to an instance of the TUI class as arguments.
 * The TUI class is used for user interaction.
 * performMove() requests the move coordinates from the user, check them for validity by calling the method validMove() from the Board class and
 * invokes the method makeMove() from the Board class to place the mark on the board.
 */
void Player::performMove(Board& board, TUI& ui){

	//initialize variables
	char mark = getMark();
	int row = -1;
	int col = -1;
	int retry = 0;
	bool valid_move = false;

	std::ostringstream turn_dialogue_col, turn_dialogue_row;
	std::ostringstream turn_prompt_col, turn_prompt_row;

	//text for requesting the move coordinates from user (request column first to be user friendly)
	turn_dialogue_col	<< "\nIts the turn of Player " <<  mark << ".\n"
				 	  	<< "Please enter the column of your next move.";
	turn_prompt_col		<< "Column: ";

	//text for requesting the move coordinates from user - row
	turn_dialogue_row	<< "Please enter the row of your next move.";
	turn_prompt_row		<< "Row: ";

	while ( valid_move == false ) { // execute the following code until you get a valid move

		// get the column from the user and store it in col
		// the argument TUI::kDontValidate means that the dialogue() method of the TUI class will not validate the user
		// input beyond checking that its an integer.
		col = ui.dialogue(turn_dialogue_col.str(),turn_prompt_col.str(),TUI::kDontValidate);

		// get the row from the user and store it in row
		row = ui.dialogue(turn_dialogue_row.str(),turn_prompt_row.str(),TUI::kDontValidate);

		// perform the external validation of the input - validate the move
		valid_move = board.validMove(row,col);

		// if move is invalid try to get a valid move from the user up to kRetry times (standard set to 3)
		if (valid_move == false){
			 retry++;
			if (retry > TUI::kRetry){
		    	throw std::invalid_argument("INVALID MOVE");
			} else {
			 std::ostringstream retry_message;
			 retry_message << "\nINVALID MOVE, please try again. The cell is already occupied or outside of the board. [" << retry << "/" << TUI::kRetry << "]";
			 ui.message(retry_message.str());
			 board.printBoard(ui);
			}
		} else {
		// if move is valid, put the mark on the board
			board.makeMove(row,col,mark);
		}
	}
}
