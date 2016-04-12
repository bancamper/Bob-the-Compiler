/*
	File: lex.cpp
	Name: Juan Bancamper
	Class: CMPT 432
	Date: 10 Feb 2016

	This file includes all the function definitions used to create Tokens
	based off of Design of Compilers Language Grammar
*/

#include <iostream>
#include <stdlib.h>
#include <map>
#include <regex>
#include <fstream>
#include <vector>
#include "bin/lex.h"
#include "bin/delta.h"

extern int state;
bool accept_space = false;

/*
create_token

Creates a structure of type Token

Parameters: desc, type, line_number
	desc: a string literal of the current character
	type: a string of the type of token created
	line_number: an integer respresenting the line in which the character was
		found

Return: token
	the token created
*/
struct Token create_token(std::string desc, std::string type, int line_number){
	Token token = {
		desc,
		type,
		line_number			
	};

	return token;
}

/*
reset_state

Sets the current state of the delta function to the start state

Parameters: state
	a integer reference to the current state

Return: none
*/
void reset_state(int &state){
	state = q0;
}

/*
is_accepting

Determines whether or not the state is accepting

Parameters: none

Return: boolean
	false: not accepting
	true: accepting
*/
bool is_accepting(){
	std::set<int>::iterator it = keywords_F.find(state);

	return (it == keywords_F.end() ? false : true);
}

/*
is_error

Checks if state is in an error state

Parameters: none

Return: boolean
	false: not in error state
	true: in error state
*/
bool is_error(){
	return state == q37;
}

/*
is_keyword

Determines if current character is within a keyword and changes the state
based on the input

Parameters: c
	current character read by lexer

Return: boolean
	false: not a character in keyword
	true: character in keyword, and state has been changed
*/
bool is_keyword(char c){
	int input;

	if(c =='a')
		input = 0;
	else if(c == 'b'){
		input = 1;
	}
	else if(c == 'e'){
		input = 2;
	}
	else if(c == 'f'){
		input = 3;
	}
	else if(c == 'g'){
		input = 4;
	}
	else if(c == 'h'){
		input = 5;
	}
	else if(c == 'i'){
		input = 6;
	}
	else if(c == 'l'){
		input = 7;
	}
	else if(c == 'n'){
		input = 8;
	}
	else if(c == 'o'){
		input = 9;
	}
	else if(c == 'p'){
		input = 10;
	}
	else if(c == 'r'){
		input = 11;
	}
	else if(c == 's'){
		input = 12;
	}
	else if(c == 't'){
		input = 13;
	}
	else if(c == 'u'){
		input = 14;
	}
	else if(c == 'w'){
		input = 15;
	}
	else{
		state = q37;
		return false;
	}

	state = keywords[state][input];

	return true;

}

/*
is_identifier

Determines if current character is an alpha

Parameters: c
	current character read by lexer

Return: boolean
	false: not an alpha character
	true: alpha character
*/
bool is_identifier(char c){
	return std::regex_match(std::string(1, c), std::regex("[a-z]"));
}

/*
is_symbol

Determines if current character is a symbol

Parameters: c
	current charater read by lexer

Return: boolean
	false: not a symbol
	true: symbol
*/
bool is_symbol(char c){
	// std::cout << "Symbol: " << c << std::endl;

	std::set<char>::iterator it = symbols_set.find(c);

	if(it == symbols_set.end()){
		// std::cout << "Not a symbol" << std::endl;

		return false;
	}
	else{
		// std::cout << symbols_map.find(std::string(1,c)) -> first 
		// << " is " << symbols_map.find(std::string(1,c)) -> second << std::endl;

		if(c == '\"'){
			if(accept_space){
				accept_space = false;
			}
			else{
				accept_space = true;
			}
		}

		return true;
	}
}

/*
is_digit

Determines if current character is a digit

Parameters: c
	current character read by lexer

Return: boolean
	false: not a digit
	true: digit
*/
bool is_digit(char c){
	return std::regex_match(std::string(1, c), std::regex("[0-9]"));
}

/*
lets_get_lexical

Runs through the process of lexical analysis from a file buffer 
using helper functions to determine tokens produced

Parameters: file
	the address path to the source code text file

Return: tokens
	a vector containing all the structured tokens produced by the lexer from
	the source code
*/
std::vector<Token> lets_get_lexical(std::string file){
	std::vector<Token> tokens;
	create_symbols_map();

	// const int SIZE = 100;
	int line_count = 1;
	// char buffer[SIZE];

	std::ifstream source_code;
	source_code.open(file);
	std::string line;

	while(std::getline(source_code, line)){

		char *buffer = new char[line.length() + 1];
		std::strcpy(buffer, line.c_str()); 

		for(int i = 0; i < strlen(buffer); i++){
			reset_state(state);

			// std::cout << "State before checking char = " << state << std::endl;
			
			char c = buffer[i];

			if(is_keyword(c)){
				bool keyword = false;
				int current = i + 1;

				for(; current < strlen(buffer); current++){
					// std::cout << "State = " << state << std::endl;
					char next_c = buffer[current];
					is_keyword(next_c);

					if(is_accepting()){
						std::string key(buffer);

						// std::cout << "Token (keyword: "
							// << key.substr(i, (current - i) + 1) 
							// << ") found at line " << line_count << std::endl;

						tokens.push_back(
							create_token(
								key.substr(i, (current - i) + 1), 
								"keyword", 
								line_count
							)
						);

						keyword = true;
						break;
					}
					else if (is_error()){
						break;
					}
				}

				if(keyword){
					i = current;
					continue;
				}
			}
			if (is_identifier(c)){
				// std::cout << "Token (identifier: " << c 
					// << ") found at line " << line_count << std::endl;

				tokens.push_back(
					create_token(
						std::string(1, c),
						"identifier",
						line_count
					)
				);
				continue;
			}
			if(is_digit(c)){
				// std::cout << "Token (digit: " << c 
					// << ") found at line " << line_count << std::endl;

				tokens.push_back(
					create_token(
						std::string(1, c), 
						"digit", 
						line_count
					)
				);
				continue;
			}
			if(is_symbol(c)){
				char pos_symb = buffer[i + 1];

				// std::cout << "Next symbol: " << pos_symb << std::endl;

				if(pos_symb == '='){
					i++;
					std::string sym = std::string(1, c) += pos_symb;

					tokens.push_back(
						create_token(
							sym,
							symbols_map.find(sym) -> second,
							line_count
						)
					);
				}
				else{
					tokens.push_back(
						create_token(
							std::string(1, c),
							symbols_map.find(std::string(1, c)) -> second,
							line_count
						)
					);
				}
			}
			else{
				if(c == ' '){
					if(accept_space){
						tokens.push_back(
							create_token(
								std::string(1, c),
								"space",
								line_count
							)
						);
					}
				}
				else{
					std::cerr << "Invalid character: " << c << " at line " 
						<< line_count << std::endl;

					exit(EXIT_FAILURE);
				}
			}
		}

		line_count++;
	}

	source_code.close();

	std::cout << std::endl;

	for(std::vector<Token>::iterator i=tokens.begin(); i != tokens.end(); ++i){
		std::cout << "Token (" << i->type << ": " << i->desc 
			<< ") found at line " << i->line_number << std::endl;
	}


	return tokens;
}