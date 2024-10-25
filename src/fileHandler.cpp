#include "../include/fileHandler.h"

void fileHandler::encryptFile(const std::string& path){
	
	std::ifstream inputFile(path, std::ios::binary);

	std::string outputPath = getOutputPath("_"+getFileName(path), true);

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

	unsigned char* key = genKey();

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

	if(!outputFile.write(reinterpret_cast<char*>(buffer), size+padding)){
		delete[] buffer;
		std::cout<<"error write";
		exit(3);
	}

	storeKey(key);

	inputFile.close();
	outputFile.close();

	delete[] buffer;
}

void fileHandler::decryptFile(const std::string& path){
	
	std::ifstream inputFile(path, std::ios::binary);

	std::string outputPath = getOutputPath(getFileName(path).substr(1), false);

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
	unsigned char* buffer = new unsigned char[size];

	// read data into buffer
	if(!inputFile.read(reinterpret_cast<char*>(buffer),size)){
		std::cout<<"error";
		delete[] buffer;
		exit(2);
	}

	unsigned char key[16] = {
		0x2b, 0x7e, 0x15, 0x16,
		0x28, 0xae, 0xd2, 0xa6,
		0xab, 0xf7, 0x15, 0x88,
		0x09, 0xcf, 0x4f, 0x3c
    };

  
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

	// padding needs to be ignored!
	if(!outputFile.write(reinterpret_cast<char*>(buffer), size)){
		delete[] buffer;
		std::cout<<"error write";
		exit(3);
	}

	inputFile.close();
	outputFile.close();

	delete[] buffer;
}


std::string fileHandler::getFileName(const std::string& filePath){
	// extract file name from file path
	std::filesystem::path path(filePath);
	std::string fileName = path.filename().string();
	return fileName;
}

std::string fileHandler::getOutputPath(const std::string& fileName, bool deleteOld){

// check OS
// windows
#ifdef _WIN32

	const char* homeDir = std::getenv("USERPROFILE");

	if(homeDir == nullptr){
		std::cerr << "Failed to get USERPROFILE environment variable." << std::endl;
        exit(1);
	}

	std::string targetFolder = std::string(homeDir) + "\\Downloads\\";
	std::string outputPath = targetFolder+fileName;

	// WINDOWS needs other way of handling directories
	// current solution throws exception -> access denied
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

	const char* homeDir = std::getenv("HOME");

	if(homeDir == nullptr){
		std::cerr << "Failed to get HOME environment variable." << std::endl;
		exit(1);
	}

	// create new folder to store file + key
	std::string targetFolder = std::string(homeDir) + "/Downloads/target/";
	std::string outputPath = targetFolder+fileName;

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

	for(int i = 0; i < 16; i++){
		std::cout<<std::hex<<(int)buffer[i]<<" | ";
	}

	keyFileStream.close();
	return buffer;
}

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



