
CXX := g++

SRCS := src/main.cpp src/fileHandler.cpp src/AES.cpp
OBJS := $(SRCS:.cpp=.o)

TARGET := enc
CXXFLAGS := -std=c++17 -maes -mpclmul -msse4.1 -O2
LDFLAGS := 

RM := rm -f
CLEANO := rm src/*.o

ifeq ($(OS), Windows_NT)
    TARGET := enc.exe
    LDFLAGS := -lbcrypt
    RM := del /q
    CLEANO := del /q .\\src\\*.o
endif

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(TARGET)
	$(CLEANO)
