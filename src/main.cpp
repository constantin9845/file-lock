#include "../include/fileHandler.h"

void invalid(){
	std::cout<<"Invalid input. \nUsage: \n./run <path> -enc/dec -algorithm -r(replace original, w/out = duplicate)";
	exit(1);
}


int main(int argc, char const *argv[]){
	
	// valid number of arguments
	if(argc == 4 || argc == 5){
		bool directionFlag, algorithmFlag;
		bool rFlag = false;
		std::string path; 

		// Parse and check if options valid
		if(std::string(argv[1]).size() < 1){ std::cout<<"1"<<std::endl; invalid(); }
		if(std::string(argv[2]) != "-enc" && std::string(argv[2]) != "-dec"){ std::cout<<"2"<<std::endl; invalid(); }
		if(std::string(argv[3]) != "-aes" && std::string(argv[3]) != "-trivium"){ std::cout<<"3"<<std::endl; invalid(); }

		if(argc == 5){
			if(std::string(argv[4]) != "-r"){ std::cout<<"5"<<std::endl; invalid(); }
			rFlag = true;
		}


		unsigned char* key = fileHandler::genKey();
		for(int i = 0; i < 16; i++){
			std::cout<<std::hex<<(int)key[i]<<" | ";
		}


		path = argv[1];
		directionFlag = ((std::string(argv[2]).substr(1,3)) == "enc") ? 1 : 0;  // 1 for encryption
		algorithmFlag = ((std::string(argv[3]).substr(1,3)) == "aes") ? 1 : 0;  // 1 for aes
		
		// single file and encryption
		if(directionFlag){
			fileHandler::encryptFile(path, algorithmFlag, rFlag);
			std::cout<<"Find Key/IV and encryted file in Downloads folder"<<std::endl;
			return 0;
		}
		else if(!directionFlag){
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