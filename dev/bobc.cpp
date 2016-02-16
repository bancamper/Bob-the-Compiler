#include <iostream>
#include <fstream>
#include "bin/lex.h"

using namespace std;

int main(int argc, char const *argv[]){
	const int SIZE = 100;
	int line_count = 1;
	char buffer[SIZE];

	ifstream code;
	code.open(argv[1], ios::in);

	while(code.getline(buffer, SIZE)){
		lets_get_lexical(line_count++, buffer);
		// cout << buffer;
	}

	
	return 0;
}