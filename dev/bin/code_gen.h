/*
	File: symbol_table.h
	Name: Juan Bancamper
	Class: CMPT 432
	Date: 25 Apr 2016

	This file declares all the functions and structures used for 
	Code Generation.
*/

#include <vector>
#include "cst.h"
#include "ast.h"

#ifndef CODE_GENGUARD_H
#define CODE_GENGUARD_H

enum boolvals{
	false_val, true_val
};

struct var_addr{
	std::string temp;
	char variable;
	std::string type;
	int scope, offset;
};

struct jump_addr{
	std::string temp;
	int offset;
};

class CodeGen{
	private:
		std::string bytes[256];
		std::string load_int;
		int heap_ptr, code_ptr, temp_num, scope_level;
		std::vector<var_addr> static_table;
		std::vector<jump_addr> jump_table;

	public:
		CodeGen();
		~CodeGen();
		void fill();
		void back_patch();
		void print_tables();
		struct var_addr find_var(char);
		struct var_addr find_var(std::string);
		struct jump_addr find_jump(std::string);
		void generate_primitive(std::string, node *);
		int generate_int(node *);
		void generate_bool(node *);
		void generate_identifier(char, char);
		void generate_string(node *, std::string);
		void generate_print(node *);
		void generate_assignment(node *);
		void generate_var_decl(node *);
		void generate(node *);
		void print_hex();
	
};

CodeGen hex(AST &);

#endif