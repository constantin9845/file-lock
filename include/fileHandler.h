#include "AES.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdlib>
#include <string>
#include <filesystem>

#ifdef _WIN32
	#include <random>
#endif



class fileHandler{

public:
	//static void encryptDirectory();
	//static void decryptDirectory();

	static void encryptFile(const std::string& path);
	static void decryptFile(const std::string& path);

	static unsigned char* genKey();

private:
	static std::string getFileName(const std::string& filePath);

	static std::string getOutputPath(const std::string& fileName, bool deleteOld);

	static void storeKey(unsigned char* key);

};