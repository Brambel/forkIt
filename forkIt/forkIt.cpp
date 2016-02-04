#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <iostream>
#include <cstring>
#include <fcntl.h>


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
void testSuite(arguments, string);
bool isValid(arguments*);
int redirectLeft(arguments*);
int redirectRight(arguments*);

bool run;

int main()
{
	
	run = true;
	while (run) {
	
		arguments * temp = writePrompt();

		int in = redirectRight(temp);
		int out = redirectLeft(temp);
		
		if(in>-1){
			dup2(in, 0);
		}
		if(out>-1){
			dup2(out,1);
		}

		if (isValid(temp)) {

			int status;
			long head = (long)*temp->argv;
			long mHead = (long) temp->argv;
			if (fork() != 0) {
				//parent
				*temp->argv =(char*) head;
				waitpid(-1, &status, 0);

				if(in>-1){
					close(in);
				}
				if(out>-1){
					close(out);
				}
			}
			else {
				//child
				*temp->argv = (char*)head;

				execvp(temp->argv[0], temp->argv);
				cout<<"process failed, likley due to unknown command\n";
				return 0;
			}
		}
	}

    return 0;
}

arguments* readPrompt() {
	arguments* temp = new arguments;
	string rawInput;
	int aryLen =10;
	getline(std::cin,rawInput);
	char** ptr = new char*[aryLen];
	temp->argv=ptr;
	
	int start = 0;
	int end=0;
	int i=0;
	for (; start < rawInput.length();start=(end+=1), ++i){//we set start to begining of next segment

		if(i==aryLen){
			aryLen+=10;
			char** tempPtr= new char*[aryLen];
			for(int j=0;j<i;++j){
				tempPtr[j]=ptr[j];
			}
		delete ptr;
		ptr=tempPtr;		
		}
		end = rawInput.find(' ', start);//get the end of the next segment
		if(end==-1){
			end=rawInput.length();//were finished fall out naturally
		}
		rawInput[end]='\0';//change whitespace to NULL
		ptr[i]=&rawInput[start]; //set a new charpointer to our new segment	
	}
	temp->argc=++i;
	temp->argv[i]=NULL;
	
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
	while(ptr!= NULL){
		cout<<*ptr<<endl;
		++ptr;
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

int redirectLeft(arguments* args){
	char* comp;
	for(int i=0;i<args->argc-1;i++){
		comp=args->argv[i];
		if(strcmp("<", comp)==0){
			return open(args->argv[i+1], O_RDONLY);
		}
		else if(strcmp("<<", comp)==0){
			return open(args->argv[i+1], O_RDONLY);			
		}
	}
	return -1;
}

int redirectRight(arguments* args){
	char* comp;
	for(int i=0;i<args->argc-1;i++){
		comp=args->argv[i];
		if(strcmp(">", comp)==0){
			return open(args->argv[i+1], O_WRONLY |O_TRUNC | O_CREAT);
		}
		else if(strcmp(">>", comp)==0){
			int temp;
			return open(args->argv[i+1], O_WRONLY |O_APPEND | O_CREAT);
			return temp;			
		}
	}
	return -1;
}
