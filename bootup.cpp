#include <string>
#include <iostream>
#include <stdio.h>

std::string exec(char const*);

int main(int argc, char* argv[]){
		std::cout << "Enter a username:\n";
		std::string user;
		std::cin >> user;
		std::string hashQuery = "cat etc/shadow | grep " + user + " | awk -F \":\" '{print $2}' | awk -F \"$\" '{print $4}'";
		std::string saltQuery = "cat etc/shadow | grep " + user + " | awk -F \":\" '{print $2}' | awk -F \"$\" '{print $3}'";
		std::string pass;
		std::cout << "Enter your password:\n";
		std::cin >> pass;
		std::string salt = exec(saltQuery.c_str());
		salt = salt.substr(0,salt.length()-1);
		std::string hash = exec(hashQuery.c_str());
		std::string hashGen = "perl -e 'print crypt(\"" + pass + "\",\"\\$6\\$" + salt + "\\$\") . \"\\n\"' | grep " + hash + " 2>/dev/null";
		std::string hashGrep = exec(hashGen.c_str());
		if(hashGrep != "" && hash != "")
			std::cout << "You logged in!" << std::endl;
		else
			std::cout << "Invalid login." << std::endl;
}

std::string exec(char const* cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
    	if(fgets(buffer, 128, pipe) != NULL)
    		result += buffer;
    }
    pclose(pipe);
    return result;
}
