#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <cstring>


#define sizeLim 10
using namespace std;


struct arguments
{
	int argc;
	char *argv[];
};

arguments* readPrompt();
arguments* writePrompt();
void testArgs(arguments*);
void testSuite(arguments, string);
bool isValid(arguments*);

bool run;

int main()
{
	
	run = true;
	while (run) {
	
		arguments * temp = writePrompt();
		//char* head = *temp->argv;
		//char* arg[] = {"ls", "-l", NULL};
		//temp->argv = arg;
		//testArgs(temp);
		if (isValid(temp)) {

			int status;
			long head = (long)*temp->argv;
			long mHead = (long) temp->argv;
			if (fork() != 0) {
				*temp->argv =(char*) head;
				//parent
				//cout<<"parent thread start"<<endl;

				waitpid(-1, &status, 0);
				//cout<<"parent thread end\n";
			}
			else {
				//printf("head: %u", head);
				*temp->argv = (char*)head;
				//temp->argv = (char**)mHead;
				//child
				char* arg[] = {"ls", "-l", NULL};
				//execvp(arg[0],arg);
				//cout<<"child start\n";
				//testArgs(temp);
				execvp(temp->argv[0], temp->argv);
				//cout<<"failed\n";
				return 0;
			}
		}
		//system("pause");
	}

	//system("pause");
    return 0;
}

arguments* readPrompt() {
	arguments* temp = new arguments;
	string rawInput;
	getline(std::cin,rawInput);
	temp->argc = 1;
	char* ptr = new char[rawInput.length()];
	
	*temp->argv = ptr;   //create unchanging head refrence  so we don't advance as we create new array
	

	for (int i = 0; i < rawInput.length(); ++i, ++ptr) {//need to catch terminating null
		if (rawInput[i] == ' ') {
			rawInput[i] = NULL;
			temp->argc += 1;
		}
		(*ptr) = rawInput[i]; //the deep copy
	}
	//testSuite(temp, rawInput);  commented out for production
	
	return temp;
}
bool isValid(arguments* args) {

	char* comp = (*args->argv);

	bool tf = true;
	
	if (strcmp("cd", comp)==0){
		chdir(&comp[3]);
		tf = false;
	}
	else if(strcmp("pwd", comp)==0) {
		char* WD = get_current_dir_name();
		cout<<*WD<<endl;
		delete WD;
		tf = false;
	}
	else if(strcmp("quit", comp)==0){
		run = false;
		tf = false;
	}
	*args->argv=comp;	
	return tf;
}

arguments* writePrompt() {
	cout << ">";
	return readPrompt();
}

void testArgs(arguments* args) { //just tests using our args class
	char * head = *args->argv;
	char* ptr = (*args->argv);
	cout << "there are " << (args->argc) << " arguments" << endl;
	for (int i = 0; i < args->argc; ++i, ++ptr) {
		while ((*ptr) != NULL) {
			cout << (*ptr);
			++ptr;
		}
		if(*ptr==NULL){
			cout<<"\'null\'";
		}	
		cout << '\n';
	}
	*args->argv = head;

}

void testSuite(arguments* args, string input) {
	
	char* ptr = (*args->argv);
	for (int i = 0; i < args->argc; ++i, ++ptr) {
		if (!input.find(ptr, strlen(ptr)-1)) {
			cout << "failed test at: " << i << endl;
		}
	}

}
