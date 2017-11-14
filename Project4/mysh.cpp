#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

int main(){
	
	string input = "";

	cout << "ur wish is my command: \n" << endl;	

	while(getline(cin, input)){

		vector<string> tokens;
		string temp = "";

		stringstream ss(input);
		while(getline(ss, temp, ' ')){
			tokens.push_back(temp);
		}		

		for(int i = 0; i < tokens.size(); i++){
			cout << tokens[i];
		}
		if(input == "bye"){
			break;			
		}

	}


	//exit status 0

}
