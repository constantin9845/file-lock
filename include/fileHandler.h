#include "trivium.h"
#include "AES.h"
#include <iostream>
#include <fstream>


class fileHandler{

public:
	//static void encryptDirectory();
	//static void decryptDirectory();

	static void encryptFile(const std::string& path, const bool& algorithmFlag, const bool& rFlag);
	//static void decryptFile();

	//static std::string generateKey();

private:


};