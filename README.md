# file-lock
Encrypt individual files using the AES block cipher


### To Do

- test for -r flag bugs
- update README guide
- Add disclaimer file
- Release 1.0.0

  
- accept file with keys for directory enc/dec


### Program Description

The program encrypts/decrypts a single file or every file for a given directory
- Hidden files are skipped!

Encryption can be done with a user key or a key will be generated
- Key should be stored in a file -> the first 16 bytes are read

A key generated -> stored together with the encrypted file(s)

### Notes

File size increases by 16 bytes after encryption, This is the IV being added to the file

### Verified File types

- PDF PNG DOCX MP4

- To fix:
- TXT file bug -> line of new line characters appended to initial text (probably padding issue) 


## Caution
- When files are decrypted -> output files replace input files!
- No recovery without key that was used for encryption -> Do not modify key file
- Preferably DO NOT OPEN key file -> set read only mode

