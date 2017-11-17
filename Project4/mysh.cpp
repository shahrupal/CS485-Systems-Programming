#include <iostream>
#include <stdio.h>
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


	/* ignore everything after % -- if token == %, break*/
	
	string input = "";
	string path = "";
	string prompt = "mysh$ ";
	const char* dir;
	char currDir[1024];
	size_t size = 1024;
	map<string,string> variables;
	pid_t pid;
	bool background = false;
	vector<pid_t> processes;


	cout << "ur wish is my command:" << endl;	
	cout << prompt;
	while(getline(cin, input)){

		path = getcwd(currDir,size);

		vector<string> tokens;
		string temp = "";
	
		// splits and stores line of input by spaces into vector
		istringstream iss(input);
		while(iss >> temp){
			tokens.push_back(temp);
		}		

		// check size of vectors to make sure appropriate number of parameters associated with specific command
		if(tokens[0] == "set"){ 
			
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
		else if(tokens[0] == "setprompt"){
				
			if(tokens.size() == 2){
				prompt = tokens[1] + "$ ";
			}	
			else{
				cout << "The 'setprompt' command requires 1 parameter. Use the following syntax: <setprompt> <string>." << endl;
			}
	
		}
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
		else if(tokens[0] == "listp"){
			
			for(int i = 0; i < processes.size(); i++){
				cout << processes[i] << endl;
			}

		}

		else if(tokens[0] == "bye"){ 
			break; 
		}

		else{
		
			if(tokens[tokens.size()-1] == "&"){
				background = true;
			}
			

			const char* argv[1024];
		
			for(int i = 0; i < tokens.size(); i++){
				if(tokens[tokens.size()-1] != "&"){
					argv[i] = tokens[i].c_str();
				}
			}

			pid = fork();
			if(pid == 0){
				execv(argv[0],(char* const*)argv);			    	        		 
			}
			else{
				
				// foreground process
				if(!background){
					int status;
					if(waitpid(pid,&status,0) < 0)	{
						cout << "Program is non-existent." << endl;
					}
				}
				else{
					// if process exists
					if(kill(pid,0) == 0){
						processes.push_back(pid);  // stores process id's
					}
				}
			}		

		}

		cout << path << prompt;

	}


	return 0;

}

