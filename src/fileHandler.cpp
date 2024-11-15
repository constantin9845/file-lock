#include "../include/fileHandler.h"

// Encrypt single file
void fileHandler::encryptFile(const std::string& path){

	// input file stream
	std::ifstream inputFile(path, std::ios::binary);

	// construct output file path
	std::string outputPath = getOutputPath("_"+getFileName(path), true);

	// output file stream
	std::ofstream outputFile(outputPath, std::ios::binary);

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
	unsigned char* buffer = new unsigned char[size+padding];

	// read data into buffer
	if(!inputFile.read(reinterpret_cast<char*>(buffer),size)){
		std::cout<<"error";
		delete[] buffer;
		exit(2);
	}

	// generate 128 bit key
	unsigned char* key = genKey();

	// encrypt data in buffer
	for(int i = 0; i < size+padding; i+=16){
		unsigned char temp[16];
		unsigned char tempResult[16];

		// load 16 bytes
		for(int j = 0, index = i; j < 16; j++, index++){
			temp[j] = buffer[index];
		}
		
		AES::encrypt(temp, tempResult, key);

		// store encrypted back in buffer
		for(int j = 0, index = i; j < 16; j++, index++){
			buffer[index] = tempResult[j];
		}

	}

	// write encrypted data to output file
	if(!outputFile.write(reinterpret_cast<char*>(buffer), size+padding)){
		delete[] buffer;
		std::cout<<"error write";
		exit(3);
	}

	// store new key together with file
	storeKey(key);

	inputFile.close();
	outputFile.close();

	delete[] key;
	delete[] buffer;
}

// Function encrypts a file but is used for full directory encryption
// uses user provided key
void fileHandler::encryptFile(const std::string& path, const std::string& outputPath, bool dirFlag, unsigned char* key){

	// input stream
	std::ifstream inputFile(path, std::ios::binary);

	// output stream
	std::ofstream outputFile(outputPath, std::ios::binary);

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
	unsigned char* buffer = new unsigned char[size+padding];

	// read data into buffer
	if(!inputFile.read(reinterpret_cast<char*>(buffer),size)){
		std::cout<<"error";
		delete[] buffer;
		exit(2);
	}

	// encrypt data in the buffer
	for(int i = 0; i < size+padding; i+=16){
		unsigned char temp[16];
		unsigned char tempResult[16];

		// load 16 bytes
		for(int j = 0, index = i; j < 16; j++, index++){
			temp[j] = buffer[index];
		}
		
		AES::encrypt(temp, tempResult, key);

		// store encrypted back in buffer
		for(int j = 0, index = i; j < 16; j++, index++){
			buffer[index] = tempResult[j];
		}

	}

	// write data to output file
	if(!outputFile.write(reinterpret_cast<char*>(buffer), size+padding)){
		delete[] buffer;
		std::cout<<"error write";
		exit(3);
	}

	inputFile.close();
	outputFile.close();

	delete[] buffer;
}

// Encrypt file with user provided key
void fileHandler::encryptFile(const std::string& path, const std::string& keyPath){
	// input stream
	std::ifstream inputFile(path, std::ios::binary);

	// construct output path
	std::string outputPath = getOutputPath("_"+getFileName(path), true);

	// output stream
	std::ofstream outputFile(outputPath, std::ios::binary);

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
	unsigned char* buffer = new unsigned char[size+padding];

	// read data into buffer
	if(!inputFile.read(reinterpret_cast<char*>(buffer),size)){
		std::cout<<"error";
		delete[] buffer;
		exit(2);
	}

	// generate key
	unsigned char* key = readKey(keyPath);

	// encrypt buffer
	for(int i = 0; i < size+padding; i+=16){
		unsigned char temp[16];
		unsigned char tempResult[16];

		// load 16 bytes
		for(int j = 0, index = i; j < 16; j++, index++){
			temp[j] = buffer[index];
		}
		
		AES::encrypt(temp, tempResult, key);

		// store encrypted back in buffer
		for(int j = 0, index = i; j < 16; j++, index++){
			buffer[index] = tempResult[j];
		}

	}

	// write data to output file
	if(!outputFile.write(reinterpret_cast<char*>(buffer), size+padding)){
		delete[] buffer;
		std::cout<<"error write";
		exit(3);
	}

	inputFile.close();
	outputFile.close();

	delete[] key;
	delete[] buffer;
}

// Decrypt single file
// Requires user key
// replaces input file
void fileHandler::decryptFile(const std::string& path, const std::string& keyPath){

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

	// padding needed for AES
	std::streamsize padding = 16-(size%16);

	// array to store bytes 
	unsigned char* buffer = new unsigned char[size];

	// read data into buffer
	if(!inputFile.read(reinterpret_cast<char*>(buffer),size)){
		std::cout<<"error";
		delete[] buffer;
		exit(2);
	}

	// read user provided key file
	unsigned char* key = readKey(keyPath);
	
  	// decrypt buffer
	for(int i = 0; i < size; i+=16){
		unsigned char temp[16];
		unsigned char tempResult[16];

		// load 16 bytes
		for(int j = 0, index = i; j < 16; j++, index++){
			temp[j] = buffer[index];
		}
		
		AES::decrypt(temp, tempResult, key);

		// store encrypted back in buffer
		for(int j = 0, index = i; j < 16; j++, index++){
			if(i < size){
				buffer[index] = tempResult[j];
			}
		}

	}


	// delete old encrypted file
	inputFile.close();
	std::filesystem::remove(path);

	// output stream
	std::ofstream outputFile(outputPath, std::ios::binary);


	// write to output file
	if(!outputFile.write(reinterpret_cast<char*>(buffer), size)){
		delete[] buffer;
		std::cout<<"error write";
		exit(3);
	}

	outputFile.close();

	delete[] key;
	delete[] buffer;
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
unsigned char* fileHandler::genKey(){

	unsigned char* buffer = new unsigned char[16];

	// check OS of user
#ifdef _WIN32

	// WINDOWS
	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_int_distribution<> distr(0, 255);

	for(int i = 0; i < 16; i++){
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

	urandom.read(reinterpret_cast<char*>(buffer),16);

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
unsigned char* fileHandler::readKey(const std::string& path){

	unsigned char* buffer = new unsigned char[16];

	std::ifstream keyFileStream(path, std::ios::binary);

	if(!keyFileStream){
		std::cout<<"error opening key file";
		delete[] buffer;
		exit(3);
	}


	if(!keyFileStream.read(reinterpret_cast<char*>(buffer), 16)){
		std::cout<<"error reading key file";
		delete[] buffer;
		exit(3);
	}

	keyFileStream.close();
	return buffer;
}

// store key in key file
void fileHandler::storeKey(unsigned char* key){

	std::string outputPath = getOutputPath("_key", false);

	std::ofstream keyOutput(outputPath, std::ios::binary);

	if(!keyOutput){
		std::cout<<"key file output error";
		exit(3);
	}

	if(!keyOutput.write(reinterpret_cast<char*>(key), 16)){
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


// Given filapth create a relative path to Root directory in encryption directory
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

