/*
 * TUI.h
 *
 *  Created on: 4. 3. 2016
 *      Author: Martin Tuma
 *
 * TUI (Textual User Interface) - class definition.
 * The TUI class is responsible for displaying messages and dialogues on the screen
 * and for acquiring and validating user input. The methods of the TUI class are used
 * by the Board, Player and AiPlayer classes as well as the main function.
 */

#ifndef TUI_H_
#define TUI_H_

#include <string>
#include <iostream>

//const int TUI_RETRY = 3;

class TUI {
public:
	static const int kRetry = 3;				// standard retry count
	static const int kDontValidate = -1;		// value used in the dialogue method if we don't want input to be validated for plausibility by the dialogue() method
	TUI();										// constructor
	virtual ~TUI();								// destructor
	void message(const std::string message);	// display a message on the screen terminated by a newline char.
	void cls();									// clear screen method
	int dialogue(const std::string message, const std::string prompt, const int max_answer); // display a dialogue on the screen, request user input and validate it
private:
};

#endif /* TUI_H_ */
