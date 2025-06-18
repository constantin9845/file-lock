g++ src/main.cpp src/fileHandler.cpp src/AES.cpp -std=c++17 -maes -mpclmul "-msse4.1" -O2  -o enc
#g++ -g -std=c++17 -maes -mpclmul -msse4.1 -02 src/main.cpp src/fileHandler.cpp src/AES.cpp -o enc
#lldb ./enc
#gdb ./enc