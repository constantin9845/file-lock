#include "../include/fileHandler.h"

void guide(){
	std::cout<<"GUIDE";
}

void menu(std::string& file, bool& directionFlag, bool& mode, int& keySize, std::string& keyPath, bool& replaceFlag){

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
		std::string t;
		std::cout<<"\nEnter path key file (enter n for none): ";
		std::cin>>t;

		if(t == "n"){
			keyPath = "";
		}
		else{
			keyPath = t;
		}
	}
	else{
		while(keyPath == ""){
			std::cout<<"\nNeed Key file for Decryption: ";
			std::cin>>keyPath;
		}
	}

	// REPLACE FLAG
	char replace;
	if(!directionFlag){
		replace = 'y';
	}
	else{
		while(replace != 'y' && replace != 'n'){
			std::cout<<"\nReplace existing file(s)? (y or n): ";
			std::cin>>replace;
		}
	}
	
	if(replace == 'y'){ replaceFlag = true;}
	else{ replaceFlag = false;}


	// SUMMARY + WARNING
	std::string summary = "";
	summary += "\n\tSUMMARY\nFile(s): "+file;
	
	if(directionFlag){ summary += "\nEncryption"; }
	else{ summary += "\nDecryption"; }

	if(mode){ summary += "\nMode: CBC"; }
	else{ summary += "\nMode: ECB"; }

	if(keySize == 128){ summary += "\nKey Size: 128"; }
	else if(keySize == 192){ summary += "\nKey Size: 192"; }
	else{ summary += "\nKey Size: 256"; }

	if(keyPath != ""){ summary += "\nKey File: "+keyPath; }
	else{ summary += "\nKey File: New key to be generated"; }

	if(replaceFlag){ summary += "\nReplace input file(s): TRUE"; }
	else{ summary += "\nReplace input file(s): FALSE"; }

	summary += "\nNOTE\n\n*Decrypting files with the wrong key damages files*\n*If files broken after decryption -> encrypt with same parameters and same key again (returns to previous state).";
	summary += "\n*Now proceed with correct key*\n";

	std::cout<<summary<<std::endl;

	std::string confirm = "";
	while(confirm != "y" && confirm != "n"){
		std::cout<<"Confirm (y or n): ";
		std::cin>>confirm;
	}

	if(confirm == "n"){
		exit(99);
	}
	
}


int main(int argc, char const *argv[]){

	std::string path = "";
	bool directionFlag;
	bool mode; // Mode: ECB = 0 / CBC = 1
	int keySize;
	std::string keyPath = "";
	bool replaceFlag = false;

	bool dirFlag;

	menu(path, directionFlag, mode, keySize, keyPath, replaceFlag);

	bool ownKey; 

	if(keyPath == ""){
		ownKey = false;
	}
	else{
		ownKey = true;
	}

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
		if(!ownKey){
			key = fileHandler::genKey(keySize);
		}
		else{
			key = fileHandler::readKey(keyPath, keySize);
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

					std::string newPath;

					if(!replaceFlag){
						// construct path inside target directory
						newPath = fileHandler::parsePath(entry.path().string(), path);

						// construct the path
						fileHandler::constructPath(newPath);
					}

					std::cout<<entry.path().string()<<std::endl;

					// encrypt file
					fileHandler::encryptFile(entry.path().string(), newPath, dirFlag, key, replaceFlag, mode, keySize);
				}
			}
		}
		if(!ownKey){
			fileHandler::storeKey(key, keySize); // store the new key
		}
		delete[] key;

	}

	// DIRECTORY DECRYPTION
	else if(!directionFlag && dirFlag){
		
		//  construct relative root directory
		std::string parentDir = path.substr(0, path.size()-1);


		// check if dir exists and if valid dir
		if(std::filesystem::exists(parentDir) && std::filesystem::is_directory(parentDir)){

			// iterate each file/dir
			for(const auto& entry : std::filesystem::recursive_directory_iterator(parentDir)){

				// skip hidden files
				if(std::filesystem::is_regular_file(entry) && entry.path().filename().string().front() != '.' && entry.path().filename().string() != "_key"){
					
					std::cout<<entry.path().string()<<std::endl;

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
