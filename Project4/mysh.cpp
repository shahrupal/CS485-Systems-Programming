#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

using namespace std;

int main(){
	
	/* initialize variables */
	string input = "";
	string path = "";
	string prompt = "mysh$ ";

	const char* dir;
	char currDir[1024];
	size_t size = 1024;
		
	bool background = false;

	pid_t pid;

	map<string,string> variables;
	map<pid_t,string> process;



	/* output default prompt */
	cout << prompt;


	/* parse through user's line of input */
	while(getline(cin, input)){

		// get current directors path
		path = getcwd(currDir,size); 

		vector<string> tokens;
		string temp = "";
	
		// splits and stores line of input by spaces into vector
		istringstream iss(input);

		while(iss >> temp){
		
			if(temp == "%"){ break;	}  // ignore comments
			tokens.push_back(temp);  // add tokens to vector

		}				


		// ACCOUNT FOR EMPTY LINE
		if(tokens.empty()){ }	

		// SET COMMAND
		else if(tokens[0] == "set"){ 
			
			if(tokens.size() == 3){

				// if variable already exists, override old value with new
				for(map<string,string>::iterator it=variables.begin(); it!=variables.end(); it++){
					if(it->first == tokens[1]){
						it->second = tokens[2];
					}
				}
				
				// store variables and their value to map -- map accounts for repeats
				variables.insert(pair<string,string>(tokens[1],tokens[2])); 

			}
			else{
				cout << "The 'set' command requires 2 parameters. Use the following syntax: <set> <variable> <value>." << endl;
			}


		}

		// SHOW COMMAND
		else if(tokens[0] == "show"){
		
			if(tokens.size() == 2){

				// if variable not in map
				if(variables.find(tokens[1]) == variables.end()){
					cout << "Variable was not assigned a value." << endl;
				}

				// if variable in map, print out value associated with variable
				else{
					cout << variables.find(tokens[1])->second << endl;
				}

			}
			else{
				cout << "The 'show' command requires 1 parameter. Use the following syntax: <show> <variable>." << endl;
			}

		}

		// SET PROMPT COMMAND
		else if(tokens[0] == "setprompt"){
				
			if(tokens.size() == 2){
				prompt = tokens[1] + "$ ";  // set prompt to user's input 
			}	
			else{
				cout << "The 'setprompt' command requires 1 parameter. Use the following syntax: <setprompt> <string>." << endl;
			}
	
		}

		// CHANGE DIRECTORY COMMAND
		else if(tokens[0] == "cd"){

			if(tokens.size() == 2){

				string temp = tokens[1];  

				// change directory to given input
				dir = tokens[1].c_str();     
				if(chdir(dir) != 0){

					// chdir changes directory and outputs 0 if executed successfully
					cout << "Invalid directory!" << endl;

				}
				else{
					
					// output current directory
					path = getcwd(currDir,size);

				}	
			}
			else{
				cout << "The 'cd' command requires 1 paremeter. Use the following syntax: <cd> <path>." << endl;
			}
			
		}

		// LIST PROCESSES COMMAND
		else if(tokens[0] == "listp"){


			// loop through processes- if still existent, output name of process to user
			for(map<pid_t,string>::iterator it=process.begin(); it!=process.end(); it++){		
				int stat;
				if(!waitpid(it->first,&stat,WNOHANG)){
					cout << it->second << endl;
				}	
			}

		}

		// BYE COMMAND
		else if(tokens[0] == "bye"){ 

			// end program
			exit(EXIT_FAILURE);
			break; 

		}

		// MISCELLANEOUS COMMANDS
		else{
		
			// if user enters ampersand, background process
			if(tokens[tokens.size()-1] == "&"){
				background = true;
			}
			

			// intialize array to store tokens
			const char* argv[1024] = {};
			int argvSize = 0;
		
			// store all tokens in an array (except for ampersand)
			for(int i = 0; i < tokens.size(); i++){
				if(tokens[i] != "&"){
					argv[i] = tokens[i].c_str();
					argvSize++;
				}
			}

			// fork the process into parent and child
			pid = fork();

			// if the process is the child
			if(pid == 0){

				// execute the first token (command)
				if(argv[0] == "/" || argv[1] == "/"){
					execv(argv[0],(char* const*)argv);
				}
				else{
					execvp(argv[0],(char* const*)argv);
				} 
			}
			// if the process is not the child
			else{
				
				// foreground process
				if(!background){

					// wait for process to be over before prompting for a new command
					int status;
					if(waitpid(pid,&status,0) < 0)	{
						cout << "Program is non-existent." << endl;
					}

				}
				// background process
				else{
					// store existent processes in map
					if(kill(pid,0) == 0){
						process[pid] = argv[0];
					}		
 				} 

			}

		}

		// reset variables
		background = false;
		tokens.clear();
	
		// output current directory path and prompt
		cout << path << prompt;

	}


	return 0;

}


