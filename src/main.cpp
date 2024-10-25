#include "../include/fileHandler.h"

void invalid(){
	std::cout<<"Invalid input. \nUsage: \n./run <path> -enc/dec";
	exit(1);
}


int main(int argc, char const *argv[]){
	
	// valid number of arguments
	if(argc == 2 || argc == 3){
		bool directionFlag;
		std::string path; 

		// Parse and check if options valid
		if(std::string(argv[1]).size() < 1){ std::cout<<"1"<<std::endl; invalid(); }
		if(std::string(argv[2]) != "-enc" && std::string(argv[2]) != "-dec"){ std::cout<<"2"<<std::endl; invalid(); }


		path = argv[1];
		directionFlag = ((std::string(argv[2]).substr(1,3)) == "enc") ? 1 : 0;  // 1 for encryption

		// single file and encryption
		if(directionFlag){
			fileHandler::encryptFile(path);
			std::cout<<"Find Key/IV and encryted file in Downloads folder"<<std::endl;
			return 0;
		}
		else if(!directionFlag){
			fileHandler::decryptFile(path);
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