

## CTR mode steps:

1) Input: file, Key, keySize

2) Generate nonce

3) expand Key

4) Padding

4) Perform parallel Encryption


## To DO

- Add tag authentication for decryption,
- Authentication very slow -> only with hw accel.
- Other, faster method for pure software implementation?

- Slow performance, implement AES-NI, key expansion should happen once only.
- Can write to file and GMAC be multithreaded?

- add docs.


-- check for very large input size.

