#include "../include/fileHandler.h"

void invalid(){
	std::cout<<"Invalid input. \nUsage: \n./run <path> -enc/dec <key file path>";
	exit(1);
}


int main(int argc, char const *argv[]){

	// Check for valid number of arguments
	if(argc >= 3 && argc <= 5){
		bool directionFlag, ownKey = false, dirFlag, replaceFlag = false;
		std::string path; 
		std::string keyPath;

		// Parse arguments 
		// Check if arguments are valid
		if(std::string(argv[1]).size() < 1){ std::cout<<"1"<<std::endl; invalid(); }
		if(std::string(argv[2]) != "-enc" && std::string(argv[2]) != "-dec"){ std::cout<<"2"<<std::endl; invalid(); }



		// Check for input key or -r flag
		if(argc == 4){
			if(std::string(argv[3]) == "-r"){
				replaceFlag = true;
			}
			else{
				ownKey = false;
				keyPath = argv[3];
			}
		}

		if(argc == 5){
			if(std::string(argv[4]) == "-r"){
				replaceFlag = true;
			}
			else{
				std::cout<<"-r flag error";
				invalid();
			}

			ownKey = true;
			keyPath = argv[3];
		}

		path = argv[1];
		directionFlag = ((std::string(argv[2]).substr(1,3)) == "enc") ? 1 : 0;  // 1 for encryption

#ifdef _WIN32

		// Check if single file or directory - WIN
		std::string star = fileHandler::getFileName(path);

		// set directory flag
		if(star.size() == 0){ dirFlag = true; }else{ dirFlag = false; }

#else
		// Check if single file or directory - UNIX
		std::string star = fileHandler::getFileName(path);

		// set directory flag
		if(star == "*"){ dirFlag = true; }else{ dirFlag = false; }
#endif

		// single file encryption
		if(directionFlag && !dirFlag){
			// User key used
			if(ownKey){
				fileHandler::encryptFile(path, keyPath, replaceFlag);
				std::cout<<"Find encryted file in Downloads folder"<<std::endl;
			}
			// Key generated
			else{
				fileHandler::encryptFile(path, replaceFlag);
				std::cout<<"Find Key/IV and encryted file in Downloads folder"<<std::endl;
			}

			return 0;
		}

		// single file decryption
		else if(!directionFlag && !dirFlag){
			fileHandler::decryptFile(path, keyPath);
			std::cout<<"File has been decrypted"<<std::endl;
			return 0;
		}

		// directory encryption
		else if(directionFlag && dirFlag){

			//  construct relative root directory
			std::string parentDir = path.substr(0, path.size()-1);

			// generate key
			unsigned char* key = fileHandler::genKey();

			// create new root dir / REPLACES EXISTING ONE (target dir in Downloads)
			if(!fileHandler::createRootDir()){
				std::cout<<"Could not create target Directory.";
				delete[] key;
				exit(3);
			}
			
			// check if dir exists and if valid
			if(std::filesystem::exists(parentDir) && std::filesystem::is_directory(parentDir)){

				// iterate each file/dir
				for(const auto& entry : std::filesystem::recursive_directory_iterator(parentDir)){

					// skip hidden files
					if(std::filesystem::is_regular_file(entry) && entry.path().filename().string().front() != '.'){
						
						
						// parse new path for current file
						std::string currentFile = fileHandler::parsePath(entry.path().string(), path);
						if(currentFile == "."){
							std::cout<<"could not parse file path";
							delete[] key;
							exit(3);
						}

						std::string newPath;

						if(!replaceFlag){
							// construct path inside target directory
							newPath = fileHandler::parsePath(entry.path().string(), path);

							// construct the path
							fileHandler::constructPath(newPath);
						}

						// encrypt file
						fileHandler::encryptFile(entry.path().string(), newPath, dirFlag, key, replaceFlag);
						
					}
				}
				fileHandler::storeKey(key); // store the new key with the file
				delete[] key;
			}
		}

		// directory decryption
		else if(!directionFlag && dirFlag){
			
			//  construct relative root directory
			std::string parentDir = path.substr(0, path.size()-1);


			// check if dir exists and if valid dir
			if(std::filesystem::exists(parentDir) && std::filesystem::is_directory(parentDir)){

				// iterate each file/dir
				for(const auto& entry : std::filesystem::recursive_directory_iterator(parentDir)){

					// skp hidden files
					if(std::filesystem::is_regular_file(entry) && entry.path().filename().string().front() != '.' && entry.path().filename().string() != "_key"){
						
						
						// parse new path for current file
						std::string currentFile = fileHandler::parsePath(entry.path().string(), path);
						if(currentFile == "."){
							std::cout<<"could not parse file path";
							exit(3);
						}

						// encrypt file
						fileHandler::decryptFile(entry.path().string(), keyPath);
					}
				}
			}
		}
		else{
			return 0;
		}
	}
	else{
		std::cout<<"else";
		invalid();
	}

	return 0;
}
