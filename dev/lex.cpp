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

struct Token create_token(std::string desc, std::string type, int line_number){
	Token token = {
		desc,
		type,
		line_number			
	};

	return token;
}

void reset_state(int& state){
	state = q0;
}

bool is_accepting(){
	std::set<int>::iterator it = keywords_F.find(state);

	return (it == keywords_F.end() ? false : true);
}

bool is_error(){
	return state == q37;
}

bool is_keyword(char c){
	int input;

	// std::cout << "Char: " << c << std::endl;

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

bool is_identifier(char c){
	return std::regex_match(std::string(1, c), std::regex("[a-z]"));
}

bool is_symbol(char c){
	std::cout << "Symbol: " << c << std::endl;

	std::set<char>::iterator it = symbols_set.find(c);

	if(it == symbols_set.end()){
		std::cout << "Not a symbol" << std::endl;

		return false;
	}
	else{
		std::cout << symbols_map.find(std::string(1,c)) -> first 
		<< " is " << symbols_map.find(std::string(1,c)) -> second << std::endl;

		return true;
	}
}

bool is_digit(char c){
	return std::regex_match(std::string(1, c), std::regex("[0-9]"));
}

std::vector<Token> lets_get_lexical(std::string file){
	std::vector<Token> tokens;
	create_symbols_map();

	const int SIZE = 100;
	int line_count = 1;
	char buffer[SIZE];

	std::ifstream source_code;
	source_code.open(file);

	while(source_code.getline(buffer, SIZE)){

		for(int i = 0; i < strlen(buffer); i++){
			reset_state(state);

			std::cout << "State before checking char = " << state << std::endl;
			
			char c = buffer[i];

			if(is_keyword(c)){
				bool keyword = false;
				int current = i + 1;

				for(; current < strlen(buffer); current++){
					std::cout << "State = " << state << std::endl;
					char next_c = buffer[current];
					is_keyword(next_c);

					if(is_accepting()){
						// reset_state(state);
						std::string key(buffer);


						std::cout << "Token (keyword: "
							<< key.substr(i, (current - i) + 1) 
							<< ") found at line " << line_count << std::endl;

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
						// reset_state(state);
						break;
					}
				}

				if(keyword){
					i = current;
					continue;
				}
				else{
					// reset_state(state);
				}
			}
			if (is_identifier(c)){
				std::cout << "Token (identifier: " << c 
					<< ") found at line " << line_count << std::endl;

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
				std::cout << "Token (digit: " << c 
					<< ") found at line " << line_count << std::endl;

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

				std::cout << "Next symbol: " << pos_symb << std::endl;

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
					// tokens.push_back(
					// 	create_token(
					// 		std::string(1, c),
					// 		"space",
					// 		line_count
					// 	)
					// );
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

	std::cout << std::endl;

	for (std::vector<Token>::iterator i = tokens.begin(); i != tokens.end(); ++i){
		std::cout << "Token (" << i->type << ": " << i->desc << ") found at line " << i->line_number << std::endl;
	}

	return tokens;
}