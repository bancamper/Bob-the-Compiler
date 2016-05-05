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
		int heap_ptr, code_ptr, temp_num, scope_level, compilation_temp_vars;
		std::vector<var_addr> static_table;
		std::vector<jump_addr> jump_table;

		void load_acc_const(std::string);
		void load_acc_mem(std::string);
		void store_acc(std::string);
		void add_with_carry(std::string);
		void load_x_const(std::string);
		void load_x_mem(std::string);
		void load_y_const(std::string);
		void load_y_mem(std::string);
		void no_op();
		void break_program();
		void compare_x_to_mem(std::string);
		void branch_not_equal();
		void increment(std::string);
		void system_call();

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
		node *get_parent(node *);
		void generate_int(node *);
		std::string generate_bool(node *);
		void generate_identifier(char, char);
		void generate_string(std::string);
		void generate_print(node *);
		void generate_assignment(node *);
		void generate_if(node *);
		void generate_while(node *);
		void generate_var_decl(node *);
		void generate(node *);
		void print_hex();
	
};

CodeGen hex(AST &);

#endif