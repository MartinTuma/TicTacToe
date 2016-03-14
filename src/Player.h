/*
 * Player.h
 *
 *  Created on: 4. 3. 2016
 *      Author: Martin Tuma
 *
 * Player class definition.
 * The Player class is responsible for storing the data of the particular player (players mark) and the interaction
 * with the human player for the Tic-Tac-Toe game.
 * The method performMove() is responsible for interaction with the human player and the placement of the mark on the
 * board. The Player class serves as a superclass for the AiPlayer class, this method performMove() is overriden in the
 * subclass AiPlayer to automatically generate the moves.
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include "Board.h"

class Player {
public:
	Player(const char mark);						//Constructor - set mark of new player
	Player();										//default Constructor
	virtual ~Player();								//default Destructor
	char getMark() const;							//Get players mark
	void setMark(const char mark);					//Set players mark called by constructor
	virtual void performMove(Board& board, TUI& ui);//Requests user input, validates move, sets mark on board
private:
	char players_mark_;								//Mark of the player (X or O)
};

#endif /* PLAYER_H_ */
