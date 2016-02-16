/*
	File: lex.h
	Name: Juan Bancamper
	Class: CMPT 432
	Date: 10 Feb 2016

	This file declares all the functions and constant data types throughout the
	lexical analysis
*/

#ifndef LEXICALGUARD_H
#define LEXICALGUARD_H

struct Token
{
	char kind[], type[];
	int line_number;
} token;

struct Token create_token();

bool is_keyword();

bool is_id();

bool is_symbol();

bool is_digit();

void lets_get_lexical(int line_count, char line_of_code[]);

#endif