/*
	File: code_gen.cpp
	Name: Juan Bancamper
	Class: CMPT 432
	Date: 25 Apr 2016

	This file defines all the functions for the class CodeGen.
*/

#include "bin/code_gen.h"
#include <iostream>
#include <sstream>
#include <regex>

CodeGen::CodeGen(){
	heap_ptr = 255;
	code_ptr = 0;
	temp_num = 0;
	scope_level = -1;
}

CodeGen::~CodeGen(){

}

void CodeGen::print_tables(){
	std::cout << "\n\n\t\t\tStatic Data\n" 
		<< "  Temp Addr   |     Var    |    Scope   |   Offset\n" 
		<< "----------------------------------------------------" 
		<< std::endl;

	for (std::vector<var_addr>::iterator i = static_table.begin(); 
		i != static_table.end(); 
		i++){
		
		std::cout << "     " << i -> temp << "     |      " << i -> variable
			<< "     |      " << i -> scope << "     |      "
			<< i -> offset << std::endl;
	}

	std::cout << "\n\n\tJump Table\n" 
		<< "  Temp Addr   |   Offset\n" 
		<< "--------------------------" 
		<< std::endl;

	for (std::vector<jump_addr>::iterator i = jump_table.begin();
		i != jump_table.end();
		i++){
		
		std::cout << "      " << i -> temp << "      |      " 
			<< i -> offset << std::endl;	
	}
}

void CodeGen::print_hex(){
	std::cout << std::endl;
	for (int i = 0; i < 256; i++){
		if(i % 8 == 0 && i > 0){
			std::cout << std::endl;
		}
		std::cout << bytes[i] << " ";
	}
}

struct var_addr CodeGen::find_var(char c){
	for (std::vector<var_addr>::iterator i = static_table.end() - 1;
		i != static_table.begin() - 1; 
		i--){

		if((i -> variable == c) && (i -> scope <= scope_level)){
			return *i;
		}
	}
}

struct var_addr CodeGen::find_var(std::string addr){
	for (std::vector<var_addr>::iterator i = static_table.begin(); 
		i != static_table.end();
		i++){
		
		if(!addr.compare(i -> temp)){
			return *i;
		}
	}
}

struct jump_addr CodeGen::find_jump(std::string jump){

}

void CodeGen::generate_primitive(std::string type, node *cur){
	std::stringstream temp_addr;
	temp_addr << "T" << temp_num << "xx";

	var_addr v = {
		temp_addr.str(),
		cur -> data[0],
		type,
		scope_level,
		temp_num++
	};

	static_table.push_back(v);


	if((code_ptr + 5) < heap_ptr){
		bytes[code_ptr++] = "A9";
		bytes[code_ptr++] = "00";
		bytes[code_ptr++] = "8D";
		bytes[code_ptr++] = v.temp.substr(0, 2);
		bytes[code_ptr++] = v.temp.substr(2, 2);
	}
	else{
		std::cout << "\nMemory Error: " << std::endl;
		exit(EXIT_FAILURE);
	}
}

void CodeGen::generate_string(node *cur, std::string assigned_string){
	bytes[heap_ptr--] = "00";
	char c = '\0';
	int ascii = 0;

	for (int i = assigned_string.size() - 2; i > 0; i--){
		c = assigned_string[i];
		ascii = (int)c;

		char *val_star = new char[2];
		std::sprintf(val_star, "%02X", ascii);
		bytes[heap_ptr--] = val_star;
	}

	std::string str_mem = find_var(cur -> data[0]).temp;

	if(str_mem.empty()){
		std::cout << "\nMemory Error: Could not find " << cur -> data << std::endl;
		exit(EXIT_FAILURE);
	}

	char *val_star = new char[2];
	std::sprintf(val_star, "%02X", heap_ptr + 1);
	
	if((code_ptr + 5) < heap_ptr){
		bytes[code_ptr++] = "A9";
		bytes[code_ptr++] = val_star;
		bytes[code_ptr++] = "8D";
		bytes[code_ptr++] = str_mem.substr(0, 2);
		bytes[code_ptr++] = str_mem.substr(2, 2);
	}
	else{
		std::cout << "\nMemory Error: " << std::endl;
		exit(EXIT_FAILURE);
	}
}

int CodeGen::generate_int(node *cur){
	if(cur){
		if(!cur -> data.compare("add")){
			return generate_int(cur -> child) + generate_int(cur -> child -> younger_sibling);
		}
		else if(std::regex_match(cur -> data, std::regex("[0-9]"))){
			return std::stoi(cur -> data);
		}
		else if(std::regex_match(cur -> data, std::regex("[a-z]"))){
			load_int = find_var(cur -> data[0]).temp;
			return 0;
		}
	}

	return 0;
}

void CodeGen::generate_identifier(char undefined, char defined){
	std::string first_var = find_var(defined).temp,
		second_var = find_var(undefined).temp;

	bytes[code_ptr++] = "AD";
	bytes[code_ptr++] = first_var.substr(0, 2);
	bytes[code_ptr++] = first_var.substr(2, 2);

	bytes[code_ptr++] = "8D";
	bytes[code_ptr++] = second_var.substr(0, 2);
	bytes[code_ptr++] = second_var.substr(2, 2);

}

void CodeGen::generate_bool(node *cur){
	if(cur){
		if(!cur -> data.compare("==") || !cur -> data.compare("!=")){
			generate_bool(cur -> child);
			generate_bool(cur -> child -> younger_sibling);
		}
		else if(!cur -> data.compare("true") || !cur -> data.compare("false")){

		}
	}
}

void CodeGen::generate_var_decl(node *cur){
	if(cur){
		if(!cur -> data.compare("int") || !cur -> data.compare("boolean")){
			generate_primitive(cur -> data, cur -> younger_sibling);
		}
		else if(!cur -> data.compare("string")){
			std::stringstream temp_addr;
			temp_addr << "T" << temp_num << "xx";

			var_addr v = {
				temp_addr.str(),
				cur -> younger_sibling -> data[0],
				cur -> data,
				scope_level,
				temp_num++
			};

			static_table.push_back(v);
		}
	}
}

void CodeGen::generate_assignment(node *cur){
	char c = cur -> data[0];

	node *little_sister = cur -> younger_sibling;

	if(std::regex_match(little_sister -> data, std::regex("[0-9]")) ||
		!little_sister -> data.compare("add")){

	 	std::string temp_var = find_var(c).temp;

	 	char *val_star = new char[2];
	 	std::sprintf(val_star, "%02X", generate_int(little_sister));

	 	if((code_ptr + 5) < heap_ptr){
			bytes[code_ptr++] = "A9";
			bytes[code_ptr++] = val_star;

			if(!load_int.empty()){
				bytes[code_ptr++] = "6D";
				bytes[code_ptr++] = load_int.substr(0, 2);
				bytes[code_ptr++] = load_int.substr(2, 2);

				load_int = "";
			}

			bytes[code_ptr++] = "8D";
			bytes[code_ptr++] = temp_var.substr(0, 2);
			bytes[code_ptr++] = temp_var.substr(2, 2);
		}
		else{
			std::cout << "\nMemory Error: " << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	else if(!little_sister -> data.compare("true") || 
		!little_sister -> data.compare("false") ||
		!little_sister -> data.compare("==") ||
		!little_sister -> data.compare("!=")){

		generate_bool(cur);
	}
	else if(std::regex_match(little_sister -> data, std::regex("\"[a-z ]*\""))) {
		generate_string(cur, little_sister -> data);
	}

	else if(std::regex_match(little_sister -> data, std::regex("[a-z]"))){
		generate_identifier(cur -> data[0], little_sister -> data[0]);
	}
}

void CodeGen::generate_print(node *cur){
	if(cur){
		if(std::regex_match(cur -> data, std::regex("[0-9]")) ||
			!cur -> data.compare("add")){

			char *val_star = new char[2];
	 		std::sprintf(val_star, "%02X", generate_int(cur));

			bytes[code_ptr++] = "A0";
			bytes[code_ptr++] = val_star;
			bytes[code_ptr++] = "A2";
			bytes[code_ptr++] = "01";
			bytes[code_ptr++] = "FF";

		}
		else if(!cur -> data.compare("true") || 
			!cur -> data.compare("false") ||
			!cur -> data.compare("==") ||
			!cur -> data.compare("!=")){

			// generate_bool();
		}
		else if(std::regex_match(cur -> data, std::regex("\"[a-z ]*\""))) {
			bytes[heap_ptr--] = "00";
			std::string assigned_string = cur -> data;
			char c = '\0';
			int ascii = 0;
			std::stringstream s;

			for (int i = assigned_string.size() - 2; i > 0; i--){
				c = assigned_string[i];
				ascii = (int)c;

				s << std::hex << ascii;
				bytes[heap_ptr--] = s.str();

				s.str(std::string());
			}

			char *val_star = new char[2];
			std::sprintf(val_star, "%02X", heap_ptr + 1);
			
			if((code_ptr + 5) < heap_ptr){
				bytes[code_ptr++] = "A0";
				bytes[code_ptr++] = val_star;
				bytes[code_ptr++] = "A2";
				bytes[code_ptr++] = "02";
				bytes[code_ptr++] = "FF";
			}
			else{
				std::cout << "\nMemory Error: " << std::endl;
				exit(EXIT_FAILURE);
			}
		}
		else if(std::regex_match(cur -> data, std::regex("[a-z]"))){
			std::string temp_var = find_var(cur -> data[0]).temp;

			bytes[code_ptr++] = "AC";
			bytes[code_ptr++] = temp_var.substr(0, 2);
			bytes[code_ptr++] = temp_var.substr(2, 2);

			bytes[code_ptr++] = "A2";
			(!find_var(cur -> data[0]).type.compare("string") ? 
				bytes[code_ptr++] = "02":
				bytes[code_ptr++] = "01"
			);
			bytes[code_ptr++] = "FF";
		}
	}
}

void CodeGen::generate(node *cur){
	if(cur){
		if(!cur -> data.compare("block")){
			scope_level++;
		}
		else if(!cur -> data.compare("var_decl")){
			generate_var_decl(cur -> child);
		}
		else if(!cur -> data.compare("assignment")){
			generate_assignment(cur -> child);
		}
		else if(!cur -> data.compare("if")){

		}
		else if(!cur -> data.compare("while")){
			
		}
		else if(!cur -> data.compare("print")){
			generate_print(cur -> child);
		}

		generate(cur -> child);

		if(!cur -> data.compare("block")){
			scope_level--;
		}

		generate(cur -> younger_sibling);
	}
}

void CodeGen::back_patch(){
	bytes[code_ptr++] = "00";

	for (int i = 0; i < code_ptr; i++){
		if(std::regex_match(bytes[i], std::regex("T[0-9]*"))){

			int offset = find_var(bytes[i] + bytes[i + 1]).offset;
			char *val_star = new char[2];
	 		std::sprintf(val_star, "%02X", code_ptr + offset);

	 		bytes[i++] = val_star;
	 		bytes[i] = "00";
		}
		else if(std::regex_match(bytes[i], std::regex("J[0-9]*"))){

		}
	}
}

void CodeGen::fill(){
	while(code_ptr <= heap_ptr){
		bytes[code_ptr++] = "00";
	}
}

CodeGen hex(AST &ast){
	CodeGen cg;
	cg.generate(ast.get_root());
	cg.back_patch();
	cg.fill();
	cg.print_hex();
	cg.print_tables();

	return cg;
}