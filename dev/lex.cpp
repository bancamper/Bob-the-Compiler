/*
	File: lex.cpp
	Name: Juan Bancamper
	Class: CMPT 432
	Date: 10 Feb 2016

	This file includes all the function definitions used to create Tokens
	based off of Design of Compilers Language Grammar
*/
#include <iostream>
#include "bin/lex.h"
#include "bin/delta.h"

using namespace std;

void lets_get_lexical(int line_count, char line_of_code[]){
	cout << "Line " << line_count << ": " << line_of_code << endl;
}