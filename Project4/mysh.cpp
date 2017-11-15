#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <string>
#include <sstream>
#include <map>

using namespace std;

int main(){
	
	string input = "";
	map<string,string> variables;

	cout << "ur wish is my command:" << endl;	

	while(getline(cin, input)){

		vector<string> tokens;
		string temp = "";

		// splits and stores line of input by spaces into vector
		stringstream ss(input);
		while(getline(ss, temp, ' ')){
			tokens.push_back(temp);
		}		

		// prints vector
//		for(int i = 0; i < tokens.size(); i++){
//			cout << tokens[i];
//		}

		// check size of vectors to make sure appropriate number of parameters associated with specific command
		if(tokens[0] == "set"){ 
			
			if(tokens.size() == 3){
				variables.insert(pair<string,string>(tokens[1],tokens[2]));  // store variables and their values into map
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
				cout << "The 'show' command requres 1 paramete. Use the following syntax: <show> <variable>." << endl;
			}

		 }
		else if(tokens[0] == "setprompt"){ }
		else if(tokens[0] == "cd"){ }
		else if(tokens[0] == "listp"){ }
		else if(tokens[0] == "bye"){ 
			break; 
		}
		else if(tokens[0] == "cmd") {
			 // two options 
		}
		else {
			cout << "Command not found. Please try again." << endl;
		}

	}


	return 0;

}
