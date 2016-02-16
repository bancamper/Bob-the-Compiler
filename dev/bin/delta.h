/*
	File: lex.cpp
	Name: Juan Bancamper
	Class: CMPT 432
	Date: 10 Feb 2016

	This file includes 4 different matrix declarations
		1. Keywords
		2. identifiers
		3. symbols/operators
		4. digits
*/

#ifndef DELTAGUARD_H
#define DELTAGUARD_H

//defining enumerations states
enum states {
	q0, q1, q2, q3, q4, q5, q6, q7, q8, q9, q10, q11, q12,
	q13, q14, q15, q16, q17, q18, q19, q20, q21, q22, q23, q24,
	q25, q26, q27, q28, q29, q30, q31, q32, q33, q34, q35, q36,
	q37
};

//defining keywords DFA matrix
int[][] keywords = {
	{q37, q21, q37, q28, q37, q37, q1, q37, q37, q37, q6, q37, q5, q33, , q7},			//q0
	{q37, q37, q37, q2, q37, q37, q37, q37, q3, q37, q37, q37, q37, q37, q37, q37},		//q1
	{q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37},	//q2
	{q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q37, q4, q37, q37},	//q3
	{},
	{}
};

//defining ID DFA matrix
int[][] identifiers = {
	{q1, q2},
	{q2, q2},
	{q2, q2}
}

//defining digits DFA matrix
int[][] digits = {
	{q1, q2},
	{q2, q2},
	{q2, q2}
}

#endif