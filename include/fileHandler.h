#include "trivium.h"
#include "AES.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdlib>


class fileHandler{

public:
	//static void encryptDirectory();
	//static void decryptDirectory();

	static void encryptFile(const std::string& path, const bool& algorithmFlag, const bool& rFlag);
	static void decryptFile(const std::string& path, const bool& algorithmFlag, const bool& rFlag);

	//static std::string generateKey();

private:
	static std::string getFileName(const std::string& filePath);

	static std::string getOutputPath(const std::string& fileName);


};