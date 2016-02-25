/*
	File: parse.h
	Name: Juan Bancamper
	Class: CMPT 432
	Date: 10 Feb 2016

	This file declares all the functions and constant data types for parsing.
*/

#ifndef PARSEGAURD_H
#define PARSEGAURD_H

#include "lex.h"
#include <vector>
#include <set>

void parse(std::vector<Token> tokens);

void parse_program();

void parse_block();

void parse_statement_list();

void parse_statement();

void parse_print();

void parse_assingment();

void parse_var_decl();

void parse_while();

void parse_if();

void parse_expr();

void parse_int();

void parse_string();

void parse_bool();

void parse_id();

void parse_char_list();

void match(std::string match);

void match(std::string arr[]);

void match_type(std::string type);

#endif