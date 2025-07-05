#include "../include/fileHandler.h"
#include <chrono>

#ifdef _WIN32
	#include <intrin.h>  
#endif

void guide(){
	std::cout<<"GUIDE";
}

void menu(std::string& file, bool& directionFlag, int& keySize, std::string& keyPath, bool& replaceFlag, std::string& AD, bool& authTag){

	// PATH
	std::cout<<"\nEnter <absolute> file/directory path: ";
	std::cin>>file;

	// ENC / DEC
	std::string direction;
	while(direction != "e" && direction != "d"){
		std::cout<<"\nEncryption / Decryption (enter e or d): ";
		std::cin>>direction;
	}
	if(direction == "e"){ directionFlag = true; }
	else{ directionFlag = false; }

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


	// AUTH TAG
	std::string temp;
	if(!directionFlag){
		while(temp == ""){
			std::cout<<"\nVerify Authentication tag(s)? (y or n): ";
			std::cin>>temp;
		}
	}
	else{
		while(temp != "y" && temp != "n"){
			std::cout<<"\nGenerate Authentication Tag(s)? (y or n): ";
			std::cin>>temp;
		}
	}
	authTag = (temp == "y");

	std::cin.ignore();

	//ADDITIONAL DATA for new TAG
	std::string ad = "";
	if(authTag){
		if(!directionFlag){
			while(ad != "y" && ad != "n"){
				std::cout<<"\nAdditional Message? (y or n): ";
				std::cin>>ad;
			}
		}
		else{
			while(ad == ""){
				std::cout<<"\nEnter Additional Message?: ";
				std::getline(std::cin, ad);
			}
		}
		
	}
	AD = ad;

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
	summary += "\n\tSUMMARY\nFile(s)                     : "+file;
	
	if(directionFlag){ summary += "\nType                        : Encryption"; }
	else{ summary += "\nType                        : Decryption"; }

	if(keySize == 128){ summary += "\nKey Size                    : 128"; }
	else if(keySize == 192){ summary += "\nKey Size                    : 192"; }
	else{ summary += "\nKey Size                    : 256"; }

	if(keyPath != ""){ summary += "\nKey File                    : "+keyPath; }
	else{ summary += "\nKey File                    : New key to be generated"; }

	if(authTag){
		if(AD.size() > 0 && AD != "n"){
			if(directionFlag){ 
				summary += "\nGenerate Authentication Tag : YES"; 
				summary += "\nAdditional Message          : YES";
			}
			else{
				summary += "\nAuthenticate Tag            : YES";
				summary += "\nAdditional Message          : YES";
			}
		}
		else{
			if(directionFlag){ 
				summary += "\nGenerate Authentication Tag : YES"; 
				summary += "\nAdditional Message          : NO";
			}
			else{
				summary += "\nAuthenticate Tag            : YES";
				summary += "\nAdditional Message          : NO";
			}
		}
	}
	else{
		if(directionFlag){ 
				summary += "\nGenerate Authentication Tag : NO"; 
				summary += "\nAdditional Message          : NO";
			}
			else{
				summary += "\nAuthenticate Tag            : NO";
				summary += "\nAdditional Message          : NO";
			}
	}

	if(replaceFlag){ summary += "\nReplace file(s)             : TRUE"; }
	else{ summary += "\nReplace file(s)             : FALSE"; }



	summary += "\n\n\tNOTE\n";
	summary += "*****************************************************";
	summary += "\n* Decrypting files with the wrong key damages files *";
	summary += "\n* If files broken after decryption:                 *";
	summary += "\n*\t1) Encrypt with same params and key.        *";
	summary += "\n*\t2) This returns files to initial state.     *";
	summary += "\n* Then retry with correct key.                      *";
	summary += "\n*                                                   *";
	summary += "\n* The same additional data message that was         *";
	summary += "\n* used to generate the authentication tag           *";
	summary += "\n* is needed to authenticate the tag.                *";
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


#ifdef _WIN32
	bool check_cpu_win(){
		int cpu_info[4];
		__cpuid(cpu_info, 1);
		
		return (cpu_info[2] & (1 << 25)) != 0; // check bit 25
	}
#endif

int main(int argc, char const *argv[]){

	std::ios_base::sync_with_stdio(false);

	std::string path = "";
	bool directionFlag;
	int keySize;
	std::string keyPath = "";
	bool replaceFlag = false;
	std::string AD = "";
	bool authTag;

	bool dirFlag;

	menu(path, directionFlag, keySize, keyPath, replaceFlag, AD, authTag);


	// Check if single file or directory
	std::filesystem::path t(path);
	std::string star = t.filename().string();
	std::string outputFilePath = "";

	// CHeck for AES hardware support
	bool hw_available = false;
#ifdef _WIN32
	hw_available = check_cpu_win();

#elif defined(__x86_64__) || defined(__i386__)
	hw_available = __builtin_cpu_supports("aes");
#endif

	std::cout<<"-----------"<<hw_available<<"---------"<<std::endl;



#ifdef _WIN32

	// set directory flag
	if(star.size() == 0){ dirFlag = true; }else{ dirFlag = false; }

	if(!dirFlag && !replaceFlag){
		// get username
		const char* homeDir = std::getenv("USERPROFILE");

		if(homeDir == nullptr){
			std::cerr << "Failed to get USERPROFILE environment variable." << std::endl;
			exit(1);
		}

		outputFilePath = std::string(homeDir) + "\\Downloads\\target\\"+star;
	}

#else

	// set directory flag
	if(star == "*"){ dirFlag = true; }else{ dirFlag = false; }

	if(!dirFlag && !replaceFlag){
		// get username
		const char* homeDir = std::getenv("HOME");

		if(homeDir == nullptr){
			std::cerr << "Failed to get HOME environment variable." << std::endl;
			exit(1);
		}

		outputFilePath = std::string(homeDir) + "/Downloads/target/"+star;
	}

#endif

	std::string message = "\n\n";

	// ENCRYPTION
	if(directionFlag){

		std::cout<<"Starting Encryption..."<<std::endl;

		unsigned char* key;

		if(keyPath.length() == 0){
			key = fileHandler::genKey(keySize);
			fileHandler::storeKey(key, keySize);
		}
		else{
			key = fileHandler::readKey(keyPath, keySize);
		}

		// Create target directory in Downloads if no replace flag set
		if(!replaceFlag){
			if(!fileHandler::createRootDir()){
				std::cout<<"Could not create target Directory.";
				exit(3);
			}
		}

		// SINGLE FILE
		if(!dirFlag){

			auto start = std::chrono::high_resolution_clock::now();

			std::cout<<"-- "<<path<<std::endl;

			if(hw_available){
				fileHandler::HW_AES_GCM(path, key, replaceFlag, keySize, outputFilePath, authTag, AD);
			}
			else{
				fileHandler::AES_GCM(path, key, replaceFlag, keySize, outputFilePath, authTag, AD);
			}


			auto end = std::chrono::high_resolution_clock::now();

			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

			std::cout<<"Finished in : "<<duration.count() << " ms"<<std::endl;

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

			// Check of top directory exists and valid
			if(std::filesystem::exists(topDir) && std::filesystem::is_directory(topDir)){

				auto start = std::chrono::high_resolution_clock::now();

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
						std::cout<<"-- "<<entry.path().string()<<std::endl;

						// encrypt entry
						if(hw_available){
							fileHandler::HW_AES_GCM(path, key, replaceFlag, keySize, outputFilePath, authTag, AD);
						}
						else{
							fileHandler::AES_GCM(path, key, replaceFlag, keySize, outputFilePath, authTag, AD);
						}
					}
				}

				auto end = std::chrono::high_resolution_clock::now();

				auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

				std::cout<<"Encryption Done."<<std::endl;

				std::cout<<"Finished in : "<<duration.count() << " ms"<<std::endl;
			}
			delete[] key;
		}
		
	}
	// DECRYPTION
	else{

		std::cout<<"Starting Decryption..."<<std::endl;

		unsigned char* k = fileHandler::readKey(keyPath, keySize);

		// SINGLE FILE
		if(!dirFlag){

			std::cout<<"-- "<<path<<std::endl;

			auto start = std::chrono::high_resolution_clock::now();
			

			if(hw_available){
				fileHandler::HW_AES_GCM_DECRYPTION(path, k, keySize, authTag, AD=="y");
			}
			else{
				fileHandler::AES_GCM_DECRYPTION(path, k, keySize, authTag, AD=="y");
			}

			
			auto end = std::chrono::high_resolution_clock::now();
			
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

			message += "\n"+fileHandler::getFileName(path)+" has been decrypted.\n";
			std::cout<<message;

			std::cout<<"Finished in : "<<duration.count() << " ms"<<std::endl;

			return 0;
		}
		// DIRECTORY
		else{
			// Top direcotry
			std::string topDir = path.substr(0, path.size()-1);

			// Check if directory exists
			if(std::filesystem::exists(topDir) && std::filesystem::is_directory(topDir)){

				auto start = std::chrono::high_resolution_clock::now();

				// iterate each file/dir
				for(const auto& entry : std::filesystem::recursive_directory_iterator(topDir)){

					// skip hidden files, AUTH tags, and AD text files
					if(
						std::filesystem::is_regular_file(entry) && 
						entry.path().filename().string().front() != '.' && 
						entry.path().filename().string() != "_key" &&
						entry.path().filename().string().find("_Additional_Message.txt") == std::string::npos &&
						entry.path().filename().string().find("_TAG") == std::string::npos
					){
						
						// logs
						std::cout<<"-- "<<entry.path().string()<<std::endl;
						
						if(hw_available){
							fileHandler::HW_AES_GCM_DECRYPTION(entry.path().string(), k, keySize, authTag, (AD == "y"));
						}
						else{
							fileHandler::AES_GCM_DECRYPTION(entry.path().string(), k, keySize, authTag, (AD == "y"));
						}
					}
				}


				auto end = std::chrono::high_resolution_clock::now();

				auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

				std::cout<<"Decryption Done."<<std::endl;

				std::cout<<"Finished in : "<<duration.count() << " ms"<<std::endl;

			}
		}
		return 0;
	}
}
