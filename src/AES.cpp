#include "../include/AES.h"

// byte substitution layer
void AES::byteSub(unsigned char state[4][4]){
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			state[i][j] = SBOX[(state[i][j])/16][(state[i][j])%16];
		}
	}
}

unsigned char AES::byteSub(unsigned char A){
	return SBOX[A/16][A%16];
}

// inverse byte substitution layer
unsigned char AES::inverseByteSub(unsigned char A){
	return INV_SBOX[A/16][A%16];
}

void AES::inverseByteSub(unsigned char state[4][4]){
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			state[i][j] = INV_SBOX[(state[i][j])/16][(state[i][j])%16];
		}
	}
}

// shift row layer
void AES::shiftRow(unsigned char state[4][4]){

	// row 2 shifted left by 1
	unsigned char k = state[1][0];
	for(int j = 0; j < 3; j++){
		state[1][j] = state[1][j+1];
	}
	state[1][3] = k;


	// row 3 shifted left by 2
	unsigned char a = state[2][0];
	unsigned char b = state[2][1];
	for(int i = 0; i < 2; i++){
		state[2][i] = state[2][i+2];
	}
	state[2][2] = a; state[2][3] = b;


	// row 4 shifted right by 1
	unsigned char c = state[3][3];
	for(int i = 3; i > 0; i--){
		state[3][i] = state[3][i-1];
	}
	state[3][0] = c;
}

// shift row layer
void AES::inverseShiftRow(unsigned char state[4][4]){

	// row 2 shifted right by 1
	unsigned char k = state[1][3];
	for(int j = 3; j > 0; j--){
		state[1][j] = state[1][j-1];
	}
	state[1][0] = k;

	// row 3 shifted right by 2
	unsigned char a = state[2][2];
	unsigned char b = state[2][3];

	for(int j = 3; j > 1; j--){
		state[2][j] = state[2][j-2];
	}
	state[2][0] = a;
	state[2][1] = b;


	// row 4 shifted left by 1
	k = state[3][0];
	for(int j = 0; j < 3; j++){
		state[3][j] = state[3][j+1];
	}
	state[3][3] = k;

}

// Mix column layer
void AES::mixCol(unsigned char state[4][4]){

	for(int i = 0; i < 4; i++){

		unsigned char c0 = 	GFmultiply(state[0][i],MIXCOL_MATRIX[0][0]) ^ 
							GFmultiply(state[1][i],MIXCOL_MATRIX[0][1]) ^ 
							GFmultiply(state[2][i],MIXCOL_MATRIX[0][2]) ^ 
							GFmultiply(state[3][i],MIXCOL_MATRIX[0][3]);

		unsigned char c1 = 	GFmultiply(state[0][i],MIXCOL_MATRIX[1][0]) ^ 
							GFmultiply(state[1][i],MIXCOL_MATRIX[1][1]) ^ 
							GFmultiply(state[2][i],MIXCOL_MATRIX[1][2]) ^ 
							GFmultiply(state[3][i],MIXCOL_MATRIX[1][3]);

		unsigned char c2 = 	GFmultiply(state[0][i],MIXCOL_MATRIX[2][0]) ^ 
							GFmultiply(state[1][i],MIXCOL_MATRIX[2][1]) ^ 
							GFmultiply(state[2][i],MIXCOL_MATRIX[2][2]) ^ 
							GFmultiply(state[3][i],MIXCOL_MATRIX[2][3]);

		unsigned char c3 = 	GFmultiply(state[0][i],MIXCOL_MATRIX[3][0]) ^ 
							GFmultiply(state[1][i],MIXCOL_MATRIX[3][1]) ^ 
							GFmultiply(state[2][i],MIXCOL_MATRIX[3][2]) ^ 
							GFmultiply(state[3][i],MIXCOL_MATRIX[3][3]);


		state[0][i] = c0; state[1][i] = c1; state[2][i] = c2; state[3][i] = c3;
	}

}

// Inverse Mix column layer
void AES::inverseMixCol(unsigned char state[4][4]){
	for(int i = 0; i < 4; i++){

		unsigned char c0 = 	GFmultiply(state[0][i],INVERSE_MIXCOL_MATRIX[0][0]) ^ 
							GFmultiply(state[1][i],INVERSE_MIXCOL_MATRIX[0][1]) ^ 
							GFmultiply(state[2][i],INVERSE_MIXCOL_MATRIX[0][2]) ^ 
							GFmultiply(state[3][i],INVERSE_MIXCOL_MATRIX[0][3]);

		unsigned char c1 = 	GFmultiply(state[0][i],INVERSE_MIXCOL_MATRIX[1][0]) ^ 
							GFmultiply(state[1][i],INVERSE_MIXCOL_MATRIX[1][1]) ^ 
							GFmultiply(state[2][i],INVERSE_MIXCOL_MATRIX[1][2]) ^ 
							GFmultiply(state[3][i],INVERSE_MIXCOL_MATRIX[1][3]);

		unsigned char c2 = 	GFmultiply(state[0][i],INVERSE_MIXCOL_MATRIX[2][0]) ^ 
							GFmultiply(state[1][i],INVERSE_MIXCOL_MATRIX[2][1]) ^ 
							GFmultiply(state[2][i],INVERSE_MIXCOL_MATRIX[2][2]) ^ 
							GFmultiply(state[3][i],INVERSE_MIXCOL_MATRIX[2][3]);

		unsigned char c3 = 	GFmultiply(state[0][i],INVERSE_MIXCOL_MATRIX[3][0]) ^ 
							GFmultiply(state[1][i],INVERSE_MIXCOL_MATRIX[3][1]) ^ 
							GFmultiply(state[2][i],INVERSE_MIXCOL_MATRIX[3][2]) ^ 
							GFmultiply(state[3][i],INVERSE_MIXCOL_MATRIX[3][3]);


		state[0][i] = c0; state[1][i] = c1; state[2][i] = c2; state[3][i] = c3;
	}
}

// Key addition layer
void AES::applyKey(unsigned char state[4][4], unsigned int* k, int& keyIndex){
	// key element -> 4 bytes
	// data elem -> 1 byte

	// 16 bytes of input

	for(int byte = 0; byte < 4; byte++){

		// 1. grab 4 bytes and reformat into 32 bit variable
		// 3. apply key
		// 4. break down again into 4 bytes

		unsigned int temp = (state[0][byte] << 24) ^ (state[1][byte] << 16) ^ (state[2][byte] << 8) ^ (state[3][byte]);
		temp ^= k[keyIndex++];

		state[0][byte] = (temp >> 24) & 0xFF;
		state[1][byte] = (temp >> 16) & 0xFF;
		state[2][byte] = (temp >> 8) & 0xFF;
		state[3][byte] = temp & 0xFF;

	}
}

// Inverse Key addition layer
void AES::inverseApplyKey(unsigned char state[4][4], unsigned int* k, int keyIndex){

	// key element -> 4 bytes
	// data elem -> 1 byte

	int index = keyIndex;

	for(int byte = 0; byte < 4; byte++){

		// 1. grab 4 bytes and reformat into 32 bit variable
		// 3. apply key
		// 4. break down again into 4 bytes

		unsigned int temp = (state[0][byte] << 24) ^ (state[1][byte] << 16) ^ (state[2][byte] << 8) ^ (state[3][byte]);
		temp ^= k[index++];

		state[0][byte] = (temp >> 24) & 0xFF;
		state[1][byte] = (temp >> 16) & 0xFF;
		state[2][byte] = (temp >> 8) & 0xFF;
		state[3][byte] = temp & 0xFF;
	}
}


//128 Key scheduler
unsigned int* AES::genKey128(unsigned char* K){

	// will store 4 bytes in each element
	unsigned int* W = new unsigned int[44];

	
	// load first 4 bytes into W
	W[0] = (K[0] << 24) ^ (K[1] << 16) ^ (K[2] << 8) ^ (K[3]);
	W[1] = (K[4] << 24) ^ (K[5] << 16) ^ (K[6] << 8) ^ (K[7]);
	W[2] = (K[8] << 24) ^ (K[9] << 16) ^ (K[10] << 8) ^ (K[11]);
	W[3] = (K[12] << 24) ^ (K[13] << 16) ^ (K[14] << 8) ^ (K[15]);

	unsigned int gConst = 1; // round constant

	// key expansion
	for(int i = 4; i < 44; i++){
		if(i % 4 == 0){
			W[i] = W[i-4] ^ g(W[i-1], gConst);
		}
		else{
			W[i] = W[i-1] ^ W[i-4]; 
		}
	}
	
	return W;
}

// 192 bit key scheduler
unsigned int* AES::genKey192(unsigned char* K){

	// expanded to 52 words
	unsigned int* W = new unsigned int[52];

	// load first 6 bytes into W
	W[0] = (K[0] << 24) ^ (K[1] << 16) ^ (K[2] << 8) ^ (K[3]);
	W[1] = (K[4] << 24) ^ (K[5] << 16) ^ (K[6] << 8) ^ (K[7]);
	W[2] = (K[8] << 24) ^ (K[9] << 16) ^ (K[10] << 8) ^ (K[11]);
	W[3] = (K[12] << 24) ^ (K[13] << 16) ^ (K[14] << 8) ^ (K[15]);
	W[4] = (K[16] << 24) ^ (K[17] << 16) ^ (K[18] << 8) ^ (K[19]);
	W[5] = (K[20] << 24) ^ (K[21] << 16) ^ (K[22] << 8) ^ (K[23]);

	unsigned int gConst = 1; // round constant

	// key expansion
	for(int i = 6; i < 52; i++){
		if(i % 6 == 0){
			W[i] = W[i-6] ^ g(W[i-1], gConst);
		}
		else{
			W[i] = W[i-1] ^ W[i-6]; 
		}
	}

	return W;
}

// 256 bit key scheduler
unsigned int* AES::genKey256(unsigned char* K){
	// expanded to 60 words
	unsigned int* W = new unsigned int[60];

	// load first 8 bytes into W
	W[0] = (K[0] << 24) ^ (K[1] << 16) ^ (K[2] << 8) ^ (K[3]);
	W[1] = (K[4] << 24) ^ (K[5] << 16) ^ (K[6] << 8) ^ (K[7]);
	W[2] = (K[8] << 24) ^ (K[9] << 16) ^ (K[10] << 8) ^ (K[11]);
	W[3] = (K[12] << 24) ^ (K[13] << 16) ^ (K[14] << 8) ^ (K[15]);
	W[4] = (K[16] << 24) ^ (K[17] << 16) ^ (K[18] << 8) ^ (K[19]);
	W[5] = (K[20] << 24) ^ (K[21] << 16) ^ (K[22] << 8) ^ (K[23]);
	W[6] = (K[24] << 24) ^ (K[25] << 16) ^ (K[26] << 8) ^ (K[27]);
	W[7] = (K[28] << 24) ^ (K[29] << 16) ^ (K[30] << 8) ^ (K[31]);

	unsigned int gConst = 1; // round constant

	// key expansion
	for(int i = 8; i < 60; i++){
		if(i % 8 == 0){
			W[i] = W[i-8] ^ g(W[i-1], gConst);
		}
		else if(i == 4 || i == 12 || i == 20 || i == 28){
			W[i] = W[i-8] ^ h(W[i-1]);
		}
		else{
			W[i] = W[i-1] ^ W[i-8]; 
		}
	}

	return W;
}

// g function for key schedule
unsigned int AES::g(unsigned int w, unsigned int& gConst){

	// rotate left
	unsigned int rotatedW = (w << 8) | (w >> 24);


	// separate into 4 parts
	unsigned char a = (rotatedW >> 24) & 0xFF;
	unsigned char b = (rotatedW >> 16) & 0xFF;
	unsigned char c = (rotatedW >> 8) & 0xFF;
	unsigned char d = rotatedW & 0xFF;


	// 2. S-box substitution 
	// store in unsigned char to use byteSub function
	a = byteSub(a);
	b = byteSub(b);
	c = byteSub(c);
	d = byteSub(d);


	// Add round constant to left most byte:
	// const used , then shifted left by 1 --> if LMB set? --> mod p(x)
	a ^= gConst;

	// LMB set
	if(gConst & 0x80){
		gConst = (gConst << 1) ^ 0x11B;
	}
	else{
		gConst = gConst << 1;
	}


	// combine bytes back together into a word
	unsigned int res = (a << 24) ^ (b << 16) ^ (c << 8) ^ d;
	
	return res;
}

unsigned int AES::h(unsigned int w){

	// separate into 4 parts
	unsigned char a = (w >> 24) & 0xFF;
	unsigned char b = (w >> 16) & 0xFF;
	unsigned char c = (w >> 8) & 0xFF;
	unsigned char d = w & 0xFF;

	a = byteSub(a);
	b = byteSub(b);
	c = byteSub(c);
	d = byteSub(d);

	// combine bytes back together into a word
	unsigned int res = (a << 24) ^ (b << 16) ^ (c << 8) ^ d;
	
	return res;
}

// Galois field multiplication by 2
// helper function to multiply by 2
unsigned char AES::GFmultiply2(unsigned char b){
	if(b & 0x80){
		return (b<<1) ^ 0x1B;
	}
	else{
		return b<<1;
	}

}

// Galois field multiplication for Mix col funcion
unsigned char AES::GFmultiply(unsigned char b, unsigned char temp){

	// multiplying by 1 --> no change
	if(temp == 0x01){
		return b;
	}

	// multiply by 2
	// 2 = x --> left shift by 1
	else if(temp == 0x02){
		return GFmultiply2(b);
	}

	// multiply by 3
	// 3 = x+1 --> left shift by 1 and XOR(add) to initial value
	else if(temp == 0x03){
		// perform shift
		unsigned char shifted = GFmultiply2(b);

		// XOR(add) to initial value 
		return shifted ^ b;	
	}

	// multiply by 9 (INV)
	// 9 = x^3 + 1 ==> left shift 3 and XOR(add) initial value
	// perform reduction if bits 6,7 or 8 are set
	else if(temp == 0x09){
		//perform shift
		unsigned char shifted = b;
		
		shifted = GFmultiply2(shifted);
		shifted = GFmultiply2(shifted);
		shifted = GFmultiply2(shifted);

		return shifted ^ b;
	}

	// multiply by b(11) (INV)
	// b = x^3 + x + 1 ==> left shift 3 XOR left shift 1 XOR initial value
	// perform reduction if bits 6,7 or 8 are set
	else if(temp == 0x0B){
		//perform shifts
		unsigned char shifted = b;
		unsigned char shifted2 = b;
		
		shifted = GFmultiply2(shifted);
		shifted = GFmultiply2(shifted);
		shifted = GFmultiply2(shifted);

		shifted2 = GFmultiply2(shifted2);

		return shifted ^ shifted2 ^ b;
	}

	// multiply by d(13) (INV)
	// d = x^3 + x^2 + 1 ==> left shift 3 XOR left shift 2 XOR initial value
	else if(temp == 0x0D){
		//perform shifts
		unsigned char shifted = b;
		unsigned char shifted2 = b;
		
		shifted = GFmultiply2(shifted);
		shifted = GFmultiply2(shifted);
		shifted = GFmultiply2(shifted);

		shifted2 = GFmultiply2(shifted2);
		shifted2 = GFmultiply2(shifted2);

		// XOR(add) to initial value
		return shifted ^ shifted2 ^ b;
	}

	// multiply by e(14) (INV)
	// e = x^3 + x^2 + x ==> left shift 3 XOR left shift 2 XOR left shift 1
	// perform reduction if bits 6,7 or 8 are set
	else if(temp == 0x0E){
		//perform shifts
		unsigned char shifted = b;
		unsigned char shifted2 = b;
		unsigned char shifted3 = b;
		
		shifted = GFmultiply2(shifted);
		shifted = GFmultiply2(shifted);
		shifted = GFmultiply2(shifted);

		shifted2 = GFmultiply2(shifted2);
		shifted2 = GFmultiply2(shifted2);

		shifted3 = GFmultiply2(shifted3);

		return shifted ^ shifted2 ^ shifted3;
	}

	else{
		return 0x00;
	}
}


// Pad AD and get number of blocks
unsigned char* AES::pad_AD(unsigned char* AD, int AD_size, int& padded_size){

	padded_size = ((AD_size/16)+1)*16;
	unsigned char* newAD = new unsigned char[padded_size]{0};

	memcpy(newAD, AD, AD_size);
	return newAD;
}


// GHASH multiplication
void AES::GALOIS_MULTIPLICATION(unsigned char* result, const unsigned char* HASH_SUBKEY){
	
	unsigned char output[16] = {0};
	unsigned char Y[16];
	memcpy(Y, HASH_SUBKEY, 16);

	for(int byte = 0; byte < 16; byte++){

		for(int bit = 7; bit >= 0; bit--){

			// Is current bit set
			if((result[byte]>>bit) & 1){
				// Y XOR output
				for(int i = 0; i < 16; i++){
					output[i] ^= Y[i];
				}
			}

			// save MSB of Y
			bool msb = Y[0] & 0x80;

			// Shift Y << 1
			for(int i = 0; i < 15; i++){
				Y[i] = (Y[i] << 1) | (Y[i+1] >> 1);
			}
			Y[15] <<= 1;

			// Reduce is msb was set
			if(msb){
				Y[15] ^= 0x87;
			}
		}
	}

	memcpy(result, output, 16);
}

// GHASH function
void AES::GHASH(unsigned char* prev_g, unsigned char* input, int input_index, const unsigned char* HASH_SUBKEY){

	for(int i = 0; i < 16; i++){
		prev_g[i] = prev_g[i]^input[input_index+i];
	}

	GALOIS_MULTIPLICATION(prev_g, HASH_SUBKEY);
}

// Calculate Authentication Tag
void AES::auth_tag(unsigned char* nonce, unsigned char* key, const int& keySize, unsigned char* AD, int AD_size, unsigned char* Y, const int Y_size, unsigned char* TAG){

	unsigned char HASH_SUBKEY[16]{0};
	unsigned char g[16]{0};

	encryptCTR(g, key, keySize, HASH_SUBKEY);

	int original_AD_size = AD_size;
	int padded_size = 0;
	unsigned char* temp = nullptr;

	// Pad AD
	if(AD_size > 0){
		temp = pad_AD(AD, AD_size, padded_size);
		delete[] AD;

		// Process AD
		for(int i = 0; i < padded_size; i+=16){
			GHASH(g, temp, i, HASH_SUBKEY);
		}

		delete[] temp;
	}



	// Process Y
	for(int i = 0; i < Y_size; i+=16){
		GHASH(g, Y, i, HASH_SUBKEY);
	}

	// AD + text length addition
	unsigned char len_block[16]{0};
	int bit_size1 = original_AD_size*8;
	int bit_size2 = Y_size*8;
	
	for(int i = 0; i < 8; i++){
		len_block[7-i] = (bit_size1>>(i*8))&0xFF;
		len_block[15-i] = (bit_size2>>(i*8))&0xFF;
	}

	GHASH(g, len_block, 0, HASH_SUBKEY);

	// encrypt (Nonce||0^31||1);
	unsigned char J[16]{0};

	memcpy(J, nonce, 12);
	J[15] = 0x01;

	unsigned char tag_mask[16]{0};
	encryptCTR(J, key, keySize, tag_mask);

	// Store in TAG
	for(int i = 0; i < 16; i++){
		TAG[i] = g[i]^tag_mask[i];
	}
}

// CTR Mode
void AES::encryptCTR(unsigned char* nonce, unsigned char* key, const int& keySize, unsigned char output[]){

	unsigned char state[4][4];

	unsigned int* k = nullptr;

	// expand key
	switch(keySize){
		case 128:
			k = genKey128(key);
			break;
		case 192:
			k = genKey192(key);
			break;
		case 256:
			k = genKey256(key);
			break;
	}

	int keyIndex = 0;

	// load state
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			state[i][j] = nonce[i + 4 * j];
		}
	}

	// Key whitening
	applyKey(state, k, keyIndex);

	// Apply encryption layers
	// perform basic rounds
	for(int i = 0; i < (keySize == 128 ? 9 : (keySize == 192) ? 11 : 13); i++){


		// byte substitution
		byteSub(state);
		
		// Shift row layer
		shiftRow(state);

		// Mix column layer
		mixCol(state);

		// perform key addition
		applyKey(state, k, keyIndex);
	}

	// Last round without mixing columns
	byteSub(state);
	shiftRow(state);
	applyKey(state, k, keyIndex);

	delete[] k;
	k = nullptr;


	// store state in output
	int index = 0;
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			output[index++] = state[j][i];
		}
	}
}
