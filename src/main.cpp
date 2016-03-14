/*
 * main.cpp
 *
 *  Created on: 4. 3. 2016
 *      Author: Martin Tuma
 *
 * main function controls the flow of the tic-tac-toe game.
 * implements exception handling to quit the execution properly in case exceptions are thrown.
 */

#include "Board.h"
#include "Player.h"
#include "AiPlayer.h"
#include "TUI.h"

#include <iostream>
#include <sstream>
#include <stdexcept>


int main (){

	TUI ui;					// create ui for user input / output
	Board myboard; 			// create a board

	Player *players[2];		// create an array for players - its an array of pointers to player objects to be created at a later stage.

	bool game_replay = false;
	int game_mode_answer = -1;
	int game_replay_answer = -1;
	int current_player = 0;

	// Lets put the text and prompts for communicating with the user in one place

	// Welcome message
	std::ostringstream game_welcome_msg;
	game_welcome_msg << "Welcome to Tic-Tac-Toe!\n";

	// Rules message
	std::ostringstream game_rules_msg;
	game_rules_msg	<< "The board size is set to " << Board::kRows << " rows and " << Board::kCols <<" columns.\n"
					<< "You win if you have " << Board::kWinLine << " symbols in a row.\n"
					<< "Player X starts the game.";

	std::ostringstream game_draw_msg;
	game_draw_msg << "\nGAME OVER - The game is a DRAW.";

	std::ostringstream game_winx_msg;
	game_winx_msg << "\nGAME OVER - Player X WON the game.";

	std::ostringstream game_wino_msg;
	game_wino_msg << "\nGAME OVER - Player O WON the game.";

	// Game mode dialogue
	std::ostringstream game_mode_dialogue, game_mode_prompt;
	game_mode_dialogue	<< "\nPlease choose game mode:\n\n"
						<< "\t[1]\tComputer (X)\t vs.\t Human (O)\n"
						<< "\t[2]\tHuman (X)\t vs.\t Computer (O)\n"
						<< "\t[3]\tComputer (X)\t vs.\t Computer (O)\n"
						<< "\t[4]\tHuman (X)\t vs.\t Human (O)\n\n"
						<< "\t[0]\tQuit Game.\n";
	game_mode_prompt 	<< "Please enter [1-4 or 0]: ";
	int game_mode_max = 4; // make sure this is set to the number of the last menu item

	// Replay game dialogue
	std::ostringstream game_replay_dialogue, game_replay_prompt;
	game_replay_dialogue	<< "\nDo you wish to play another round?\n\n"
							<< "\t[1]\tYES let's start over.\n\n"
							<< "\t[0]\tNO please QUIT the game.\n";
	game_replay_prompt 		<< "Please enter [1 or 0]: ";
	int game_replay_max = 1; // make sure this is set to the number of the last menu item

	// Start the game
	ui.message(game_welcome_msg.str());			// say Hi.
	ui.message(game_rules_msg.str());			// announce board size and rules

	try {										// this part is risky, lets catch exceptions.
		do {
			game_replay = false;		//make sure we don't go into an infinite loop
			current_player = 0;			//reset the player that starts
			game_mode_answer = -1;		//reset game mode answer

			// get the game mode input from user and store it in game_mode_answer
			game_mode_answer = ui.dialogue(game_mode_dialogue.str(),game_mode_prompt.str(),game_mode_max);

			// evaluate the game_mode_answer
			switch (game_mode_answer){
			case 0:
				return 0;
			case 1:
				players[0]= new AiPlayer('X');	// allocate space for and create an instance of AiPlayer - space must be explicitly released at the end!!!
				players[1]= new Player('O');	// allocate space for and create an instance of Player - space must be explicitly released at the end!!!
				break;
			case 2:
				players[0]= new Player('X');
				players[1]= new AiPlayer('O');
				break;
			case 3:
				players[0]= new AiPlayer('X');
				players[1]= new AiPlayer('O');
				break;
			case 4:
				players[0]= new Player('X');
				players[1]= new Player('O');
				break;
			/*case 5: // Test Case for AI quality
				players[0]= new AiPlayer('X');
				players[1]= new AiPlayer('O');
				myboard.makeMove(1,2,'X');
				myboard.makeMove(2,3,'X');
				myboard.makeMove(3,3,'X');
				myboard.makeMove(3,1,'O');
				myboard.makeMove(3,2,'O');
				current_player = 1;
				break;*/
			default:
				// this is reached only if answer was not modified - this should never happen.
				throw "UNREACHEABLE CODE - Error in program flow";
			}

			while (myboard.evaluateBoard()== Board::PLAY){			// while we can play
				myboard.printBoard(ui);								// print current board status
				players[current_player]->performMove(myboard,ui);	// get the move from Player or AiPlayer
				if (current_player == 0){							// swap the players
					current_player = 1;
				} else {
					current_player = 0;
				}
			}
			myboard.printBoard(ui);									// display the final board on the screen

			Board::BoardStatus final_status = myboard.evaluateBoard(); // evaluate the board status

			switch (final_status){
			case Board::DRAW:
				ui.message(game_draw_msg.str());
				break;
			case Board::WINX:
				ui.message(game_winx_msg.str());
				break;
			case Board::WINO:
				ui.message(game_wino_msg.str());
				break;
			default:
				// this is reached only if answer was not modified - this should never happen.
				throw "UNREACHEABLE CODE - Error in program flow";
			}

			game_replay_answer = -1;			//reset game replay answer

			// get the game replay input from user and store it in game_replay_answer
			game_replay_answer = ui.dialogue(game_replay_dialogue.str(),game_replay_prompt.str(),game_replay_max);

			// evaluate the game_replay_answer
			switch (game_replay_answer){
			case 0:
				delete players[0];		//free up the storage
				delete players[1];		//free up the storage
				return 0;
			case 1:
				delete players[0];		//free up the storage
				delete players[1];		//free up the storage
				myboard.resetBoard();
				game_replay = true;
				break;
			default:
				// this is reached only if answer was not modified - this should never happen.
				throw "UNREACHEABLE CODE - Error in program flow";
			}
			ui.cls();
		} while (game_replay == true);

	} // catching exceptions
	catch(const std::exception& e){ //catch all standard exceptions
		std::cerr << e.what() << " - QUITTING." << std::endl;
	}
	catch (const char* msg){
		std::cerr << msg << " - QUITTING." << std::endl;
	}
	catch(...){ //catch all other exceptions
		std::cerr << "An unexpected error has occurred. - QUITTING" << std::endl;
	}
	delete players[0];		//free up the storage
	delete players[1];		//free up the storage
	return 0;
}
