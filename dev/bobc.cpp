/*
	File: bobc.cpp
	Name: Juan Bancamper
	Class: CMPT 432
	Date: 17 Feb 2016

	This file recieves a plain text file via the command line
	and compiles the programming language
 */ 
#include <iostream>
#include <map>
#include <vector>
#include "bin/lex.h"
// #include "err.h"
// #include "bin/parse.h"

int main(int argc, char const *argv[]){
	
	std::vector<Token> tokens;

	// std::map<std::string, std::string> symbols;
	// symbols["{"] = "open_block";
	// symbols["}"] = "closed_block";
	// symbols["("] = "open_ paren";
	// symbols[")"] = "closed_paren";
	// symbols["="] = "assign";
	// symbols["=="] = "equals";
	// symbols["!="] = "not_equals";
	// symbols["+"] = "plus";
	// symbols["$"] = "end_of_program";
	// symbols["\""] = "quote";

	// std::cout << symbols.end() -> first << std::endl;

	tokens = lets_get_lexical(argv[argc - 1]);

	// parse(tokens);
	
	return 0;
}