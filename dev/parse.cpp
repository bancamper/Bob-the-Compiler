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
#include "bin/cst.h"
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
void match(std::set<std::string> match_set, Tree &cst){

	for(std::set<std::string>::iterator i = match_set.begin(); i != match_set.end(); ++i){
		std::string matched = *i;

		// std::cout << "\nTrying to match: " << matched << std::endl;
		// std::cout << "Current Token: " << curr_token -> desc << std::endl;

		if(!matched.compare(curr_token -> desc)){
			// std::cout << "Matched token (" << curr_token -> type << ": " 
				// << curr_token -> desc << ")" << std::endl;
			cst.add_leaf_node(matched, curr_token -> line_number);
			++curr_token;

			return;
		}
	}

	std::cout << "Parse Error on line " << curr_token -> line_number
		<< ".\nExpected ";

	for(std::set<std::string>::iterator i = match_set.begin(); i != match_set.end(); ++i){
		std::cout << *i << ", ";
	}

	std::cout << "found " << curr_token -> desc 
		<< ". \n\nCompilation unsuccessful" << std::endl;
	exit(EXIT_FAILURE);

}

/*
match

Same function as above, just uses one character to match instead of a set

Parameters: character
	one letter string to match the current token

Return: none
*/
void match(std::string character, Tree &cst){
	// std::cout << "\nTrying to match: " << character << std::endl;
	// std::cout << "Current Token: " << curr_token -> desc << std::endl;

	if(!character.compare(curr_token -> desc)){
		// std::cout << "Matched token (" << curr_token -> type << ": " 
			// << curr_token -> desc << ")" << std::endl;
		++curr_token;

		cst.add_leaf_node(character, curr_token -> line_number);
	}
	else{
		std::cout << "Parse Error on line " << curr_token -> line_number
			<< ".\nExpected " << character << ", found " << curr_token -> desc
			<< ".\n\nCompilation unsuccessful" << std::endl;
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
void match_type(std::string token_type, Tree &cst){
	// std::cout << "\nTrying to match: " << token_type << std::endl;
	// std::cout << "Current Token: " << curr_token -> type << std::endl;

	if(!token_type.compare(curr_token -> type)){
		// std::cout << "Matched token (" << curr_token -> type << ": " 
			// << curr_token -> desc << ")" << std::endl;
		cst.add_leaf_node(curr_token -> desc, curr_token -> line_number);
		++curr_token;
	}
	else{
		std::cout << "Parse Error on line " << curr_token -> line_number
			<< ".\nExpected type" << token_type << ", found " <<
			curr_token -> type << ": " << curr_token -> desc
			<< ".\n\nCompilation unsuccesful" << std::endl;
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
void parse_char_list(Tree &cst){
	cst.add_branch_node("char_list");
	if (!(curr_token -> type).compare("identifier")){
		match_type("identifier", cst);
		parse_char_list(cst);
	}
	else if(!(curr_token -> type).compare("keyword")){
		match_type("keyword", cst);
		parse_char_list(cst);
	}
	else if (!(curr_token -> type).compare("space")){
		match(" ", cst);
		parse_char_list(cst);
	}
	else{
		// ε production, do nothing
	}
	cst.kill_all_children();
}

/*
parse_identifier

Matches a valid variable name

Parameters: none

Return: none
*/
void parse_identifier(Tree &cst){
	cst.add_branch_node("identifier");
	match_type("identifier", cst);
	cst.kill_all_children();
}

/*
parse_bool

Matches a valid boolean expression by looking ahead one token

Parameters: none

Return: none
*/
void parse_bool(Tree &cst){
	cst.add_branch_node("boolean");
	if(!(curr_token -> type).compare("open_paren")){
		match_type("open_paren", cst);
		parse_expr(cst);
		match(boolop, cst);
		parse_expr(cst);
		match(")", cst);
	}
	else{
		match(boolval, cst);
	}
	cst.kill_all_children();
}

/*
parse_string

Matches a valid string expression

Parameters: none

Return: none
*/
void parse_string(Tree &cst){
	cst.add_branch_node("string");
	match("\"", cst);
	parse_char_list(cst);
	match("\"", cst);
	cst.kill_all_children();
}

/*
parse_int

Matches a valid integer expression by looking ahead one token

Parameters: none

Return: none
*/
void parse_int(Tree &cst){
	cst.add_branch_node("int");

	std::vector<Token>::iterator next_token = std::next(curr_token, 1);

	if(!(next_token -> type).compare("plus")){
		match_type("digit", cst);
		match("+", cst);
		parse_expr(cst);
	}
	else{
		match_type("digit", cst);
	}

	cst.kill_all_children();
}

/*
parse_expr

Checks the current token to call the correct parsing function

Parameters: none

Return: none
*/
void parse_expr(Tree &cst){
	cst.add_branch_node("expr");
	if(!(curr_token -> type).compare("digit")){
		parse_int(cst);
	}
	else if(!(curr_token -> type).compare("quote")){
		parse_string(cst);
	}
	else if(!(curr_token -> type).compare("open_paren") ||
		!(curr_token -> type).compare("keyword")){
		parse_bool(cst);
	}
	else if(!(curr_token -> type).compare("identifier")){
		parse_identifier(cst);
	}
	else{
		std::cout << "Parse Error on line " << curr_token -> line_number
			<< ".\nExpected digit, identifier, open_paren, bool_expr, found " <<
			curr_token -> type << ": " << curr_token -> desc
			<< ".\n\nCompilation unsuccessful" << std::endl;
		exit(EXIT_FAILURE);
	}
	cst.kill_all_children();
}

/*
parse_if

Matches a valid if conditional

Parameters: none

Return: none
*/
void parse_if(Tree &cst){
	cst.add_branch_node("if_stmt");
	match("if", cst);
	parse_bool(cst);
	parse_block(cst);
	cst.kill_all_children();
}

/*
parse_while

Matches a valid while loop declaration

Parameters: none

Return: none
*/
void parse_while(Tree &cst){
	cst.add_branch_node("while_stmt");
	match("while", cst);
	parse_bool(cst);
	parse_block(cst);
	cst.kill_all_children();
}

/*
parse_var_decl

Matches a valid variable declaration

Parameters: none

Return: none
*/
void parse_var_decl(Tree &cst){
	cst.add_branch_node("var_decl");
	match(types, cst);
	parse_identifier(cst);
	cst.kill_all_children();
}

/*
parse_assignment

Matches a valid assignment

Parameters: none

Return: none
*/
void parse_assingment(Tree &cst){
	cst.add_branch_node("assignment");
	parse_identifier(cst);
	match("=", cst);
	parse_expr(cst);
	cst.kill_all_children();
}

/*
parse_print

Matches the print keyword and a valid expression

Parameters: none

Return: none
*/
void parse_print(Tree &cst){
	cst.add_branch_node("print_stmt");
	match("print", cst);
	match("(", cst);
	parse_expr(cst);
	match(")", cst);
	cst.kill_all_children();
}

/*
parse_statement

Checks the current token to be able to parse the correct statement

Parameters: none

Return: none
*/
void parse_statement(Tree &cst){
	cst.add_branch_node("statement");
	if(!(curr_token -> desc).compare("print")){
		parse_print(cst);
	}
	else if(!(curr_token -> type).compare("identifier")){
		parse_assingment(cst);
	}
	else if(types.find(curr_token -> desc) != types.end()){
		parse_var_decl(cst);
	}
	else if(!(curr_token -> desc).compare("while")){
		parse_while(cst);
	}
	else if(!(curr_token -> desc).compare("if")){
		parse_if(cst);
	}
	else if(!(curr_token -> type).compare("open_block")){
		parse_block(cst);
	}
	else{
		std::cout << "Parse Error on line " << curr_token -> line_number
			<< ".\nExpected statement, found " << curr_token -> desc
			<< "\nStatements begin with:\n\tprint\n\tidentifier -- [a-z]\n\t"
			<< "int, string, boolean\n\twhile\n\tif\n\t{"
			<< "\n\nCompilation unsuccessful" << std::endl;
		exit(EXIT_FAILURE);
	}
	cst.kill_all_children();
}

/*
parse_statement_list

Recursively parses a valid statement

Parameters: none

Return: none
*/
void parse_statement_list(Tree &cst){
	cst.add_branch_node("statement_list");
	if(!(curr_token -> type).compare("closed_block")){
		// ε production, do nothing
	}
	else{
		parse_statement(cst);
		parse_statement_list(cst);
	}
	cst.kill_all_children();
}

/*
parse_block

Matches a block of code

Parameters: none

Return: none
*/
void parse_block(Tree &cst){
	cst.add_branch_node("block");
	if(!(curr_token -> type).compare("open_block")) {
		match("{", cst);
		parse_statement_list(cst);
		match("}", cst);
	}
	else{
		std::cout << "Invalid block declaration on line " << curr_token -> line_number << std::endl;
		exit(EXIT_FAILURE);
	}
	cst.kill_all_children();
}

/*
parse_program

Parses a block and matches end of program

Parameters: none

Return: none
*/
void parse_program(Tree &cst){
	cst.add_branch_node("program");
	parse_block(cst);

	if(!curr_token -> desc.compare("$")){
		match("$", cst);
		std:: cout << "No Parse Warnings" << std::endl;
	}
	else{
		std::cout << "\nParse Warning(s):\n\tMissing end_of_program - $ on line " 
			<< (--curr_token) -> line_number << std::endl << std::endl;
		cst.add_leaf_node("$", curr_token -> line_number);
		curr_token++;
	}
}

/*
parse

Starts the parsing process by creating a pointer to the tokens

Parameters: tokens
	a vector containg the tokens produced by lex.cpp

Return: none
*/
Tree parse(std::vector<Token>::iterator tokens){
	curr_token = tokens;
	int program_coutner = 0;
	
	// while(curr_token != tokens.end()){
		// std::cout << "\n-------------------------------------------------------\n"
		// 	<< "Program #" << ++program_coutner
		// 	<< "\n-------------------------------------------------------\n"
		// 	<< std::endl;
		Tree cst;
		parse_program(cst);

		// std::cout << "\n\n\tCST" << std::endl;
		// cst.print_tree(cst.get_root(), 0);
		return cst;
	// }
}
