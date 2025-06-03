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

#ifndef FH_H
#define FH_H

#ifdef _WIN32
	#include <random>
	#include <windows.h>
#endif



class fileHandler{

public:

	/*
	    Encrypt single file 
	*/
	static void encryptFile(const std::string& path, const std::string& keyPath, const bool& replaceFlag, const bool& mode, const int& keySize);


	/*
	    Encrypts all files in specified directory (includes subdirectory files)
     	    Reconstructs file structure inside target folder
	    Output into Downloads/target/ with key file
	    @param path absolute filepath
     	    @param outputPath path where target folder is placed
	    @param dirFlag specifies directory 
      	    @param key 128 bit key
	*/
	static void encryptFile(const std::string& path, std::string outputPath, unsigned char* key, bool mode, int keySize);

	/*
	    Decrypt single file with user provided key
     	    key should be stored in a file -> first 16 bytes are read
	    Output into same location as input file
     	    -> INPUT FILE IS DELETED AFTER COMPLETION
	    @param path absolute filepath
     	    @param keyPath absolute path of key file
	*/
	static void decryptFile(const std::string& path, unsigned char* key, bool mode, int keySize);

	/*
	    GCM encryption
	*/
	static void AES_GCM(const std::string& path, unsigned char* key, const bool& replaceFlag, const int& keySize, const std::string& outputFilePath);


	/*
	    GCM decryption
	*/
	static void AES_GCM_DECRYPTION(const std::string& path, unsigned char* key, const int& keySize);



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

private:
	

};

#endif
