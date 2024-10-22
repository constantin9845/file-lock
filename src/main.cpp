#include "../include/fileHandler.h"
#include <string>

void invalid(){
	std::cout<<"Invalid input. \nUsage: \n./run <path> -d/f(dir or file) -enc/dec -algorithm -r(replace original, w/out = duplicate)";
	exit(1);
}


int main(int argc, char const *argv[]){
	
	// valid number of arguments
	if(argc == 5 || argc == 6){
		bool fileFlag, directionFlag, algorithmFlag;
		bool rFlag = false;
		std::string path; 

		// Parse and check if options valid
		if(std::string(argv[1]).size() < 1){ std::cout<<"1"<<std::endl; invalid(); }
		if(std::string(argv[2]) != "-d" && std::string(argv[2]) != "-f"){ std::cout<<"2"<<std::endl; invalid(); }
		if(std::string(argv[3]) != "-enc" && std::string(argv[3]) != "-dec"){ std::cout<<"3"<<std::endl; invalid(); }
		if(std::string(argv[4]) != "-aes" && std::string(argv[4]) != "-trivium"){ std::cout<<"4"<<std::endl; invalid(); }

		if(argc == 6){
			if(std::string(argv[5]) != "-r"){ std::cout<<"5"<<std::endl; invalid(); }
			rFlag = true;
		}


		path = argv[1];
		fileFlag = ((std::string(argv[2]).substr(1)[0]) == 'f') ? true : false; // true for single file
		directionFlag = ((std::string(argv[3]).substr(1,3)) == "enc") ? 1 : 0;  // 1 for encryption
		algorithmFlag = ((std::string(argv[4]).substr(1,3)) == "aes") ? 1 : 0;  // 1 for aes
		
		// single file and encryption
		if(fileFlag && directionFlag){
			fileHandler::encryptFile(path, algorithmFlag, rFlag);
			std::cout<<"Find Key/IV and encryted file in Downloads folder"<<std::endl;
			return 0;
		}
		else if(fileFlag && !directionFlag){
			fileHandler::decryptFile(path, algorithmFlag, rFlag);
			std::cout<<"Find Key/IV and decrypted file in Downloads folder"<<std::endl;
			return 0;
		}
		else{
			// iterate directory;
			return 0;
		}
	}
	else{
		invalid();
	}


	return 0;
}