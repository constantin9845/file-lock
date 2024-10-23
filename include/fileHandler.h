#include "AES.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdlib>
#include <string>

#ifdef _WIN32
	#include <windows.h>
	#include <wincrypt.h>
#endif


class fileHandler{

public:
	//static void encryptDirectory();
	//static void decryptDirectory();

	static void encryptFile(const std::string& path, const bool& algorithmFlag, const bool& rFlag);
	static void decryptFile(const std::string& path, const bool& algorithmFlag, const bool& rFlag);

	static unsigned char* genKey();

private:
	static std::string getFileName(const std::string& filePath);

	static std::string getOutputPath(const std::string& fileName);


};