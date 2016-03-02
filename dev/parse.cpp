/*
	File: parse.cpp
	Name: Juan Bancamper
	Class: CMPT 432
	Date: 20 Feb 2016

	This file includes all the function definitions and data types declared in
	the parser.
*/

#include <iostream>
#include <array>
#include <regex>
#include <stdlib.h>
#include <vector>
#include "bin/lex.h"
#include "bin/parse.h"

std::vector<Token>::iterator curr_token;

std::string t[] = {"int", "string", "boolean"};
std::set<std::string> types(t, t + 3);

std::string bo[] = {"==", "!="};
std::set<std::string> boolop(bo, bo + 2);

std::string bv[] = {"false", "true"};
std::set<std::string> boolval(bv, bv + 2);

/*
match

This function iterates over a set to match the current token to a certain
terminal

Parameters: match_set
	a set containing all the possible matches for a token

Return: none
*/
void match(std::set<std::string> match_set){

	for(std::set<std::string>::iterator i = match_set.begin(); i != match_set.end(); ++i){
		std::string matched = *i;

		std::cout << "\nTrying to match: " << matched << std::endl;
		std::cout << "Current Token: " << curr_token -> desc << std::endl;

		if(!matched.compare(curr_token -> desc)){
			std::cout << "Matched token (" << curr_token -> type << ": " 
				<< curr_token -> desc << ")" << std::endl;
			++curr_token;
			return;
		}
	}

	std::cout << "Parse Error" << std::endl;
	exit(EXIT_FAILURE);

}

/*
match

Same function as above, just uses one character to match instead of a set

Parameters: character
	one letter string to match the current token

Return: none
*/
void match(std::string character){
	std::cout << "\nTrying to match: " << character << std::endl;
	std::cout << "Current Token: " << curr_token -> desc << std::endl;

	if(!character.compare(curr_token -> desc)){
		std::cout << "Matched token (" << curr_token -> type << ": " 
			<< curr_token -> desc << ")" << std::endl;
		++curr_token;
	}
	else{
		std::cout << "Parse Error" << std::endl;
		exit(EXIT_FAILURE);
	}
}

/*
match_type

Same as the function above, different type of parameter

Parameters: token_type
	a string of the type of token to be matched

Return: none
*/
void match_type(std::string token_type){
	std::cout << "\nTrying to match: " << token_type << std::endl;
	std::cout << "Current Token: " << curr_token -> type << std::endl;

	if(!token_type.compare(curr_token -> type)){
		std::cout << "Matched token (" << curr_token -> type << ": " 
			<< curr_token -> desc << ")" << std::endl;
		++curr_token;
	}
	else{
		std::cout << "Parse Error" << std::endl;
		exit(EXIT_FAILURE);
	}
}

/*
parse_char_list

Checks the current token for a char and recursively iterates the next tokens
for chars

Parameters: none

Return: none
*/
void parse_char_list(){
	if (!(curr_token -> type).compare("identifier")){
		match_type("identifier");
		parse_char_list();
	}
	else if (!(curr_token -> type).compare("space")){
		match(" ");
		parse_char_list();
	}
	else{
		// ε production, do nothing
	}
}

/*
parse_identifier

Matches a valid variable name

Parameters: none

Return: none
*/
void parse_identifier(){
	match_type("identifier");
}

/*
parse_bool

Matches a valid boolean expression by looking ahead one token

Parameters: none

Return: none
*/
void parse_bool(){
	if(!(curr_token -> type).compare("open_paren")){
		match_type("open_paren");
		parse_expr();
		match(boolop);
		parse_expr();
		match(")");
	}
	else{
		match(boolval);
	}
}

/*
parse_string

Matches a valid string expression

Parameters: none

Return: none
*/
void parse_string(){
	match("\"");
	parse_char_list();
	match("\"");
}

/*
parse_int

Matches a valid integer expression by looking ahead one token

Parameters: none

Return: none
*/
void parse_int(){
	std::vector<Token>::iterator next_token = std::next(curr_token, 1);

	if(!(next_token -> type).compare("plus")){
		match_type("digit");
		match("+");
		parse_expr();
	}
	else{
		match_type("digit");
	}
}

/*
parse_expr

Checks the current token to call the correct parsing function

Parameters: none

Return: none
*/
void parse_expr(){
	if(!(curr_token -> type).compare("digit")){
		parse_int();
	}
	else if(!(curr_token -> type).compare("quote")){
		parse_string();
	}
	else if(!(curr_token -> type).compare("open_paren")){
		parse_bool();
	}
	else if(!(curr_token -> type).compare("identifier")){
		parse_identifier();
	}
	else{
		std::cout << "Parse Error" << std::endl;
		exit(EXIT_FAILURE);
	}
}

/*
parse_if

Matches a valid if conditional

Parameters: none

Return: none
*/
void parse_if(){
	match("if");
	parse_bool();
	parse_block();
}

/*
parse_while

Matches a valid while loop declaration

Parameters: none

Return: none
*/
void parse_while(){
	match("while");
	parse_bool();
	parse_block();
}

/*
parse_var_decl

Matches a valid variable declaration

Parameters: none

Return: none
*/
void parse_var_decl(){
	match(types);
	parse_identifier();
}

/*
parse_assignment

Matches a valid assignment

Parameters: none

Return: none
*/
void parse_assingment(){
	parse_identifier();
	match("=");
	parse_expr();
}

/*
parse_print

Matches the print keyword and a valid expression

Parameters: none

Return: none
*/
void parse_print(){
	match("print");
	match("(");
	parse_expr();
	match(")");
}

/*
parse_statement

Checks the current token to be able to parse the correct statement

Parameters: none

Return: none
*/
void parse_statement(){
	if(!(curr_token -> desc).compare("print")){
		parse_print();
	}
	else if(!(curr_token -> type).compare("identifier")){
		parse_assingment();
	}
	else if(types.find(curr_token -> desc) != types.end()){
		parse_var_decl();
	}
	else if(!(curr_token -> desc).compare("while")){
		parse_while();
	}
	else if(!(curr_token -> desc).compare("if")){
		parse_if();
	}
	else if(!(curr_token -> type).compare("open_block")){
		parse_block();
	}
	else{
		std::cout << "Parse Error" << std::endl;
		exit(EXIT_FAILURE);
	}
}

/*
parse_statement_list

Recursively parses a valid statement

Parameters: none

Return: none
*/
void parse_statement_list(){
	if(!(curr_token -> type).compare("closed_block")){
		// ε production, do nothing
	}
	else{
		parse_statement();
		parse_statement_list();
	}

}

/*
parse_block

Matches a block of code

Parameters: none

Return: none
*/
void parse_block(){
	if(!(curr_token -> type).compare("open_block")) {
		match("{");
		parse_statement_list();
		match("}");
	}
	else{
		std::cout << "Parse Error" << std::endl;
		exit(EXIT_FAILURE);
	}
}

/*
parse_program

Parses a block and matches end of program

Parameters: none

Return: none
*/
void parse_program(){
	parse_block();
	match("$");
}

/*
parse

Starts the parsing process by creating a pointer to the tokens

Parameters: tokens
	a vector containg the tokens produced by lex.cpp

Return: none
*/
void parse(std::vector<Token> tokens){
	curr_token = tokens.begin();
	parse_program();
}

