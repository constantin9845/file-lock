# file-lock
Encrypt individual files using the AES block cipher


### To Do

- Implement : user decides target forled or straight replacement (add warning)
- Implement target directory choice for encryption (replace or downloads folder)

- directory enc can have new key for each file or use single key
- accept file with keys for directory enc/dec


### Program Description

The program encrypts/decrypts a single file or every file for a given directory
- Hidden files are skipped!

Encryption can be done with a user key or a key will be generated
- Key should be stored in a file -> the first 16 bytes are read

A key generated -> stored together with the encrypted file(s)


#### Input exmaple

```bash
# Compile
./run.sh

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

