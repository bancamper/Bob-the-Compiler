/*
	File: symbol_table.h
	Name: Juan Bancamper
	Class: CMPT 432
	Date: 8 Apr 2016

	This file declares all the functions and classes for a Symbol Table.
*/

#include <map>

#ifndef SYMBOL_TABLEGUARD_H
#define SYMBOL_TABLEGUARD_H

struct scope{
	int scope_level;
	std::map<char, std::string> symbols;
	scope *parent;
	scope *child;
	scope *older_sibling;
	scope *younger_sibling;
};

class Symbol_Table{
	private:
		scope *root, *current;
		scope *get_parent(scope *);
		scope *get_youngest_sibling(scope *);


	public:
		Symbol_Table();
		~Symbol_Table();
		bool search(char, scope *);
		void insert(char, std::string);
		void add_scope(int);
		void kill_all_children();
		scope *get_current();
		void integrity(char, std::string);
		void print_tree(scope *, int);
	
};



#endif