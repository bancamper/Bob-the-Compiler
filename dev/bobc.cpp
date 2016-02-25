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
#include "bin/parse.h"

int main(int argc, char const *argv[]){
	
	std::vector<Token> tokens;

	tokens = lets_get_lexical(argv[argc - 1]);

	parse(tokens);

	std::cout << "\nCompilation Successful" << std::endl;
	
	return 0;
}