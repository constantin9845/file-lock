#include "../include/fileHandler.h"

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

// Create key file
void fileHandler::storeKey(unsigned char* key, const int& keySize){

	std::string outputPath;

#ifdef _WIN32

	// get user name
	const char* homeDir = std::getenv("USERPROFILE");

	if(homeDir == nullptr){
		std::cerr << "Failed to get USERPROFILE environment variable." << std::endl;
        exit(1);
	}

	// construct target folder path
	std::string targetFolder = std::string(homeDir) + "\\Downloads\\";



// Mac/Linux
#else

	// get user name
	const char* homeDir = std::getenv("HOME");

	if(homeDir == nullptr){
		std::cerr << "Failed to get HOME environment variable." << std::endl;
		exit(1);
	}

	// create new folder to store file + key
	std::string targetFolder = std::string(homeDir) + "/Downloads/";

#endif

	outputPath = targetFolder+"_key0";

	// check if file already exists
	int counter = 1;
	while(std::filesystem::exists(outputPath)){
		outputPath = outputPath.substr(0,outputPath.length()-1);
		outputPath += std::to_string(counter++);
	}

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



// Given path of file, create relative path inside /Downloads/target folder
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

// create all directories for provided path
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


// update counter
void fileHandler::setCounterInNonce(unsigned char* nonce, uint32_t counter){
	nonce[12] = (counter >> 24) & 0xFF;
	nonce[13] = (counter >> 16) & 0xFF;
	nonce[14] = (counter >> 8) & 0xFF;
	nonce[15] = counter & 0xFF;
}

// Thread worker
void fileHandler::worker(unsigned char* buffer, int startBlock, int endBlock, unsigned char* key, int keySize, const unsigned char* baseNonce){

	for(int block = startBlock; block < endBlock; block++){
		int i = 12 + block * 16;

		unsigned char output[16];
		unsigned char nonce[16];

		std::memcpy(nonce, baseNonce, 16);
		setCounterInNonce(nonce, block); // block number = counter

		AES::encryptCTR(nonce, key, keySize, output);

		for(int j = 0; j < 16; j++){
			buffer[i + j] ^= output[j];
		}
	}
}

// GCM encryption
void fileHandler::AES_GCM(const std::string& path, unsigned char* key, const bool& replaceFlag, const int& keySize, const std::string& outputFilePath){

	// Read file 

	// input file stream
	std::ifstream inputFile(path, std::ios::binary);

	std::string outputPath;

	// Construct output path
	if(replaceFlag){
		outputPath = path;
	}
	else{
		outputPath = outputFilePath;
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


	// Generate nonce || counter = 0
	unsigned char* nonce = genKey(128);

	// Apply counter = 0 to nonce
	for(int i = 12; i<16; i++){
		nonce[i] = 0x00;
	}


	// Buffer to store data + Apply Padding, PKC#7
	unsigned char* buffer = new unsigned char[size + padding + 12];

	// Store nonce in buffer
	for(int i = 0; i < 12; i++){
		buffer[i] = nonce[i];
	}

	// read data into buffer
	if(!inputFile.read(reinterpret_cast<char*>(buffer+12),size)){
		std::cout<<"Could not read data into buffer";
		delete[] buffer;
		exit(2);
	}

	// Read padding bytes into buffer
	for(int i = size+12; i < size+padding+12; i++){
		buffer[i] = static_cast<unsigned char>(padding);
	}

	// 


	// Parallell Encryption
	unsigned int number_of_threads = std::thread::hardware_concurrency(); 
	int totalBlocks = (size + padding) / 16;

	if(number_of_threads == 0) number_of_threads = 4;
	number_of_threads = std::min<unsigned int>(number_of_threads, totalBlocks);

	int blocksPerThread = totalBlocks / number_of_threads;
	int remainder = totalBlocks % number_of_threads;

	std::vector<std::thread> threads;
	int currentBlock = 0;

	for(unsigned int t = 0; t < number_of_threads; t++){
		int blocksForThread = blocksPerThread + (t < remainder ? 1 : 0);
		int start = currentBlock;
		int end = currentBlock + blocksForThread;

		threads.emplace_back(worker, buffer,start,end,key,keySize,nonce);
		currentBlock = end;
	}

	for(auto& th : threads){
		th.join();
	}



	// delete old file
	if(replaceFlag){
		inputFile.close();
		std::filesystem::remove(path);
	}
	else{
		inputFile.close();
	}

	// output file stream
	std::ofstream outputFile(outputPath, std::ios::binary);

	if(!outputFile.write(reinterpret_cast<char*>(buffer), size+padding+12)){
		delete[] buffer;
		delete[] nonce;
		std::cout<<"error write";
		exit(3);
	}

	outputFile.close();

	// Generate

	delete[] nonce;
	delete[] buffer;
}

// GCM decryption
void fileHandler::AES_GCM_DECRYPTION(const std::string& path, unsigned char* key, const int& keySize){

	// Read file 

	// input file stream
	std::ifstream inputFile(path, std::ios::binary);

	std::string outputPath = path;

	if(!inputFile){
		std::cout<<"Error opening file.";
		exit(2);
	}


	// get size of file
	inputFile.seekg(0, std::ios::end);
	std::streamsize size = inputFile.tellg();
	inputFile.seekg(0, std::ios::beg);


	// Get nonce
	unsigned char* nonce;
	nonce = new unsigned char[16]{0};

	// Buffer to store data 
	unsigned char* buffer = new unsigned char[size];

	// read data into buffer
	if(!inputFile.read(reinterpret_cast<char*>(buffer),size)){
		std::cout<<"Could not read data into buffer";
		delete[] buffer;
		exit(2);
	}

	// load nonce
	for(int i = 0; i < 12; i++){
		nonce[i] = buffer[i];
	}


	// Parallell Encryption
	unsigned int number_of_threads = std::thread::hardware_concurrency(); 
	int totalBlocks = size / 16;

	if(number_of_threads == 0) number_of_threads = 4;
	number_of_threads = std::min<unsigned int>(number_of_threads, totalBlocks);

	int blocksPerThread = totalBlocks / number_of_threads;
	int remainder = totalBlocks % number_of_threads;

	std::vector<std::thread> threads;
	int currentBlock = 0;

	for(unsigned int t = 0; t < number_of_threads; t++){
		int blocksForThread = blocksPerThread + (t < remainder ? 1 : 0);
		int start = currentBlock;
		int end = currentBlock + blocksForThread;

		threads.emplace_back(worker, buffer,start,end,key,keySize,nonce);
		currentBlock = end;
	}

	for(auto& th : threads){
		th.join();
	}

	// Get padding
	std::streamsize padding = buffer[size-1];


	inputFile.close();
	std::filesystem::remove(path);

	// output file stream
	std::ofstream outputFile(outputPath, std::ios::binary);

	if(!outputFile.write(reinterpret_cast<char*>(buffer+12), size-padding)){
		delete[] buffer;
		delete[] nonce;
		std::cout<<"error write";
		exit(3);
	}

	outputFile.close();

	delete[] nonce;
	delete[] buffer;
}


