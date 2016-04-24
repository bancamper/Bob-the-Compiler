/*
	File: cst.cpp
	Name: Juan Bancamper
	Class: CMPT 432
	Date: 10 Feb 2016

	This file defines all the class functions for the Concrete Syntax Tree.
*/

#include "bin/cst.h"
#include <iostream>


void Tree::destroy_tree(node *cur){
	if(cur){
		destroy_tree(cur -> child);
		destroy_tree(cur -> younger_sibling);
		delete cur;
	}
}

void Tree::destroy_tree(){
	destroy_tree(root);
}

Tree::Tree(){
	root = NULL;
	current = NULL;
}

Tree::~Tree(){
	destroy_tree();
}

node *Tree::get_parent(node *cur){
	// std::cout << "\tCurrent: " << cur -> data << std::endl;

	if(cur -> older_sibling){

		return get_parent(cur -> older_sibling);
	}

	return cur -> parent;
}

node *Tree::get_youngest_sibling(node *cur){
	if(cur -> younger_sibling){
		return get_youngest_sibling(cur -> younger_sibling);
	}

	return cur;
}

void Tree::add_branch_node(std::string branch){
	node *temp = new node;
	temp -> data = branch;
	temp -> line_num = 0;
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

void Tree::add_leaf_node(std::string leaf, int line){
	node *temp = new node;
	temp -> data = leaf;
	temp -> line_num = line;
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

node *Tree::get_root(){
	return root;
}

void Tree::kill_all_children(){
	node *n = new node;

	// std::cout << "In get_parent()" << std::endl;
	n = get_parent(current);

	// std::cout << "\nCurrent Parent: " << n -> data << std::endl;

	current = n;
}

void Tree::print_tree(node *cur, int level){
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