/*
	File: delta.h
	Name: Juan Bancamper
	Class: CMPT 432
	Date: 10 Feb 2016

	This file includes 4 different matrix declarations
		1. keywords
		2. identifiers
		3. symbols/operators
		4. digits
*/

#ifndef DELTAGUARD_H
#define DELTAGUARD_H

#include <set>
#include <map>
#include <cstdlib>

//defining enumerations states
enum states {
	q0, q1, q2, q3, q4, q5, q6, q7, q8, q9, q10, q11, q12,
	q13, q14, q15, q16, q17, q18, q19, q20, q21, q22, q23, q24,
	q25, q26, q27, q28, q29, q30, q31, q32, q33, q34, q35, q36,
	q37
};

int state = q0;

//set of all accepting states
int accepting_states[] = {q2, q4, q13, q17, q20, q27, q32, q36};
std::set<int> keywords_F(accepting_states, accepting_states + 8);

//map of all symbols and their names
std::map<std::string, std::string> symbols_map;

void create_symbols_map(){
	symbols_map["{"] = "open_block";
	symbols_map["}"] = "closed_block";
	symbols_map["("] = "open_paren";
	symbols_map[")"] = "closed_paren";
	symbols_map["="] = "assign";
	symbols_map["=="] = "equals";
	symbols_map["!="] = "not_equals";
	symbols_map["+"] = "plus";
	symbols_map["$"] = "end_of_program";
	symbols_map["\""] = "quote";
}

//set of all availble symbols
char sym_set[] = {'{', '}', '(', ')', '=', '!', '+', '$', '\"'};
std::set<char> symbols_set(sym_set, sym_set + 9);


//defining keywords DFA matrix
//good luck finding accepting states
int keywords[][16] = {
	{q37, q21, q37, q28, q37, q37, q1, q37, q37, q37, q6, q37, q5, q33, q37, q7},		//q0
	{q37, q37, q37, q2, q37, q37, q37, q37, q3, q37, q37, q37, q37, q37, q37, q37},		//q1
	{q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37},	//q2 if
	{q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q4, q37, q37},	//q3
	{q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37},	//q4 int
	{q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q9, q37, q37},	//q5
	{q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q14, q37, q37, q37, q37},	//q6
	{q37, q37, q37, q37, q37, q8, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37}, 	//q7
	{q37, q37, q37, q37, q37, q37, q18, q37, q37, q37, q37, q37, q37, q37, q37, q37},	//q8
	{q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q10, q37, q37, q37, q37},	//q9
	{q37, q37, q37, q37, q37, q37, q11, q37, q37, q37, q37, q37, q37, q37, q37, q37},	//q10
	{q37, q37, q37, q37, q37, q37, q37, q37, q12, q37, q37, q37, q37, q37, q37, q37}, 	//q11
	{q37, q37, q37, q37, q13, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37}, 	//q12
	{q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37}, 	//q13 string
	{q37, q37, q37, q37, q37, q37, q15, q37, q37, q37, q37, q37, q37, q37, q37, q37}, 	//q14
	{q37, q37, q37, q37, q37, q37, q37, q37, q16, q37, q37, q37, q37, q37, q37, q37}, 	//q15
	{q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q17, q37, q37}, 	//q16
	{q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37}, 	//q17 print
	{q37, q37, q37, q37, q37, q37, q37, q19, q37, q37, q37, q37, q37, q37, q37, q37}, 	//q18
	{q37, q37, q20, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37}, 	//q19
	{q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37}, 	//q20 while
	{q37, q37, q37, q37, q37, q37, q37, q37, q37, q22, q37, q37, q37, q37, q37, q37}, 	//q21
	{q37, q37, q37, q37, q37, q37, q37, q37, q37, q23, q37, q37, q37, q37, q37, q37}, 	//q22
	{q37, q37, q37, q37, q37, q37, q37, q24, q37, q37, q37, q37, q37, q37, q37, q37}, 	//q23
	{q37, q37, q25, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37}, 	//q24
	{q26, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37}, 	//q25
	{q37, q37, q37, q37, q37, q37, q37, q37, q27, q37, q37, q37, q37, q37, q37, q37}, 	//q26
	{q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37}, 	//q27 boolean
	{q29, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37}, 	//q28
	{q37, q37, q37, q37, q37, q37, q37, q30, q37, q37, q37, q37, q37, q37, q37, q37}, 	//q29
	{q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q31, q37, q37, q37}, 	//q30
	{q37, q37, q32, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37}, 	//q31
	{q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37}, 	//q32 false
	{q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q34, q37, q37, q37, q37}, 	//q33
	{q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q35, q37}, 	//q34
	{q37, q37, q36, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37}, 	//q35
	{q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37}, 	//q36 true
	{q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37}	//q37 error
};

void reset_state(int& state);

#endif