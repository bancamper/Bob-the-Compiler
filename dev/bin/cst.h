/*
	File: cst.h
	Name: Juan Bancamper
	Class: CMPT 432
	Date: 3 Apr 2016

	This file declares all the functions and class structure for a Concrete
	Syntax Tree.
*/

#include <iostream>

#ifndef CSTGUARD_H
#define CSTGUARD_H value

struct node{
	std::string data;
	node *parent;
	node *child;
	node *older_sibling;
	node *younger_sibling;
};

class Tree{
	private:
		node *root;
		node *current;

		void destroy_tree(node *leaf);
		node *get_youngest_sibling(node *);
		node *get_parent(node *);

	public:
		Tree();
		~Tree();

		void add_branch_node(std::string branch);
		void add_leaf_node(std::string leaf);
		void kill_all_children();
		void destroy_tree();
		void print_tree(node *,int level);

		node *get_root();
		// std::string get_current();

};

#endif