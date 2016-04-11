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
#include "bin/parse.h"
#include "bin/cst.h"
#include "bin/ast.h"
// #include "err.h"

int main(int argc, char const *argv[]){
	
	std::vector<Token> tokens;

	tokens = lets_get_lexical(argv[argc - 1]);

	Tree cst = parse(tokens);

	AST ast;

	ast.build_tree(cst.get_root());

	std::cout << "\n\n\tAST" << std::endl;

	ast.print_tree(ast.get_root(), 0);

	std::cout << "\nCompilation Successful" << std::endl;
	
	return 0;
}