# file-lock
Encrypt individual files using the AES block cipher

### To Do

- Directory decryption implemented:
-- Verify directory decryption on WIN -> output path function probably needs edit
-- DOCUMENTATION
-- Implement target choice for encryption (replace or downloads folder)

- directory enc can have new key for each file or use single key
- accept file with keys for directory enc/dec

### Path parsing for directory enc/dec

- take initial path passed into program
- compare to current file
- remove common part


### Directory enc/dec process

- iterate files -> pass to enc/dec function
- sub dirs? path module can iterate -> need to recreate file structure in original/target folder
-- subdirectories relative to input directory need to be recreated before file is saved


### user interaction
-> bash script:
- ./run <path> -enc/dec -<key>(optional)


- AFTER ENC : to downloads/target/ :
	 file w/ key and IV / 
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
