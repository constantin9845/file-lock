#include "../include/fileHandler.h"

void invalid(){
	std::cout<<"Invalid input. \nUsage: \n./run <path> -enc/dec <key file path>";
	exit(1);
}


int main(int argc, char const *argv[]){
	
	// valid number of arguments
	if(argc == 3 || argc == 4){
		bool directionFlag, ownKey = false;
		std::string path; 
		std::string keyPath;

		// Parse and check if options valid
		if(std::string(argv[1]).size() < 1){ std::cout<<"1"<<std::endl; invalid(); }
		if(std::string(argv[2]) != "-enc" && std::string(argv[2]) != "-dec"){ std::cout<<"2"<<std::endl; invalid(); }

		if(argc == 4){
			ownKey = true;
			keyPath = argv[3];
		}

		path = argv[1];
		directionFlag = ((std::string(argv[2]).substr(1,3)) == "enc") ? 1 : 0;  // 1 for encryption

		// single file and encryption
		if(directionFlag){
			if(ownKey){
				fileHandler::encryptFile(path, keyPath);
				std::cout<<"Find encryted file in Downloads folder"<<std::endl;
			}
			else{
				fileHandler::encryptFile(path);
				std::cout<<"Find Key/IV and encryted file in Downloads folder"<<std::endl;
			}

			return 0;
		}
		else if(!directionFlag){
			fileHandler::decryptFile(path, keyPath);
			std::cout<<"Find decrypted file in Downloads folder"<<std::endl;
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