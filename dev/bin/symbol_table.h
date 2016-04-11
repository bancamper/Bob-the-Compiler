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
		bool search(char);
		void insert(char, std::string);


	public:
		Symbol_Table();
		~Symbol_Table();
	
};



#endif