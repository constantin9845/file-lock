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
	summary += "\n\tSUMMARY\nFile(s)        : "+file;
	
	if(directionFlag){ summary += "\nType           : Encryption"; }
	else{ summary += "\nType           : Decryption"; }

	if(mode){ summary += "\nAES Mode       : CBC"; }
	else{ summary += "\nAES Mode       : ECB"; }

	if(keySize == 128){ summary += "\nKey Size       : 128"; }
	else if(keySize == 192){ summary += "\nKey Size       : 192"; }
	else{ summary += "\nKey Size       : 256"; }

	if(keyPath != ""){ summary += "\nKey File       : "+keyPath; }
	else{ summary += "\nKey File       : New key to be generated"; }

	if(replaceFlag){ summary += "\nReplace file(s): TRUE"; }
	else{ summary += "\nReplace file(s): FALSE"; }

	summary += "\n\n\tNOTE\n";
	summary += "*****************************************************";
	summary += "\n* Decrypting files with the wrong key damages files *";
	summary += "\n* If files broken after decryption:                 *";
	summary += "\n*\t1) Encrypt with same params and key.        *";
	summary += "\n*\t2) This returns files to initial state.     *";
	summary += "\n* Then retry with correct key.                      *";
	summary += "\n*****************************************************";

	std::cout<<summary<<std::endl;

	std::string confirm = "";
	while(confirm != "y" && confirm != "n"){
		std::cout<<"Confirm (y or n): ";
		std::cin>>confirm;
	}
	std::cout<<std::endl;

	if(confirm == "n"){
		std::cout<<"Exiting\n";
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


	// Check if single file or directory
	std::filesystem::path t(path);
	std::string star = t.filename().string();
#ifdef _WIN32

	// set directory flag
	if(star.size() == 0){ dirFlag = true; }else{ dirFlag = false; }
#else

	// set directory flag
	if(star == "*"){ dirFlag = true; }else{ dirFlag = false; }
#endif

	std::string message = "\n\n";

	// ENCRYPTION
	if(directionFlag){

		unsigned char* key;

		if(keyPath.length() == 0){
			key = fileHandler::genKey(keySize);
			fileHandler::storeKey(key, keySize);
		}
		else{
			key = fileHandler::readKey(keyPath, keySize);
		}

		// SINGLE FILE
		if(!dirFlag){
			fileHandler::AES_GCM(path, key, replaceFlag, keySize);

			if(replaceFlag){
				message += "\n"+fileHandler::getFileName(path)+" has been encrypted.\n";
				std::cout<<message;
			}
			else{
				message += "\n"+fileHandler::getFileName(path)+" has been encrypted.\n";
				message += "Find encrypted file in Downloads/target/\n";
				std::cout<<message;
			}
		}
		// DIRECTORY
		else{

			// Top direcotry
			std::string topDir = path.substr(0, path.size()-1);

			// Generate or read key
			unsigned char* key = (keyPath == "") ? fileHandler::genKey(keySize) : fileHandler::readKey(keyPath, keySize);

			// Create target directory in Downloads if no replace flag set
			if(!replaceFlag){
				if(!fileHandler::createRootDir()){
					std::cout<<"Could not create target Directory.";
					exit(3);
				}
			}

			// Check of top directory exists and valid
			if(std::filesystem::exists(topDir) && std::filesystem::is_directory(topDir)){

				// Iterate each file in sub directories
				for(const auto& entry : std::filesystem::recursive_directory_iterator(topDir)){

					// Skip hidden files
					if(std::filesystem::is_regular_file(entry) && entry.path().filename().string().front() != '.'){

						std::string outputPath;

						if(!replaceFlag){
							// Create relative path inside target directory
							outputPath = fileHandler::parsePath(entry.path().string(), path);

							// construct the path
							fileHandler::constructPath(outputPath);
						}

						// logs
						std::cout<<entry.path().string()<<std::endl;

						// encrypt entry
						fileHandler::encryptFile(entry.path().string(), outputPath, key, mode, keySize);
					}
				}

			}
			if(keyPath == ""){
				fileHandler::storeKey(key, keySize);
			}
			delete[] key;
		}
		
	}
	// DECRYPTION
	else{

		// SINGLE FILE
		if(!dirFlag){
			fileHandler::AES_GCM_DECRYPTION(path, fileHandler::readKey(keyPath, keySize), keySize);
			message += "\n"+fileHandler::getFileName(path)+" has been decrypted.\n";
			std::cout<<message;
			return 0;
		}
		// DIRECTORY
		else{
			// Top direcotry
			std::string topDir = path.substr(0, path.size()-1);

			// Check if directory exists
			if(std::filesystem::exists(topDir) && std::filesystem::is_directory(topDir)){

				// iterate each file/dir
				for(const auto& entry : std::filesystem::recursive_directory_iterator(topDir)){

					// skip hidden files
					if(std::filesystem::is_regular_file(entry) && entry.path().filename().string().front() != '.' && entry.path().filename().string() != "_key"){
						
						// logs
						std::cout<<entry.path().string()<<std::endl;

						// decrypt file
						fileHandler::decryptFile(entry.path().string(), fileHandler::readKey(keyPath, keySize), mode, keySize);
					}
				}

				message += "\nDecryption Finished.\n";
				std::cout<<message;

			}
		}
		return 0;
	}
}
