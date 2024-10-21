# file-lock
Encrypt individual files using the AES block cipher

### To Do
- Encryption/decryption with aes works (tested) -> refine logic + add file name/path/extension
- need a way to track padding -> attach to key? attach to file?
-- encrypted and decrypted files are exactly the same size
- test functionality till here


### user interaction
-> bash script:
- ./run <path> -d/f(dir or file) -enc/dec -algorithm -r(replace original, w/out = duplicate)

- if TRIVIUM DEC: promt <key> <IV>
- if TRIVIUM ENC: promt <key> <IV> or empty for new

- if AES DEC: promt <key>
- if AES ENC: promt <key> or empty for new

- AFTER ENC : to downloads/ :
	 txt file w/ key and IV / 
	 if -r -> encrypted file as well



### program process
-> select file
-> read in binary format 

 - number of bits must be a multiple of 128 for AES -> needs padding
 - keep track of size padding -> for decryption

 - Trivium -> no specific size but -> converts text to boolean vector 
 - need to add function to trivium class to take raw bytes as input

-> encrypt 
-> return as (corrupted file) 


- options

-> encrypt file / all files in given dir
-> generate keys IVs
-> decrypt above