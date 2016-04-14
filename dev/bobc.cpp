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

void output(std::string option, std::vector<Token> tokens, Tree &cst, AST &ast){
	if(!option.compare("-trees")){
		std::cout << "\n\n\tCST" << std::endl;
		cst.print_tree(cst.get_root(), 0);

		std::cout << "\n\n\tAST" << std::endl;
		ast.print_tree(ast.get_root(), 0);
	}
	else if(!option.compare("-tokens")){
		for(std::vector<Token>::iterator i=tokens.begin(); i != tokens.end(); ++i){
			std::cout << "Token (" << i->type << ": " << i->desc 
				<< ") found at line " << i->line_number << std::endl;
		}
	}
	else if(!option.compare("-verbose")){
		for(std::vector<Token>::iterator i=tokens.begin(); i != tokens.end(); ++i){
			std::cout << "Token (" << i->type << ": " << i->desc 
				<< ") found at line " << i->line_number << std::endl;
		}

		std::cout << "\n\n\tCST" << std::endl;
		cst.print_tree(cst.get_root(), 0);

		std::cout << "\n\n\tAST" << std::endl;
		ast.print_tree(ast.get_root(), 0);
	}
}

void print_options(){
	std::cout << "\t-trees\t\tOutput CST and AST"
		<< std::endl;

	std::cout << "\t-tokens\t\tOutput all tokens"
		<< std::endl;

	std::cout << "\t-verbose\tOutput everything the compiler is doing"
		<< std::endl;
}

int main(int argc, char const *argv[]){

	if(argc > 3){
		std::cout << "\nUsage: ./bobc [option] <source file>\noptions include:" << std::endl;
		print_options();
		exit(EXIT_FAILURE);
	}

	std::vector<Token> tokens;
	tokens = lets_get_lexical(argv[argc - 1]);

	Tree cst = parse(tokens);

	AST ast;
	ast.build_tree(cst.get_root());


	if(argc == 3){
		output(argv[1], tokens, cst, ast);
	}

	std::cout << "\nCompilation Successful" << std::endl;
	
	return 0;
}