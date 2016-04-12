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
#include "cst.h"
#include <vector>
#include <set>

Tree parse(std::vector<Token> tokens);

void parse_program(Tree &cst);

void parse_block(Tree &cst);

void parse_statement_list(Tree &cst);

void parse_statement(Tree &cst);

void parse_print(Tree &cst);

void parse_assingment(Tree &cst);

void parse_var_decl(Tree &cst);

void parse_while(Tree &cst);

void parse_if(Tree &cst);

void parse_expr(Tree &cst);

void parse_int(Tree &cst);

void parse_string(Tree &cst);

void parse_bool(Tree &cst);

void parse_id(Tree &cst);

void parse_char_list(Tree &cst);

void match(std::string, Tree &);

void match(std::set<std::string>, Tree &);

void match_type(std::string, Tree &);

#endif