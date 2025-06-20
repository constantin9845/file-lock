
RM = rm -f
CLEANO = del src/*.o
CXXFLAGS := -std=c++17
TARGET := enc

ifeq ($(OS), Windows_NT)
	RM = del /q
	CLEANO = del .\\src\\*.o
	CXXFLAGS := -std=c++17 -maes -mpclmul "-msse4.1" -O2
	TARGET := enc.exe
endif

CXX := g++

SRCS := src/main.cpp src/fileHandler.cpp src/AES.cpp
OBJS := $(SRCS:.cpp=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^


clean:
	$(CLEANO)
	$(RM) $(TARGET)
