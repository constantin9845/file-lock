

## CTR mode steps:

1) Input: file, Key, keySize

2) Generate nonce

3) expand Key

4) Padding

4) Perform parallel Encryption


## GMAC steps:

1) input: Additional info, size(Additional info), key, keySize, plaintextSize

- Encrypt 0^128 with key --> h
- h XOR AD_i (AD : additional data) --> j
- j XOR ciphertext_i --> k
- final GMAC steps with j --? AUTH TAG
--
- 16 byte AUTH TAG attached to cipherfile

### GHASH function

- 


## To DO


- IMplement multiplication for GHASH

- Counter should start at 1, not 0
- Slow performance, implement AES-NI, key expansion should happen once only.
- Can write to file and GMAC be multithreaded?

- Implement auth
- graph timing results
- add docs.


-- check for very large input size.

