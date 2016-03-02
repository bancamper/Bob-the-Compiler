/*
	File: lex.h
	Name: Juan Bancamper
	Class: CMPT 432
	Date: 10 Feb 2016

	This file declares all the functions and constant data types throughout the
	lexical analysis.
*/

#ifndef LEXICALGUARD_H
#define LEXICALGUARD_H

struct Token{	
	std::string desc, type;
	int line_number;
};
//type: keyword, identifier, symbol, digit
//desc: 

struct Token create_token(std::string desc, std::string type, int line_number);

bool is_accepting();

bool is_error();

bool is_keyword(char c);

bool is_identifier(char c);

bool is_symbol(char c);

bool is_digit(char c);

std::vector<Token> lets_get_lexical(std::string line_of_code);

#endif