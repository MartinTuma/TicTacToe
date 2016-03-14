/*
 * TUI.cpp
 *
 *  Created on: 4. 3. 2016
 *      Author: Martin Tuma
 *
 * TUI (Textual User Interface) class implementation.
 * The TUI class is responsible for displaying messages and dialogues on the screen and for acquiring and validating user input.
 * The methods of the TUI class are used by the Board, Player and AiPlayer classes as well as the main function.
 *
 */

#include "TUI.h"

#include <limits>
#include <stdexcept>
#include <sstream>

/*
 * Constructor
 */
TUI::TUI() {
}

/*
 * Destructor
 */
TUI::~TUI(){
	// TODO implement destructor if necessary
}

/*
 * message() - displays a message on the screen
 * expected input is a message string to be displayed.
 */
void TUI::message(const std::string message){
	std::cout << message << "\n" ;
}

void TUI::cls(){
	std::cout << std::string(100, '\n');
}

/*
 * dialogue() - displays a message, a prompt and requests an input from the user.
 * Expects the dialog message, the dialog prompt and the maximum valid answer (max_answer) as inputs.
 * Answers are expected to be within the range of 0-max_answer. The user input is validated in two stages.
 * In the first stage the input is checked to be an integer, in the second stage the integer is being checked to be
 * within the 0-max_answer range. The second stage of validation may be omitted if the constant kDontValidate as the
 * max_answer input.
 */
int TUI::dialogue( const std::string dialog_msg, const std::string dialog_prompt, const int max_answer){
	// initialize variables
	int answer = -1;
	int input = -1;
	bool valid_answer = false;
	bool valid_input = false;
	int retry = 0;

	while (valid_answer == false){  					// 2nd stage of validation
		while (valid_input == false){ 					// 1st stage of validation
			message(dialog_msg);						// display dialog message and prompt
			std::cout << dialog_prompt << std::flush;
			std::cin >> input;							// read input from cin into variable input
			if (std::cin.fail()){						// if the input was was not succesfully read into input? (the input was not an integer)
				std::cin.clear();						// reset the cin stream
				std::cin.ignore(std::numeric_limits<int>::max(),'\n'); //ignore any remaining chars on the line
				retry++;								// increase the retry count
				if (retry > kRetry){					// if max retry count was reached throw an exception
					throw std::invalid_argument("INVALID INPUT - Not an integer");
				} else {								// else request user to repeat input
					std::ostringstream retry_message;
					retry_message << "Invalid input. Please enter a valid number. RETRY [" << retry << "/" << kRetry << "]";
					message(retry_message.str());
				}
			} else {
				valid_input = true;
				std::cin.ignore(std::numeric_limits<int>::max(),'\n');	//ignore any remaining chars on the line if any
																		//(in case the user entered an integer and some gibberish afterwards before pushing enter)
			}
		}
		if (max_answer == kDontValidate){
			//input validation will be performed in calling code
			answer = input;
			valid_answer = true;
		} else {
			//perform basic input validation
			if ( input < 0  || input > max_answer ){ 	// if the entered int is not within the expected range
				retry++;								// increase retry count
				if (retry > TUI::kRetry){				// if max retry count was reached throw an exception
					throw std::invalid_argument("INVALID INPUT - Selection/Answer out of range");
				}else{									// else request user to repeat input
					valid_input = false;
					std::ostringstream retry_message;
					retry_message << "Invalid answer. Please select a valid answer from 1 to " << max_answer << " or 0. RETRY [" << retry << "/" << kRetry << "]";
					message(retry_message.str());
				}
			} else {
				answer = input;
				valid_answer = true;
			}
		}
	}
	return answer;
}


