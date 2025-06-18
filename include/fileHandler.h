#include "AES.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdlib>
#include <string>
#include <filesystem>
#include <system_error>
#include <algorithm>
#include <thread>
#include <cstring>
#include <fcntl.h>

#ifndef FH_H
#define FH_H

#ifdef _WIN32
	#include <random>
	#include <windows.h>
#else
	#include <unistd.h>
	#include <errno.h>
	#include <sys/types.h>
	#include <sys/stat.h>
#endif



class fileHandler{

public:

	/*
	    GCM encryption
	*/
	static void AES_GCM(const std::string& path, unsigned char* key, const bool& replaceFlag, const int& keySize, const std::string& outputFilePath, bool authTag, std::string AD);

	static void HW_AES_GCM(const std::string& path, unsigned char* key, const bool& replaceFlag, const int& keySize, const std::string& outputFilePath, bool authTag, std::string AD);

	/*
	    GCM decryption
	*/
	static void AES_GCM_DECRYPTION(const std::string& path, unsigned char* key, const int& keySize, bool authTag, bool AD);

	static void HW_AES_GCM_DECRYPTION(const std::string& path, unsigned char* key, const int& keySize, bool authTag, bool AD);



	// Generate 128 bit key
	// unix systems -> /dev/urandom , WIN -> random module
	static unsigned char* genKey(const int& keySize);

	// Write given key to a file
	// @param key key stored in byte array
	static void storeKey(unsigned char* key, const int& keySize);

	// reads key from given file
	// @param path absolute filepath
	static unsigned char* readKey(const std::string& path, const int& keySize);

	// returns name from absolute path of file
	// @param path absolute filepath
	static std::string getFileName(const std::string& filePath);

	// returns string for absolute path of file
	static std::string getDecryptionFileName(const std::string& filePath);

	// Constructs output path for Downloads/target/
	// Used for single file encryption
	// @param fileName name of file
	// @param deleteOld flag to delete existing Downloads/target/ file(s)
	static std::string getOutputPath(const std::string& fileName, bool deleteOld);

	// Creates target directory in Downloads 
	static bool createRootDir();

	// Given absolute path and absolute path of relative root folder of the target encryption directory
	// return the file's relative path to this root folder
	// @param filePath absolute path of file
	// @param path absolute path of relative root folder 
	static std::string parsePath(const std::string& filePath, const std::string& path);

	// constructs path for subdirectories in directory encryption
	// needed for reconstructing file structure
	// @param filePath absolute path of file
	static void constructPath(const std::string& filePath);

	static void setCounterInNonce(unsigned char* nonce, uint32_t counter);

	static void worker(unsigned char* buffer, int startBlock, int endBlock, unsigned char* key, int keySize, const unsigned char* baseNonce);

	static void HW_worker(unsigned char* buffer, int startBlock, int endBlock, unsigned char* key, int keySize, unsigned char* baseNonce);

private:
	

};

#endif
