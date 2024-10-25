# file-lock
Encrypt individual files using the AES block cipher

### To Do
- WINDOWS fix creating output directory in getOutputPath()
- add option/logic to use own key
- implement directory encryption
- refine logic / split up encryption process




### user interaction
-> bash script:
- ./run <path> -d/f(dir or file) -enc/dec -r(replace original, w/out = duplicate)


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
