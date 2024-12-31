#include "../include/fileHandler.h"

void guide(){
	std::cout<<"GUIDE";
}

void menu(std::string& file, bool& directionFlag, bool& mode, int& keySize, std::string& keyPath, std::string& IVpath, bool& replaceFlag){

	// PATH
	std::cout<<"\nEnter <absolute> file/directory path: ";
	std::cin>>file;

	// ENC / DEC
	std::string direction;
	while(direction != "enc" && direction != "dec"){
		std::cout<<"\nEncryption / Decryption (enter enc or dec): ";
		std::cin>>direction;
	}
	if(direction == "enc"){ directionFlag = true; }
	else{ directionFlag = false; }

	// MODE
	std::string temp;
	while(temp != "ecb" && temp != "cbc"){
		std::cout<<"\nMode (ecb or cbc): ";
		std::cin>>temp;
	}
	if(temp == "ecb"){ mode = false; }
	else{ mode = true; }

	// KEY SIZE
	std::string k = "";
	while(k != "128" && k != "192" && k != "256"){
		std::cout<<"\nKey size (128/192/256): ";
		std::cin>>k;
	}
	if(k == "128"){ keySize = 128; }
	else if(k == "192"){ keySize = 192; }
	else{ keySize = 256; }

	// KEY FILE
	if(directionFlag){
		std::cout<<"\nEnter path key file (leave blank for none): ";
		std::cin>>keyPath;

	}
	else{
		while(keyPath == ""){
			std::cout<<"\nNeed Key file for Decryption: ";
			std::cin>>keyPath;
		}
	}

	// IV FILE
	if(mode){
		while(IVpath == ""){
			std::cout<<"\nNeed IV file for Decryption: ";
			std::cin>>IVpath;
		}
	}

	// REPLACE FLAG
	char replace;
	while(replace != 'y' && replace != 'n'){
		std::cout<<"\nReplace existing file(s)? (y or n): ";
		std::cin>>replace;
	}
	if(replace == 'y'){ replaceFlag = true;}
	else{ replaceFlag = false;}
}


int main(int argc, char const *argv[]){

	std::string path = "";
	bool directionFlag;
	bool mode; // Mode: ECB = 0 / CBC = 1
	int keySize;
	std::string keyPath = "";
	std::string IVpath = "";
	bool replaceFlag = false;

	bool dirFlag;

	menu(path, directionFlag, mode, keySize, keyPath, IVpath, replaceFlag);

	bool ownKey = (keyPath != "");
	bool ownIV = (IVpath != "");

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

	// SINGLE FILE ENCRYPTION
	if(directionFlag && !dirFlag){
		// With user key 
		if(ownKey){
			fileHandler::encryptFile(path, keyPath, replaceFlag, mode, keySize);
			std::cout<<"Find encryted file in Downloads folder"<<std::endl;
		}
		// New key
		else{
			fileHandler::encryptFile(path, replaceFlag, mode, keySize);
			std::cout<<"Find Key/IV and encryted file in Downloads folder"<<std::endl;
		}

		return 0;
	}

	// SINGLE FILE DECRYPTION
	else if(!directionFlag && !dirFlag){
		fileHandler::decryptFile(path, keyPath, mode, keySize);
		std::cout<<"File has been decrypted"<<std::endl;
		return 0;
	}

	// DIRECTORY ENCRYPTION
	else if(directionFlag && dirFlag){

		//  construct relative root directory
		std::string parentDir = path.substr(0, path.size()-1);

		unsigned char* key;
		if(ownKey){
			key = fileHandler::readKey(keyPath, keySize);
		}
		else{
			key = fileHandler::genKey(keySize);
			fileHandler::storeKey(key, keySize); // store the new key
		}

		// create new root dir / REPLACES EXISTING ONE (target dir in Downloads)
		if(!fileHandler::createRootDir()){
			std::cout<<"Could not create target Directory.";
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
					fileHandler::encryptFile(entry.path().string(), newPath, dirFlag, keyPath, replaceFlag, mode, keySize);
					
				}
			}
			delete[] key;
		}
	}

	// DIRECTORY DECRYPTION
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

					// decrypt file
					fileHandler::decryptFile(entry.path().string(), keyPath, mode, keySize);
				}
			}
		}
	}

	else{
		return 0;
	}
}
