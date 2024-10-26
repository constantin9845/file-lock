#include "AES.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdlib>
#include <string>
#include <filesystem>
#include <system_error>

#ifdef _WIN32
	#include <random>
	#include <windows.h>
#endif



class fileHandler{

public:
	//static void encryptDirectory();
	//static void decryptDirectory();

	static void encryptFile(const std::string& path);
	static void encryptFile(const std::string& path, const std::string& keyPath);
	
	static void decryptFile(const std::string& path, const std::string& keyPath);

	static unsigned char* genKey();

	static void storeKey(unsigned char* key);

	static unsigned char* readKey(const std::string& path);

private:
	static std::string getFileName(const std::string& filePath);

	static std::string getOutputPath(const std::string& fileName, bool deleteOld);

};