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

AST::AST(){
	root = NULL;
	current = NULL;
}

AST::~AST(){

}

node *AST::get_root(){
	return root;
}

node *AST::get_parent(node *cur){
	std::cout << "\tCurrent: " << cur -> data << std::endl;

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
			std::cout << "Parent: " << current -> data << " -- Child: " << temp -> data << std::endl;
			current -> child = new node;
			current -> child = temp; 

			temp -> parent = new node;
			temp -> parent = current;
		}
		else{
			node *n = new node;
			n = get_youngest_sibling(current -> child);

			std::cout << "Current Youngest: " << n -> data << std::endl;

			n -> younger_sibling = new node;
			n -> younger_sibling = temp;

			temp -> older_sibling = new node;
			temp -> older_sibling = n;

			std::cout << "Older Sibling: " << temp -> older_sibling -> data << " -- Younger Sibling: " 
				<< n -> younger_sibling -> data << std::endl;

		}
	}

	current = new node;
	current = temp;

	std::cout << "Current Pointed Node: " << current -> data << std::endl << std::endl;
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
			std::cout << "Parent: " << current -> data << " -- Child: " << temp -> data << std::endl;
			current -> child = new node;
			current -> child = temp;

			temp -> parent = new node;
			temp -> parent = current;
		}
		else{
			node *n = new node;
			n = get_youngest_sibling(current -> child);

			std::cout << "Current Youngest: " << n -> data << std::endl;

			n -> younger_sibling = new node;
			n -> younger_sibling = temp;

			temp -> older_sibling = new node;
			temp -> older_sibling = n;

			std::cout << "Older Sibling: " << temp -> older_sibling -> data << " -- Younger Sibling: " 
				<< n -> younger_sibling -> data << std::endl;
		}
	}

	std::cout << "Current Pointed Node: " << current -> data << std::endl << std::endl;
}

void AST::kill_all_children(){
	node *n = new node;

	std::cout << "In get_parent()" << std::endl;
	n = get_parent(current);

	std::cout << "\nCurrent Parent: " << n -> data << std::endl;

	current = n;
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
}

void AST::traverse_var_decl(node *cur){
	add_branch_node(cur -> data);

	node *child = new node;
	child = cur -> child;

	std::string type = child -> data, 
		id = child -> younger_sibling -> child -> data;

	add_leaf_node(type);
	add_leaf_node(id);
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
	add_leaf_node(cur -> data);
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
	add_leaf_node(var);

	node *expr = new node;
	expr = get_youngest_sibling(child);

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
	// std::cout << cur -> younger_sibling -> data << std::endl;
	traverse_expr(cur -> younger_sibling -> younger_sibling -> child);
}

void AST::build_tree(node *cur){
	if(cur){
		std::cout << "Current Node from CST: " << cur -> data << std::endl;

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
			// kill_all_children();
			std::cout << get_parent(current) << std::endl;
			std::cout << root << std::endl;
			if(get_parent(current) != root){
				kill_all_children();
				// std::cout << "kill_all_children" << std::endl;
				kill_all_children();
			}
		}
		else if(!cur -> data.compare("print_stmt")){
			traverse_print(cur -> child);
		}

		build_tree(cur -> child);
		build_tree(cur -> younger_sibling);
	}

	// std::cout << current -> data << std::endl;
	
	
}




