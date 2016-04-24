/*
	File: cst.h
	Name: Juan Bancamper
	Class: CMPT 432
	Date: 3 Apr 2016

	This file declares all the functions and classes for a Concrete
	Syntax Tree.
*/

#include <iostream>

#ifndef CSTGUARD_H
#define CSTGUARD_H value

struct node{
	std::string data;
	int line_num;
	node *parent;
	node *child;
	node *older_sibling;
	node *younger_sibling;
};

class Tree{
	private:
		node *root;
		node *current;

		void destroy_tree(node *);
		node *get_youngest_sibling(node *);
		node *get_parent(node *);

	public:
		Tree();
		~Tree();

		void add_branch_node(std::string);
		void add_leaf_node(std::string, int);
		void kill_all_children();
		void destroy_tree();
		void print_tree(node *,int);

		node *get_root();
};

#endif