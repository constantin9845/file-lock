

## CTR mode steps:

1) Input: file, Key, keySize

2) Generate nonce

3) expand Key

4) Padding

4) Perform parallel Encryption


## Decryption: Auth tag handling 

1 file: 

- Ask path
- Create new Tag with encrypted buffer
- compare tags


Directory:

- Path : filepath + "_TAG" --> must maintain naming format
- Create new Tag with encrypted buffer
- compare tags



---> Comparing:

- TAG supplied encryption folder
- Compute TAG : text supplied in _Additional_Message file + cipher + nonce


## To DO


- When TAG with no AD --> TAG comes out ad empty file. (works fine if AD is added)

- Add input fix for AD input: newline must be eaten before std::getline().

- Authentication very slow -> only with hw accel.
- Other, faster method for pure software implementation?

- Can write to file and GMAC be multithreaded?

- add docs.


-- check for very large input size.
-- duplicate AD txt files in dir encryption
-- add option to auto detect AD and tagL: for dir decryption : some yes some no.




