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
#include <sstream>


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

scope *Symbol_Table::get_root(){
	return root;
}

void Symbol_Table::init(char c){
	find(get_current(), c) -> second.init = true;
}

std::string Symbol_Table::check_vars(scope *cur){
	if(cur){
		std::stringstream w;

		for (auto elem : cur -> symbols){
			if(!elem.second.init){
				w << "\tUninitialized variable: " << elem.first << " on line " << elem.second.line_num << std::endl;
				continue;
			}
			if(!elem.second.used){
				w << "\tUnused variable: " << elem.first << " on line " << elem.second.line_num << std::endl;
			}
		}

		return w.str() + check_vars(cur -> child) + check_vars(cur -> younger_sibling);
	}

	return "";
}

void Symbol_Table::insert(char c, std::string type, int line_num){
	std::map<char, var_info>::iterator it = current -> symbols.find(c);
	if(it != current -> symbols.end()){
		std::cout << "\nRedecleration Error:\n\t" << it -> second.type << " " 
			<< it -> first << ", on line " << it -> second.line_num << "\n\t" 
			<< type << " " << c << ", on line " << line_num << std::endl;
		exit(EXIT_FAILURE);
	}

	var_info v = {
		type,
		line_num,
		false,
		false
	};
	current -> symbols.insert(std::make_pair<char, var_info>(c, v));
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

		for(auto elem : cur -> symbols){
			for (int i = 0; i < level; i++){
				std::cout << "|";
				std::cout << "   ";
			}
			std::cout << "Type: " << elem.second.type << ", Id: " << elem.first << std::endl;
		}

		print_tree(cur -> child, level + 1);
		print_tree(cur -> younger_sibling, level);
	}
}

void Symbol_Table::print_table(scope *cur){
	if(cur){
		for(auto elem : cur -> symbols){
			std::cout << "     " << elem.first << "      |   " << elem.second.type 
				<< "\t|      " << cur -> scope_level << "      |     " 
				<< elem.second.line_num << "\t  |     " << elem.second.init 
				<< "      |     " << elem.second.used << std::endl;
		}

		print_table(cur -> child);
		print_table(cur -> younger_sibling);
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

	// print_tree(root, 0);
}

void Symbol_Table::integrity(char c, std::string type, int line){
	scope *n = new scope;
	n = current;

	while(n){
		std::string map_type = n -> symbols.find(c) -> second.type;
		// std::cout << "Current Type: " << map_type << std::endl;
		if(!map_type.compare(type)){
			return;
		}
		else if(map_type.compare(type) && !map_type.empty()){
			std::cout << "\nType Mismatch Error:  Cannot assign " << type << " to a " << map_type << ", on line " << line << std::endl;
			exit(EXIT_FAILURE);
		}

		n = get_parent(n);
	}

	std::cout << "\nOut of Scope: " << c << ", on line " << line << std::endl;
	exit(EXIT_FAILURE);

}

std::map<char, var_info>::iterator Symbol_Table::find(scope *cur, char c){
	if(cur){
		std::string type = cur -> symbols.find(c) -> second.type;

		if(!type.empty()){
			return cur -> symbols.find(c);
		}

		return find(get_parent(cur), c);
	}

}

void Symbol_Table::integrity(char undefined_var, char defined_var, int line){

	std::string defined_type = find(current, defined_var) -> second.type,
		undefined_type = find(current, undefined_var) -> second.type; 

	if(!defined_type.compare(undefined_type)){
		find(current, defined_var) -> second.used = true;
		return;
	}
	else if(defined_type.compare(undefined_type) && !defined_type.empty()){
		std::cout << "\nType Mismatch Error:  Cannot assign " << undefined_type << " to a " << defined_type << ", on line " << line << std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout << "\nOut of Scope: " << defined_var << ", on line " << line << std::endl;
	exit(EXIT_FAILURE);
}

void Symbol_Table::kill_all_children(){
	scope *n = new scope;
	n = get_parent(current);
	if(n){
		current = n;
	}
}

AST::AST(){
	root = NULL;
	current = NULL;
	level_count = 0;
}

AST::~AST(){

}

void AST::print_table(){
	std::cout << "\n\t\t\tSymbol Table\n"
		<< "    Name    |    Type\t|    Scope    |    Line\t  |    Init    |    Used\n" 
		<< "----------------------------------------------------------------------------" 
		<< std::endl;
	symbol_table.print_table(symbol_table.get_root());
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

void AST::add_leaf_node(std::string leaf, int line){
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

void AST::kill_all_children(){
	node *n = new node;

	n = get_parent(current);
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
}

void AST::traverse_var_decl(node *cur){
	add_branch_node(cur -> data);

	node *child = new node;
	child = cur -> child;

	node *type = child, 
		*id = child -> younger_sibling -> child;

	add_leaf_node(type -> data, type -> line_num);
	add_leaf_node(id -> data, id -> line_num);
	kill_all_children();
}

void AST::traverse_int(node *cur){

	if(cur -> younger_sibling){
		add_branch_node("add");
		add_leaf_node(cur -> data, cur -> line_num);
		traverse_expr(cur -> younger_sibling -> younger_sibling -> child);
		kill_all_children();
	}
	else{
		add_leaf_node(cur -> data, cur -> line_num);
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
	add_leaf_node(char_seq, cur -> line_num);

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
		add_leaf_node(cur -> data, cur -> line_num);
	}
}

void AST::traverse_id(node *cur){
	add_leaf_node(cur -> data, cur -> line_num);
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
	node *var = child -> child;
	add_leaf_node(var -> data, var -> line_num);

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
	traverse_expr(cur -> younger_sibling -> younger_sibling -> child);
	kill_all_children();
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
		else if(!cur -> data.compare("print_stmt")){
			traverse_print(cur -> child);
		}

		build_tree(cur -> child);

		if(!cur -> data.compare("block")){
			kill_all_children();

			if(!current -> data.compare("while") || !current -> data.compare("if")){
				kill_all_children();
			}
		}

		build_tree(cur -> younger_sibling);

	}
	
}

void AST::check_string(node *cur){

}

std::string AST::check_bool(node *cur){
	if(cur){
		std::string type_1 = "", type_2;
		if(!cur -> data.compare("!=") || !cur -> data.compare("==")){
			type_1 = check_bool(cur -> child);
			type_2 = check_bool(cur -> child -> younger_sibling);
		}
		else if(!cur -> data.compare("true") || !cur -> data.compare("false")){
			return "boolean";
		}

		else if(std::regex_match(cur -> data, std::regex("[0-9]"))){
			return "int";
		}
		else if(std::regex_match(cur -> data, std::regex("[a-z]"))){
			if(symbol_table.find(symbol_table.get_current(), cur -> data[0]) -> second.type.empty()){
				std::cout << "\nUndefined variable: " << cur -> data << ", on line " << cur -> line_num << std::endl;
				exit(EXIT_FAILURE);
			}

			symbol_table.find(symbol_table.get_current(), cur -> data[0]) -> second.used = true;

			return symbol_table.find(symbol_table.get_current(), cur -> data[0]) -> second.type;
			
		}
		else if(std::regex_match(cur -> data, std::regex("\"[a-z ]*\""))) {
			return "string";
		}

		if(!type_1.compare(type_2)){
			return "boolean";
		}
		else{
			std::cout << "\nInvalid boolean expression: cannot compare " << type_1 << " and " <<  type_2 << " on line " << cur -> child -> line_num << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	return "";
}

void AST::check_int(node *cur){
	if(cur){
		if(!cur -> data.compare("add")){
			check_int(cur -> child);
			check_int(cur -> child -> younger_sibling);
		}
		else if(std::regex_match(cur -> data, std::regex("[0-9]"))){
			//Making sure it is a digit
		}
		else if(std::regex_match(cur -> data, std::regex("[a-z]"))){
			symbol_table.integrity(cur -> data[0], "int", cur -> line_num);
		}
		else{
			std::cout << "\nInvalid int expression: [" << cur -> data << "] on line " << cur -> line_num << std::endl;
			exit(EXIT_FAILURE);
		}
	}
}

void AST::check_print(node *cur){
	if(cur){
		if(!cur -> data.compare("add") || std::regex_match(cur -> data, std::regex("[0-9]"))){
			check_int(cur);
		}
		else if(std::regex_match(cur -> data, std::regex("[a-z]"))){
			if(symbol_table.find(symbol_table.get_current(), cur -> data[0]) -> second.type.empty()){
				std::cout << "\nUndefined variable: " << cur -> data << ", on line " << cur -> line_num << std::endl;
				exit(EXIT_FAILURE);
			}
			symbol_table.find(symbol_table.get_current(), cur -> data[0]) -> second.used = true;

		}
		else if(!cur -> data.compare("true") || 
			!cur -> data.compare("false") ||
			!cur -> data.compare("==") ||
			!cur -> data.compare("!=")){

			check_bool(cur);
		}
		else if(std::regex_match(cur -> data, std::regex("\"[a-z ]*\""))) {
			// String integrity
		}
	}
}

void AST::check_while(node *cur){
	check_bool(cur);
}

void AST::check_if(node *cur){
	check_bool(cur);
}

void AST::check_assignment(node *cur){
	node *little_brother = cur -> younger_sibling;

	if(std::regex_match(little_brother -> data, std::regex("[0-9]")) ||
		!little_brother -> data.compare("add")){

		symbol_table.integrity(cur -> data[0], "int", cur -> line_num);
		check_int(little_brother);
	}

	else if(!little_brother -> data.compare("true") || 
		!little_brother -> data.compare("false") ||
		!little_brother -> data.compare("==") ||
		!little_brother -> data.compare("!=")){

		symbol_table.integrity(cur -> data[0], "boolean", cur -> line_num);
		check_bool(little_brother);
	}
	else if(std::regex_match(little_brother -> data, std::regex("\"[a-z ]*\""))) {
		symbol_table.integrity(cur -> data[0], "string", cur -> line_num);
	}

	else if(std::regex_match(little_brother -> data, std::regex("[a-z]"))){
		symbol_table.integrity(cur -> data[0], little_brother -> data[0], cur -> line_num);
	}

	symbol_table.init(cur -> data[0]);

}

void AST::type_check(node *cur){
	if(cur){
		if(!cur -> data.compare("var_decl")){
			symbol_table.insert(
				cur -> child -> younger_sibling -> data[0],
				cur -> child -> data,
				cur -> child -> line_num
			);
		}
		else if(!cur -> data.compare("assignment")){
			check_assignment(cur -> child);
		}
		else if(!cur -> data.compare("if")){
			check_if(cur -> child);
		}
		else if(!cur -> data.compare("while")){
			check_while(cur -> child);
		}
		else if(!cur -> data.compare("block")){
			symbol_table.add_scope(level_count++);
		}
		else if(!cur -> data.compare("print")){
			check_print(cur -> child);
		}

		type_check(cur -> child);

		if(!cur -> data.compare("block")){
				symbol_table.kill_all_children();
				level_count--;
		}

		type_check(cur -> younger_sibling);
	}
}

void AST::scope_check(){
	std::string warnings = symbol_table.check_vars(symbol_table.get_root());
	if(!warnings.empty()){
		std:: cout << "Semantic Warning(s):\n" << warnings << std::endl; 
	}
	else{
		std:: cout << "No Semantic Warnings\n" << std::endl; 
	}
}

AST semantisize(Tree &c){
	AST ast;
	ast.build_tree(c.get_root());
	ast.type_check(ast.get_root());
	ast.scope_check();

	return ast;
}



