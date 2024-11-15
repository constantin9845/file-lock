#include "AES.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdlib>
#include <string>
#include <filesystem>
#include <system_error>
#include <algorithm>

#ifndef FH_H
#define FH_H

#ifdef _WIN32
	#include <random>
	#include <windows.h>
#endif



class fileHandler{

public:
	//static void encryptDirectory();
	//static void decryptDirectory();

	/*
	    Encrypt single file + generate key
	    Output into Downloads/target/ with encrypted file + key file
     	    @param path absolute filepath
	*/
	static void encryptFile(const std::string& path);

	/*
	    Encrypts all files in specified directory (includes subdirectory files)
     	    Reconstructs file structure inside target folder
	    Output into Downloads/target/ with key file
	    @param path absolute filepath
     	    @param outputPath path where target folder is placed
	    @param dirFlag specifies directory 
      	    @param key 128 bit key
	*/
	static void encryptFile(const std::string& path, const std::string& outputPath ,bool dirFlag, unsigned char* key);

	/*
	    Encrypt single file with user provided key
     	    key should be stored in a file -> first 16 bytes are read
	    Output into Downloads/target/ 
	    @param path absolute filepath
     	    @param keyPath absolute path of key file
	*/
	static void encryptFile(const std::string& path, const std::string& keyPath);

	/*
	    Decrypt single file with user provided key
     	    key should be stored in a file -> first 16 bytes are read
	    Output into same location as input file
     	    -> INPUT FILE IS DELETED AFTER COMPLETION
	    @param path absolute filepath
     	    @param keyPath absolute path of key file
	*/
	static void decryptFile(const std::string& path, const std::string& keyPath);

	/*
	    Decrypts all files in specified directory (includes subdirectory files)
     	    DECRYPTED FILES ARE DELETED AFTER COMPLETION
     	    key should be stored in a file -> first 16 bytes are read
	    Output into same location as input files
	    @param path absolute filepath
     	    @param dirFlag specifies directory 
     	    @param keyPath absolute path of key file
	*/
	static void decryptFile(const std::string& path, bool dirFlag, const std::string& keyPath);

	// Generate 128 bit key
	// unix systems -> /dev/urandom , WIN -> random module
	static unsigned char* genKey();

	// Write given key to a file
	// @param key key stored in byte array
	static void storeKey(unsigned char* key);

	// reads key from given file
	// @param path absolute filepath
	static unsigned char* readKey(const std::string& path);

	// returns name from absolute path of file
	// @param path absolute filepath
	static std::string getFileName(const std::string& filePath);

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

private:
	

};

#endif
