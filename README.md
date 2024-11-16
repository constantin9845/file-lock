# file-lock
Encrypt individual files using the AES block cipher


### To Do

- Test Windows Building / linux works
- Add disclaimer file
- Release 1.0.0

  
- directory enc can have new key for each file or use single key
- accept file with keys for directory enc/dec


### Program Description

The program encrypts/decrypts a single file or every file for a given directory
- Hidden files are skipped!

Encryption can be done with a user key or a key will be generated
- Key should be stored in a file -> the first 16 bytes are read

A key generated -> stored together with the encrypted file(s)

### Verified File types

- PDF PNG DOCX MP4

- To fix:
- TXT file bug -> line of new line characters appended to initial text (probably padding issue) 


### Build Guide

#### 1. Create Build Directory

- Navigate to project folder and create build directory
```bash
mkdir build
cd build
```

#### 2. Generate Build Files
Linux/mac
```bash
cmake ..
```

WIN
```bash
cmake .. -G "MinGW Makefiles"
```

#### 3. Build Project
Linux/mac
```bash
cmake --build .
```

WIN
```bash
cmake --build . --config Release
```

#### 4. Installation

##### With Root Privileges
Linux/mac
```bash
sudo cmake --install .
```

WIN - Use Administrator Command Prompt
```bash
# Navigate to build directory
cmake --install .
```


##### Without Root privileges
Run program from Build folder


### Examples


```bash


# Single file Encryption
./enc "<Path to file>" -enc "<path to key file>"(optional)

# Encrypt entire directory

## UNIX -> *
./enc "/path/*" -enc "<path to key file>"(optional)


## WIN -> /empty
./enc "/path/" -enc "<path to key file>"(optional)


# Decrypt File
./enc "<Path to file>" -dec "<path to key file>"(required)


# Decrypt directory

## UNIX -> *
./enc "/path/*" -dec "<path to key file>"(required)


## WIN -> /empty
./enc "/path/" -dec "<path to key file>"(reduired)

```

## Caution
- When files are decrypted -> output files replace input files!
- No recovery without key that was used for encryption -> to not modify key file
- Preferably DO NOT OPEN key file -> set read only mode

