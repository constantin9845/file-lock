# Define the executable name and source files
EXEC = run
SRC = src/fileHandler.cpp src/AES.cpp src/trivium.cpp
INCLUDE = -Iinclude

# Define the installation directory (e.g., /usr/local/bin)
INSTALL_DIR = /usr/local/bin

# Default target: compile the program
all: $(EXEC)

# Compile the C++ program
$(EXEC): $(SRC)
    g++ $(INCLUDE) -o $(EXEC) $(SRC)

# Install the executable to /usr/local/bin
install: $(EXEC)
    chmod +x $(EXEC)
    sudo mv $(EXEC) $(INSTALL_DIR)

# Clean up the build (optional)
clean:
    rm -f $(EXEC)
