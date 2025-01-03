#include "../include/fileHandler.h"

// Encrypt single file
void fileHandler::encryptFile(const std::string& path, bool replaceFlag, bool mode, int keySize){

	// input file stream
	std::ifstream inputFile(path, std::ios::binary);

	std::string outputPath;

	if(replaceFlag){
		// create output path -> same as input
		outputPath = getDecryptionFileName(path);
	}
	else{
		// construct output file path
		outputPath = getOutputPath("_"+getFileName(path), true);
	}

	if(!inputFile){
		std::cout<<"Error opening file.";
		exit(2);
	}

	// get size of file
	inputFile.seekg(0, std::ios::end);
	std::streamsize size = inputFile.tellg();
	inputFile.seekg(0, std::ios::beg);

	// padding needed for AES
	std::streamsize padding = 16-(size%16);

	unsigned char* IV = nullptr;
	unsigned char* key = genKey(keySize);

	// array to store bytes 
	unsigned char* buffer;

	// CBC mode
	if(mode){
		// generate IV
		IV = genKey(128);

		// array to store bytes 
		buffer = new unsigned char[size+padding+16];
	}
	else{
		// array to store bytes 
		buffer = new unsigned char[size+padding];
	}

	// read data into buffer
	if(!inputFile.read(reinterpret_cast<char*>(buffer),size)){
		std::cout<<"Could not read data into buffer";
		delete[] buffer;
		exit(2);
	}
		
	// Encrypt in CBC mode
	if(mode){
		// ENCRYPTION OF FIRST BLOCK USES IV
		// REST OF ENCRYPTION USES PREVIOUS BLOCK OUTPUT
		for(int i = 0; i < size+padding; i+=16){
			unsigned char temp[16];
			unsigned char tempResult[16];

			// load 16 bytes
			for(int j = 0, index = i; j < 16; j++, index++){
				temp[j] = buffer[index];
			}

			// first block -> IV
			if(i == 0){
				AES::encryptCBC(temp, tempResult, key, IV, keySize);
			}
			else{

				// get previous output
				unsigned char* prev = new unsigned char[16];
				for(int l = i-16, s = 0; l < i; l++, s++){
					prev[s] = buffer[l];
				}

				AES::encryptCBC(temp, tempResult, key, prev, keySize);
				delete[] prev;
			}

			// store encrypted back in buffer
			for(int j = 0, index = i; j < 16; j++, index++){
				buffer[index] = tempResult[j];
			}
		}

		// Store IV at end of buffer
		for(int i = 0; i < 16; i++){
			buffer[size+padding+i] = IV[i];
		}

	}
	// Encrypt in ECB mode
	else{
		for(int i = 0; i < size+padding; i+=16){
			unsigned char temp[16];
			unsigned char tempResult[16];

			// load 16 bytes
			for(int j = 0, index = i; j < 16; j++, index++){
				temp[j] = buffer[index];
			}

			AES::encrypt(temp, tempResult, key, keySize);


			// store encrypted back in buffer
			for(int j = 0, index = i; j < 16; j++, index++){
				buffer[index] = tempResult[j];
			}
		}
	}

	

	if(replaceFlag){
		inputFile.close();
		std::filesystem::remove(path);
	}
	else{
		inputFile.close();
	}

	// output file stream
	std::ofstream outputFile(outputPath, std::ios::binary);

	// CBC mode
	if(mode){
		// write encrypted data to output file
		if(!outputFile.write(reinterpret_cast<char*>(buffer), size+padding+16)){
			delete[] buffer;
			delete[] key;
			delete[] IV;
			std::cout<<"error write";
			exit(3);
		}
	}
	// ECB mode
	else{
		// write encrypted data to output file
		if(!outputFile.write(reinterpret_cast<char*>(buffer), size+padding)){
			delete[] buffer;
			delete[] key;
			std::cout<<"error write";
			exit(3);
		}
	}

	

	// store new key together with file
	storeKey(key, keySize);

	outputFile.close();

	delete[] key;
	delete[] IV;
	delete[] buffer;
}

// Function encrypts a file but is used for full directory encryption
// uses user provided key
void fileHandler::encryptFile(const std::string& path, std::string outputPath, bool dirFlag, unsigned char* key, bool replaceFlag, bool mode, int keySize){

	// input stream
	std::ifstream inputFile(path, std::ios::binary);


	if(replaceFlag){
		// create output path -> same as input
		outputPath = getDecryptionFileName(path);
	}

	if(!inputFile){
		std::cout<<"Error opening file.";
		exit(2);
	}

	// get size of file
	inputFile.seekg(0, std::ios::end);
	std::streamsize size = inputFile.tellg();
	inputFile.seekg(0, std::ios::beg);


	// padding needed for AES
	std::streamsize padding = 16-(size%16);

	// array to store bytes 
	unsigned char* buffer;
	unsigned char* IV = nullptr;


	if(mode){
		// generate IV
		IV = genKey(128);

		buffer = new unsigned char[size+padding+16];
	}
	else{
		buffer = new unsigned char[size+padding];
	}

	// read data into buffer
	if(!inputFile.read(reinterpret_cast<char*>(buffer),size)){
		std::cout<<"error";
		delete[] buffer;
		exit(2);
	}

	if(mode){
		// ENCRYPTION OF FIRST BLOCK USES IV
		// REST OF ENCRYPTION USES PREVIOUS BLOCK OUTPUT
		for(int i = 0; i < size+padding; i+=16){
			unsigned char temp[16];
			unsigned char tempResult[16];

			// load 16 bytes
			for(int j = 0, index = i; j < 16; j++, index++){
				temp[j] = buffer[index];
			}

			// first block -> IV
			if(i == 0){
				AES::encryptCBC(temp, tempResult, key, IV, keySize);
			}
			else{

				// get previous output
				unsigned char* prev = new unsigned char[16];
				for(int l = i-16, s = 0; l < i; l++, s++){
					prev[s] = buffer[l];
				}

				AES::encryptCBC(temp, tempResult, key, prev, keySize);
				delete[] prev;
			}

			// store encrypted back in buffer
			for(int j = 0, index = i; j < 16; j++, index++){
				buffer[index] = tempResult[j];
			}
		}

		// Store IV at end of buffer
		for(int i = 0; i < 16; i++){
			buffer[size+padding+i] = IV[i];
		}
	}
	else{
		for(int i = 0; i < size+padding; i+=16){
			unsigned char temp[16];
			unsigned char tempResult[16];

			// load 16 bytes
			for(int j = 0, index = i; j < 16; j++, index++){
				temp[j] = buffer[index];
			}

			AES::encrypt(temp, tempResult, key, keySize);

			// store encrypted back in buffer
			for(int j = 0, index = i; j < 16; j++, index++){
				buffer[index] = tempResult[j];
			}
		}
	}

	

	if(replaceFlag){
		inputFile.close();
		std::filesystem::remove(path);
	}
	else{
		inputFile.close();
	}

	// output file stream
	std::ofstream outputFile(outputPath, std::ios::binary);

	
	if(mode){
		// write data to output file
		if(!outputFile.write(reinterpret_cast<char*>(buffer), size+padding+16)){
			delete[] buffer;
			delete[] IV;
			std::cout<<"error write";
			exit(3);
		}
	}
	else{
		// write data to output file
		if(!outputFile.write(reinterpret_cast<char*>(buffer), size+padding)){
			delete[] buffer;
			delete[] IV;
			std::cout<<"error write";
			exit(3);
		}
	}
	

	outputFile.close();

	delete[] IV;
	delete[] buffer;
}

// Encrypt file with user provided key
void fileHandler::encryptFile(const std::string& path, const std::string& keyPath, bool replaceFlag, bool mode, int keySize){
	// input stream
	std::ifstream inputFile(path, std::ios::binary);

	std::string outputPath;

	if(replaceFlag){
		// create output path -> same as input
		outputPath = getDecryptionFileName(path);
	}
	else{
		// construct output file path
		outputPath = getOutputPath("_"+getFileName(path), true);
	}

	if(!inputFile){
		std::cout<<"Error opening file.";
		exit(2);
	}

	// get size of file
	inputFile.seekg(0, std::ios::end);
	std::streamsize size = inputFile.tellg();
	inputFile.seekg(0, std::ios::beg);

	// padding needed for AES
	std::streamsize padding = 16-(size%16);

	// array to store bytes 
	unsigned char* buffer;

	unsigned char* IV = nullptr;
	unsigned char* key = readKey(keyPath, keySize);

	if(mode){
		// generate IV
		IV = genKey(128);

		buffer = new unsigned char[size+padding+16];
	}
	else{ 
		buffer = new unsigned char[size+padding];
	}

	// read data into buffer
	if(!inputFile.read(reinterpret_cast<char*>(buffer),size)){
		std::cout<<"Could not read data into buffer";
		delete[] buffer;
		exit(2);
	}

	// CBC mode
	if(mode){
		// ENCRYPTION OF FIRST BLOCK USES IV
		// REST OF ENCRYPTION USES PREVIOUS BLOCK OUTPUT
		for(int i = 0; i < size+padding; i+=16){
			unsigned char temp[16];
			unsigned char tempResult[16];

			// load 16 bytes
			for(int j = 0, index = i; j < 16; j++, index++){
				temp[j] = buffer[index];
			}

			// first block -> IV
			if(i == 0){
				AES::encryptCBC(temp, tempResult, key, IV, keySize);
			}
			else{

				// get previous output
				unsigned char* prev = new unsigned char[16];
				for(int l = i-16, s = 0; l < i; l++, s++){
					prev[s] = buffer[l];
				}

				AES::encryptCBC(temp, tempResult, key, prev, keySize);
				delete[] prev;
			}

			// store encrypted back in buffer
			for(int j = 0, index = i; j < 16; j++, index++){
				buffer[index] = tempResult[j];
			}

		}

		// Store IV at end of buffer
		for(int i = 0; i < 16; i++){
			buffer[size+padding+i] = IV[i];
		}
	}
	else{
		for(int i = 0; i < size+padding; i+=16){
			unsigned char temp[16];
			unsigned char tempResult[16];

			// load 16 bytes
			for(int j = 0, index = i; j < 16; j++, index++){
				temp[j] = buffer[index];
			}

			AES::encrypt(temp, tempResult, key, keySize);


			// store encrypted back in buffer
			for(int j = 0, index = i; j < 16; j++, index++){
				buffer[index] = tempResult[j];
			}
		}
	}

	

	if(replaceFlag){
		inputFile.close();
		std::filesystem::remove(path);
	}
	else{
		inputFile.close();
	}

	// output file stream
	std::ofstream outputFile(outputPath, std::ios::binary);

	if(mode){
		// write data to output file
		if(!outputFile.write(reinterpret_cast<char*>(buffer), size+padding+16)){
			delete[] buffer;
			delete[] key;
			delete[] IV;
			std::cout<<"error write";
			exit(3);
		}
	}
	else{
		// write data to output file
		if(!outputFile.write(reinterpret_cast<char*>(buffer), size+padding)){
			delete[] buffer;
			delete[] key;
			delete[] IV;
			std::cout<<"error write";
			exit(3);
		}
	}

	

	outputFile.close();

	delete[] buffer;
	delete[] key;
	delete[] IV;
}

// Decrypt single file
// Requires user key 
// replaces input file
void fileHandler::decryptFile(const std::string& path, const std::string& keyPath, bool mode, int keySize){

	// input stream
	std::ifstream inputFile(path, std::ios::binary);

	// create output path -> same as input
	std::string outputPath = getDecryptionFileName(path);


	if(!inputFile){
		std::cout<<"Error opening file.";
		exit(2);
	}

	// get size of file
	inputFile.seekg(0, std::ios::end);
	std::streamsize size = inputFile.tellg();
	inputFile.seekg(0, std::ios::beg);

	// array to store bytes 
	unsigned char* buffer = new unsigned char[size];
	unsigned char* output;

	if(mode){
		output = new unsigned char[size-16];
	}
	else{
		output = new unsigned char[size];
	}

	// read data into buffer
	if(!inputFile.read(reinterpret_cast<char*>(buffer),size)){
		std::cout<<"error";
		delete[] buffer;
		exit(2);
	}

	unsigned char* key = readKey(keyPath, keySize);
	unsigned char* IV = nullptr;

	if(mode){
		IV = new unsigned char[16];
		for(int i = size-16, k = 0; i < size; i++, k++){
			IV[k] = buffer[i];
		}

		// decrypt buffer
		for(int i = 0; i < size-16; i+=16){
			unsigned char temp[16];
			unsigned char tempResult[16];

			// load 16 bytes
			for(int j = 0, index = i; j < 16; j++, index++){
				temp[j] = buffer[index];
			}

			// check if first block -> apply IV
			if(i == 0){
				AES::decryptCBC(temp, tempResult, key, IV, keySize);
			}
			else{

				// get previous output
				unsigned char* prev = new unsigned char[16];
				for(int l = i-16, s = 0; l < i; l++, s++){
					prev[s] = buffer[l];
				}

				AES::decryptCBC(temp, tempResult, key, prev, keySize);
				delete[] prev;
			}

			// store decrypted text in output
			for(int j = 0, index = i; j < 16; j++, index++){
				if(i < size){
					output[index] = tempResult[j];
				}
			}
		}
	}
	// ECB mode
	else{
		// decrypt buffer
		for(int i = 0; i < size; i+=16){
			unsigned char temp[16];
			unsigned char tempResult[16];

			// load 16 bytes
			for(int j = 0, index = i; j < 16; j++, index++){
				temp[j] = buffer[index];
			}

			AES::decrypt(temp, tempResult, key, keySize);

			// store decrypted text in output
			for(int j = 0, index = i; j < 16; j++, index++){
				if(i < size){
					output[index] = tempResult[j];
				}
			}
		}
	}

	
	// delete old encrypted file
	inputFile.close();
	std::filesystem::remove(path);

	// output stream
	std::ofstream outputFile(outputPath, std::ios::binary);

	if(mode){
		// write to output file
		if(!outputFile.write(reinterpret_cast<char*>(output), size-16)){
			delete[] output;
			delete[] buffer;
			delete[] IV;
			std::cout<<"Could not write buffer to output file";
			exit(3);
		}
	}
	else{
		// write to output file
		if(!outputFile.write(reinterpret_cast<char*>(output), size)){
			delete[] output;
			delete[] buffer;
			delete[] IV;
			std::cout<<"Could not write buffer to output file";
			exit(3);
		}
	}

	outputFile.close();

	delete[] key;
	delete[] buffer;
	delete[] IV;
	delete[] output;
}

// return filename given an absolute path
std::string fileHandler::getFileName(const std::string& filePath){
	// extract file name from file path
	std::filesystem::path path(filePath);
	std::string fileName = path.filename().string();
	return fileName;
}

// return path of current file and remove _ in name
std::string fileHandler::getDecryptionFileName(const std::string& filePath){

	std::string newName;

	// extract file name from file path
	std::filesystem::path path(filePath);
	std::string fileName = path.filename().string();

	if(fileName[0] == '_'){
		fileName = fileName.substr(1);
	}

#ifdef _WIN32
	newName = path.parent_path().string() + "\\" +fileName;
#else

	newName = path.parent_path().string() + "/" +fileName;


#endif
	return newName;
}

// construct output path
// target directory in Downloads
std::string fileHandler::getOutputPath(const std::string& fileName, bool deleteOld){

// check OS
// windows
#ifdef _WIN32

	// get user name
	const char* homeDir = std::getenv("USERPROFILE");

	if(homeDir == nullptr){
		std::cerr << "Failed to get USERPROFILE environment variable." << std::endl;
        exit(1);
	}

	// construct target folder path
	std::string targetFolder = std::string(homeDir) + "\\Downloads\\target\\";
	std::string outputPath = targetFolder+fileName;

	// delete existing target directory
	if(deleteOld){
		if(std::filesystem::exists(targetFolder)){
			std::filesystem::remove_all(targetFolder);
		}

		if(!std::filesystem::create_directory(targetFolder)){
			std::cout<<"dir not created";
			exit(10);
		}
	}

	return outputPath;

// Mac/Linux
#else

	// get user name
	const char* homeDir = std::getenv("HOME");

	if(homeDir == nullptr){
		std::cerr << "Failed to get HOME environment variable." << std::endl;
		exit(1);
	}

	// create new folder to store file + key
	std::string targetFolder = std::string(homeDir) + "/Downloads/target/";
	std::string outputPath = targetFolder+fileName;

	// delete existing directory
	if(deleteOld){
		if(std::filesystem::exists(targetFolder)){
			std::filesystem::remove_all(targetFolder);
		}

		if(!std::filesystem::create_directory(targetFolder)){
			std::cout<<"dir not created";
			exit(10);
		}
	}

	return outputPath;

#endif
}

// Generate 128 bit key
unsigned char* fileHandler::genKey(const int& keySize){

	unsigned char* buffer = new unsigned char[keySize/8];

	// check OS of user
#ifdef _WIN32

	// WINDOWS
	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_int_distribution<> distr(0, 255);

	for(int i = 0; i < keySize/8; i++){
		buffer[i] = static_cast<unsigned char>(distr(eng));
	}
	

#else
	// MAC / LINUX

	std::ifstream urandom("/dev/urandom", std::ios::binary);
	if(!urandom){
		std::cout<<"error opening urandom";
		delete[] buffer;
		exit(3);
	}

	urandom.read(reinterpret_cast<char*>(buffer),keySize/8);

	if(!urandom){
		std::cout<<"error reading urandom";
		delete[] buffer;
		exit(3);
	}

	urandom.close();

#endif

	return buffer;
}

// read key from key file
unsigned char* fileHandler::readKey(const std::string& path, const int& keySize){

	unsigned char* buffer = new unsigned char[keySize/8];

	std::ifstream keyFileStream(path, std::ios::binary);

	if(!keyFileStream){
		std::cout<<"error opening key file";
		delete[] buffer;
		exit(3);
	}


	if(!keyFileStream.read(reinterpret_cast<char*>(buffer), keySize/8)){
		std::cout<<"error reading key file";
		delete[] buffer;
		exit(3);
	}

	keyFileStream.close();
	return buffer;
}

// store key in key file
void fileHandler::storeKey(unsigned char* key, const int& keySize){

	std::string outputPath = getOutputPath("_key", false);

	std::ofstream keyOutput(outputPath, std::ios::binary);

	if(!keyOutput){
		std::cout<<"key file output error";
		exit(3);
	}

	if(!keyOutput.write(reinterpret_cast<char*>(key), keySize/8)){
		std::cout<<"error key file write";
		exit(3);
	}

	keyOutput.close();
}

// create target directory
bool fileHandler::createRootDir(){
	// check OS
// windows
#ifdef _WIN32

	// get user name
	const char* homeDir = std::getenv("USERPROFILE");

	if(homeDir == nullptr){
		std::cerr << "Failed to get USERPROFILE environment variable." << std::endl;
        exit(1);
	}

	// construct path
	std::string targetFolder = std::string(homeDir) + "\\Downloads\\target\\";

	// delete if exists
	if(std::filesystem::exists(targetFolder)){
		std::filesystem::remove_all(targetFolder);
	}

	// create target directory
	if(!std::filesystem::create_directory(targetFolder)){
		return false;
	}

	return true;

// Mac/Linux
#else

	// get user name
	const char* homeDir = std::getenv("HOME");

	if(homeDir == nullptr){
		std::cerr << "Failed to get HOME environment variable." << std::endl;
		exit(1);
	}

	// create new folder to store file + key
	std::string targetFolder = std::string(homeDir) + "/Downloads/target/";

	// delete if already exists
	if(std::filesystem::exists(targetFolder)){
		std::filesystem::remove_all(targetFolder);
	}

	// create new target folder
	if(!std::filesystem::create_directory(targetFolder)){
		return false;
	}

	return true;

#endif
}


// Given file path create a relative path to Root directory in encryption directory
std::string fileHandler::parsePath(const std::string& filePath, const std::string& path){

	std::string relativePath = filePath.substr(path.size()-1);


#ifdef _WIN32

	// get username
	const char* homeDir = std::getenv("USERPROFILE");

	if(homeDir == nullptr){
		std::cerr << "Failed to get USERPROFILE environment variable." << std::endl;
        exit(1);
	}

	std::string targetFolder = std::string(homeDir) + "\\Downloads\\target";


	return targetFolder+relativePath;


#else

	std::string outputPath;

	// get username
	const char* homeDir = std::getenv("HOME");

	if(homeDir == nullptr){
		std::cerr << "Failed to get HOME environment variable." << std::endl;
		exit(1);
	}

	std::string targetFolder = std::string(homeDir) + "/Downloads/target/";

	outputPath = targetFolder+relativePath;

	return outputPath;

#endif
}

// create directory for provided path
void fileHandler::constructPath(const std::string& filePath){
	std::filesystem::path t(filePath);

	std::string temp = t.parent_path().string();


#ifdef _WIN32
	temp+= "\\";
#else
	temp += "/";
#endif

	std::filesystem::create_directories(temp);
}

