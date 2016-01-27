// forkIt.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream"
#include <string>

#define sizeLim 10
using namespace std;

struct arguments
{
	int argc;
	char **argv;
};

arguments* readPrompt();
arguments* writePrompt();
void testArgs(arguments*);
void testSuite(arguments*, string);

int main()
{
	arguments * temp = writePrompt();
	testArgs(temp);
	system("pause");
    return 0;
}

arguments* readPrompt() {
	arguments* temp = new arguments;
	string rawInput;
	getline(std::cin,rawInput);
	temp->argc = 1;
	char* ptr = new char[rawInput.length()+1];
	
	char* head = ptr;   //create unchanging head refrence  so we don't advance as we create new array
	temp->argv = (&head);

	for (int i = 0; i < rawInput.length()+1; ++i, ++ptr) {//need to catch terminating null
		if (rawInput[i] == ' ') {
			rawInput[i] = NULL;
			temp->argc += 1;
		}
		(*ptr) = rawInput[i];
	}

	testSuite(temp, rawInput);

	return temp;
}

arguments* writePrompt() {
	cout << ">";
	return readPrompt();
}

void testArgs(arguments* args) { //just tests using our args class

	char* ptr = (*args->argv);
	cout << "there are " << (args->argc) << " arguments" << endl;
	for (int i = 0; i < args->argc; ++i, ++ptr) {
		while ((*ptr) != NULL) {
			cout << (*ptr);
			++ptr;
		}
		cout << '\n';
	}
}

void testSuite(arguments* args, string input) {
	
	char* ptr = (*args->argv);
	for (int i = 0; i < args->argc; ++i, ++ptr) {
		if (!input.find(ptr, strlen(ptr)-1)) {
			cout << "failed test at: " << i << endl;
		}
	}

}
