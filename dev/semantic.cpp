/*
	File: semantic.cpp
	Name: Juan Bancamper
	Class: CMPT 432
	Date: 8 Apr 2016

	This file defines all the functions for classes Abstract
	Syntax Tree and Symbol Table.
*/

#include "bin/ast.h"
#include "bin/cst.h"
#include "bin/symbol_table.h"
#include <regex>
#include <iostream>


Symbol_Table::Symbol_Table(){
	root = NULL;
	current = NULL;
}

Symbol_Table::~Symbol_Table(){
	// destroy_tree();
}

scope *Symbol_Table::get_current(){
	return current;
}

bool Symbol_Table::search(char c, scope *cur){
	if(cur){
		std::string type = cur -> symbols[c];
		if(!type.compare("")){
			return search(c, get_parent(cur));
		}
		else{
			return true;
		}
	}

	return false;
}

void Symbol_Table::insert(char c, std::string type){
	current -> symbols.insert(std::pair<char, std::string>(c, type));
}

scope *Symbol_Table::get_parent(scope *cur){
	if(cur -> older_sibling){
		return get_parent(cur -> older_sibling);
	}

	return cur -> parent;
}

scope *Symbol_Table::get_youngest_sibling(scope *cur){
	if(cur -> younger_sibling){
		return get_youngest_sibling(cur -> younger_sibling);
	}

	return cur;
}

void Symbol_Table::print_tree(scope *cur, int level){
	if(cur){
		for (int i = 0; i < level; i++){
		
			std::cout << "|";
			std::cout << "   ";
		}

		std::cout << cur -> scope_level << std::endl;
		print_tree(cur -> child, level + 1);
		print_tree(cur -> younger_sibling, level);
	}
}

void Symbol_Table::add_scope(int level){
	scope *temp = new scope;
	temp -> scope_level = level;
	temp -> parent = NULL;
	temp -> child = NULL;
	temp -> older_sibling = NULL;
	temp -> younger_sibling = NULL;

	if(!root){
		root = new scope;
		root = temp;
	}
	else{
		if(!current -> child){
			// std::cout << "Parent Scope: " << current << " -- Child: " << temp << std::endl;
			current -> child = new scope;
			current -> child = temp; 

			temp -> parent = new scope;
			temp -> parent = current;
		}
		else{
			scope *n = new scope;
			n = get_youngest_sibling(current -> child);

			n -> younger_sibling = new scope;
			n -> younger_sibling = temp;

			temp -> older_sibling = new scope;
			temp -> older_sibling = n;

		}
	}

	current = new scope;
	current = temp;
	// std::cout << "Current Scope Level: " << current -> scope_level << std::endl;
}

void Symbol_Table::integrity(char c, std::string type){
	scope *n = new scope;
	n = current;

	while(n){
		std::string map_type = n -> symbols[c];
		// std::cout << "Current Type: " << map_type << std::endl;
		if(!map_type.compare(type)){
			return;
		}
		else if(map_type.compare(type) && !map_type.empty()){
			std::cout << "\nType Mismatch Error:  Cannot cast " << type << " to a " << map_type << ", on line..." << std::endl;
			exit(EXIT_FAILURE);
		}

		n = get_parent(n);
	}

	std::cout << "\nUndefined variable: " << c << ", on line..." << std::endl;
	exit(EXIT_FAILURE);

}

void Symbol_Table::kill_all_children(){
	scope *n = new scope;
	n = get_parent(current);
	if(n){
		current = n;
	}
	// std::cout << "Current Parent Scope: " << current << std::endl;
}

AST::AST(){
	root = NULL;
	current = NULL;
	level_count = 0;
}

AST::~AST(){

}

node *AST::get_root(){
	return root;
}

node *AST::get_parent(node *cur){
	// std::cout << "\tCurrent: " << cur << std::endl;

	if(cur -> older_sibling){

		return get_parent(cur -> older_sibling);
	}

	return cur -> parent;
}

node *AST::get_youngest_sibling(node *cur){
	if(cur -> younger_sibling){
		return get_youngest_sibling(cur -> younger_sibling);
	}

	return cur;
}

void AST::add_branch_node(std::string branch){
	node *temp = new node;
	temp -> data = branch;
	temp -> parent = NULL;
	temp -> child = NULL;
	temp -> older_sibling = NULL;
	temp -> younger_sibling = NULL;

	if(!root){
		root = new node;
		root = temp;
	}
	else{
		if(!current -> child){
			// std::cout << "Parent: " << current -> data << " -- Child: " << temp -> data << std::endl;
			current -> child = new node;
			current -> child = temp; 

			temp -> parent = new node;
			temp -> parent = current;
		}
		else{
			node *n = new node;
			n = get_youngest_sibling(current -> child);

			// std::cout << "Current Youngest: " << n -> data << std::endl;

			n -> younger_sibling = new node;
			n -> younger_sibling = temp;

			temp -> older_sibling = new node;
			temp -> older_sibling = n;

			// std::cout << "Older Sibling: " << temp -> older_sibling -> data << " -- Younger Sibling: " 
				// << n -> younger_sibling -> data << std::endl;

		}
	}

	current = new node;
	current = temp;

	// std::cout << "Current Pointed Node: " << current -> data << std::endl << std::endl;
}

void AST::add_leaf_node(std::string leaf){
	node *temp = new node;
	temp -> data = leaf;
	temp -> parent = NULL;
	temp -> child = NULL;
	temp -> older_sibling = NULL;
	temp -> younger_sibling = NULL;

	if(!root){
		root = new node;
		root = temp;
	}
	else{
		if(!current -> child){
			// std::cout << "Parent: " << current -> data << " -- Child: " << temp -> data << std::endl;
			current -> child = new node;
			current -> child = temp;

			temp -> parent = new node;
			temp -> parent = current;
		}
		else{
			node *n = new node;
			n = get_youngest_sibling(current -> child);

			// std::cout << "Current Youngest: " << n -> data << std::endl;

			n -> younger_sibling = new node;
			n -> younger_sibling = temp;

			temp -> older_sibling = new node;
			temp -> older_sibling = n;

			// std::cout << "Older Sibling: " << temp -> older_sibling -> data << " -- Younger Sibling: " 
				// << n -> younger_sibling -> data << std::endl;
		}
	}

	// std::cout << "Current Pointed Node: " << current -> data << std::endl << std::endl;
}

void AST::kill_all_children(){
	node *n = new node;


	// std::cout << "In get_parent()" << std::endl;
	n = get_parent(current);

	// std::cout << "\nCurrent Parent: " << n << std::endl;

	if(n){
		current = n;
	}

}

void AST::print_tree(node *cur, int level){
	if(cur){
		for (int i = 0; i < level; i++){
		
			std::cout << "|";
			std::cout << "   ";
		}

		std::cout << cur -> data << std::endl;
		print_tree(cur -> child, level + 1);
		print_tree(cur -> younger_sibling, level);
	}
}

void AST::traverse_block(){
	add_branch_node("block");
	symbol_table.add_scope(level_count++);
}

void AST::traverse_var_decl(node *cur){
	add_branch_node(cur -> data);

	node *child = new node;
	child = cur -> child;

	std::string type = child -> data, 
		id = child -> younger_sibling -> child -> data;

	add_leaf_node(type);
	add_leaf_node(id);
	symbol_table.insert(id[0], type);
	kill_all_children();
}

void AST::traverse_int(node *cur){

	if(cur -> younger_sibling){
		add_branch_node("add");
		add_leaf_node(cur -> data);
		traverse_expr(cur -> younger_sibling -> younger_sibling -> child);
		kill_all_children();
	}
	else{

		add_leaf_node(cur -> data);
	}

}

void AST::traverse_string(node *cur){
	node *n = new node;
	n = cur;
	std::string char_seq = "\"";

	while(n -> younger_sibling -> child){
		char_seq += n -> younger_sibling -> child -> data;
		n = n -> younger_sibling -> child;
	}
	char_seq += "\"";
	add_leaf_node(char_seq);

}

void AST::traverse_bool(node *cur){
	if(cur -> younger_sibling){
		node *temp = new node;
		temp = cur -> younger_sibling;
		add_branch_node(temp -> younger_sibling -> data);
		traverse_expr(temp -> child);
		traverse_expr(temp -> younger_sibling -> younger_sibling -> child);
		kill_all_children();
	}
	else{
		add_leaf_node(cur -> data);
	}
}

void AST::traverse_id(node *cur){
	if(symbol_table.search(cur -> data[0], symbol_table.get_current())){
		add_leaf_node(cur -> data);
	}
}

void AST::traverse_expr(node *cur){
	if(cur){
		if(!cur -> data.compare("int")){
			traverse_int(cur -> child);
		}
		else if(!cur -> data.compare("string")){
			traverse_string(cur -> child);
		}
		else if(!cur -> data.compare("boolean")){
			traverse_bool(cur -> child);
		}
		else if(!cur -> data.compare("identifier")){
			traverse_id(cur -> child);
		}
	}
}

void AST::traverse_assignment(node *cur){
	add_branch_node(cur -> data);

	node *child = new node;
	child = cur -> child;
	std::string var = child -> child -> data;
	if(symbol_table.search(var[0], symbol_table.get_current())) {
		add_leaf_node(var);
	}
	else{
		std::cout << "\nUndefined variable: " << var << ", on line " << std::endl;
		exit(EXIT_FAILURE);
	}

	node *expr = new node;
	expr = get_youngest_sibling(child);

	symbol_table.integrity(var[0], expr -> child -> data);

	traverse_expr(expr -> child);

	kill_all_children();

}

void AST::traverse_while(node *cur){
	add_branch_node("while");
	traverse_bool(cur -> child -> younger_sibling -> child);
}

void AST::traverse_if(node *cur){
	add_branch_node("if");
	traverse_bool(cur -> child -> younger_sibling -> child);
}

void AST::traverse_print(node *cur){
	add_branch_node("print");
	traverse_expr(cur -> younger_sibling -> younger_sibling -> child);
}

void AST::build_tree(node *cur){
	if(cur){

		if(!cur -> data.compare("block")){
			traverse_block();
		}
		else if(!cur -> data.compare("var_decl")){
			traverse_var_decl(cur);
		}
		else if(!cur -> data.compare("assignment")){
			traverse_assignment(cur);
		}
		else if(!cur -> data.compare("while_stmt")){
			traverse_while(cur);
		}
		else if(!cur -> data.compare("if_stmt")){
			traverse_if(cur);
		}
		else if(!cur -> data.compare("}")){
			symbol_table.kill_all_children();
			level_count--;
			node *temp = new node;
			temp = get_parent(current);

			if(temp != root && temp){
				kill_all_children();
				kill_all_children();
			}
			else{
				kill_all_children();
			}
		}
		else if(!cur -> data.compare("print_stmt")){
			traverse_print(cur -> child);
		}

		build_tree(cur -> child);
		build_tree(cur -> younger_sibling);
	}
	
	
}




