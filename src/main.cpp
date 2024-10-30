#include "../include/fileHandler.h"

void invalid(){
	std::cout<<"Invalid input. \nUsage: \n./run <path> -enc/dec <key file path>";
	exit(1);
}


int main(int argc, char const *argv[]){
	
	// valid number of arguments
	if(argc == 3 || argc == 4){
		bool directionFlag, ownKey = false, dirFlag;
		std::string path; 
		std::string keyPath;

		// Parse and check if options valid
		if(std::string(argv[1]).size() < 1){ std::cout<<"1"<<std::endl; invalid(); }
		if(std::string(argv[2]) != "-enc" && std::string(argv[2]) != "-dec"){ std::cout<<"2"<<std::endl; invalid(); }

		// Check for input key
		if(argc == 4){
			ownKey = true;
			keyPath = argv[3];
		}

		path = argv[1];
		directionFlag = ((std::string(argv[2]).substr(1,3)) == "enc") ? 1 : 0;  // 1 for encryption

#ifdef _WIN32

		// Check if single file or directory
		std::string star = fileHandler::getFileName(path);

		// set directory flag
		if(star.size() == 0){ dirFlag = true; }else{ dirFlag = false; }

#else
		// Check if single file or directory
		std::string star = fileHandler::getFileName(path);

		// set directory flag
		if(star == "*"){ dirFlag = true; }else{ dirFlag = false; }
#endif

		// single file encryption
		if(directionFlag && !dirFlag){
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
		// single file decryption
		else if(!directionFlag && !dirFlag){
			fileHandler::decryptFile(path, keyPath);
			std::cout<<"Find decrypted file in Downloads folder"<<std::endl;
			return 0;
		}
		// directory encryption
		else if(directionFlag && dirFlag){

			//  get root directory
			std::string parentDir = path.substr(0, path.size()-1);


			// create new root dir / REPLACES EXISTING ONE
			if(!fileHandler::createRootDir()){
				std::cout<<"Could not create target Directory.";
				exit(3);
			}
			
			// check if dir exists and if valid dir
			if(std::filesystem::exists(parentDir) && std::filesystem::is_directory(parentDir)){

				// iterate each file/dir
				for(const auto& entry : std::filesystem::recursive_directory_iterator(parentDir)){
					if(std::filesystem::is_regular_file(entry) && entry.path().filename().string().front() != '.'){
						
						
						// parse new path for current file
						std::string currentFile = fileHandler::parsePath(entry.path().string());
						if(currentFile == "."){
							std::cout<<"could not parse file path";
							exit(3);
						}
						
						// recreate path inside target folder
						std::cout<<fileHandler::parsePath(entry.path().string())<<std::endl;

						// encrypt file
						//fileHandler::encryptFile(entry.path().string(), dirFlag);
					}
				}
			}
		}
		// directory decryption
		else if(!directionFlag && dirFlag){

		}
		else{
			// iterate directory;
			return 0;
		}
	}
	else{
		std::cout<<"else";
		invalid();
	}


	return 0;
}