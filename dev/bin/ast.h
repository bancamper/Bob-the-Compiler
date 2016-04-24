/*
	File: ast.h
	Name: Juan Bancamper
	Class: CMPT 432
	Date: 8 Apr 2016

	This file declares all the functions and classes for a Abstract
	Syntax Tree.
*/

#include "cst.h"
#include "symbol_table.h"

#ifndef ASTGUARD_H
#define ASTGUARD_H

class AST{
	private:
		Symbol_Table symbol_table;
		int level_count;
		node *root, *current;
		void add_branch_node(std::string);
		void add_leaf_node(std::string, int);
		void kill_all_children();
		node *get_youngest_sibling(node *);
		node *get_parent(node *);

		void traverse_block();
		void traverse_var_decl(node *);
		void traverse_assignment(node *);
		void traverse_expr(node *);
		void traverse_int(node *);
		void traverse_string(node *);
		void traverse_bool(node *);
		void traverse_print(node *);
		void traverse_while(node *);
		void traverse_if(node *);
		void traverse_id(node *);

		void check_assignment(node *);
		void check_if(node *);
		void check_while(node *);
		void check_int(node *);
		void check_string(node *);
		std::string check_bool(node *);
		void check_print(node *);

	public:
		AST();
		~AST();

		void print_tree(node *, int);
		void scope_check();
		void print_table();
		void type_check(node *);
		void build_tree(node *);

		node *get_root();
};

AST semantisize(Tree &);

#endif