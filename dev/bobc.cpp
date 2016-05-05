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
#include "bin/code_gen.h"

extern std::vector<Token>::iterator curr_token;

void print_options(){
	std::cout << "\t-help\t\tDisplay these options"
		<< std::endl;

	std::cout << "\t-symbols\tOutput symbol table created by programs"
		<< std::endl;

	std::cout << "\t-trees\t\tOutput CST and AST"
		<< std::endl;

	std::cout << "\t-tokens\t\tOutput all tokens"
		<< std::endl;

	std::cout << "\t-verbose\tOutput everything the compiler is doing"
		<< std::endl;


}

void output(
	std::string option, 
	std::vector<Token> tokens,
	std::vector<Token>::iterator start_tok,
	std::vector<Token>::iterator end_tok,
	Tree &cst, AST &ast){
	
	if(!option.compare("-trees")){
		std::cout << "\n\n\tCST" << std::endl;
		cst.print_tree(cst.get_root(), 0);

		std::cout << "\n\n\tAST" << std::endl;
		ast.print_tree(ast.get_root(), 0);
	}
	else if(!option.compare("-tokens")){
		for(std::vector<Token>::iterator i = start_tok; i != end_tok; ++i){

			std::cout << "Token (" << i->type << ": " << i->desc 
				<< ") found at line " << i->line_number << std::endl;
		}
	}
	else if(!option.compare("-verbose")){
		for(std::vector<Token>::iterator i = start_tok; i != end_tok; ++i){

			std::cout << "Token (" << i->type << ": " << i->desc 
				<< ") found at line " << i->line_number << std::endl;
		}

		std::cout << "\n\n\tCST" << std::endl;
		cst.print_tree(cst.get_root(), 0);

		std::cout << "\n\n\tAST" << std::endl;
		ast.print_tree(ast.get_root(), 0);

		ast.print_table();
	}
	else if(!option.compare("-symbols")){
		ast.print_table();
	}
	else{
		std::cout << "Unrecognized option" << std::endl;
		std::cout << "\nUsage: ./bobc [option] <source file>\noptions include:" << std::endl;
		print_options();
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char const *argv[]){
	if(argc > 3 || argc < 2 || !std::string(argv[1]).compare("-help")) {
		std::cout << "\nUsage: ./bobc [option] <source file>\noptions include:" << std::endl;
		print_options();
		exit(EXIT_FAILURE);
	}

	std::vector<Token> tokens;
	tokens = lets_get_lexical(argv[argc - 1]);
	std::vector<Token>::iterator token_ptr = tokens.begin();

	int program_counter = 0;

	while(token_ptr != tokens.end()){
		std::cout << "\n-------------------------------------------------------\n"
			<< "Program #" << program_counter++
			<< "\n-------------------------------------------------------\n"
			<< std::endl;
		Tree cst = parse(token_ptr);
		AST ast = semantisize(cst);

		ast.print_tree(ast.get_root(), 0);
		std::cout << std::endl;

		CodeGen cg = hex(ast);

		if(argc == 3){
			output(argv[1], tokens, token_ptr, curr_token, cst, ast);
		}
		token_ptr = curr_token;
	}

	std::cout << "\nCompilation Successful" << std::endl;
	
	return 0;
}