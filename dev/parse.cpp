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


void match(std::set<std::string> match_set){

	for (std::set<std::string>::iterator i = match_set.begin(); i != match_set.end(); ++i){
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

void parse_char_list(){
	if (!(curr_token -> type).compare("identifier")){
		match_type("identifier");
		parse_char_list();
	}
	// else if (!(curr_token -> type).compare("space")){
	// 	match(" ");
	// 	parse_char_list();
	// }
	else{
		// ε production, do nothing
	}
}

void parse_identifier(){
	match_type("identifier");
}

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

void parse_string(){
	match("\"");
	parse_char_list();
	match("\"");
}

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

void parse_if(){
	match("if");
	parse_bool();
	parse_block();
}

void parse_while(){
	match("while");
	parse_bool();
	parse_block();
}

void parse_var_decl(){
	match(types);
	parse_identifier();
}

void parse_assingment(){
	parse_identifier();
	match("=");
	parse_expr();
}

void parse_print(){
	match("print");
	match("(");
	parse_expr();
	match(")");
}

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

void parse_statement_list(){
	if(!(curr_token -> type).compare("closed_block")){
		// ε production, do nothing
	}
	else{
		parse_statement();
		parse_statement_list();
	}

}

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

void parse_program(){
	parse_block();
	match("$");
}

void parse(std::vector<Token> tokens){
	curr_token = tokens.begin();
	parse_program();
}

