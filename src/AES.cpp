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

#if defined(__x86_64__) || defined(__i386__)

	// 128 bit key HW scheduler
	__m128i* AES::genKey128_HW(__m128i key){
		
		__m128i* W = new __m128i[11];
		W[0] = key;

		// Round 1
		__m128i temp = _mm_aeskeygenassist_si128(W[0],0x01);
		temp = _mm_shuffle_epi32(temp, _MM_SHUFFLE(3,3,3,3));

		__m128i tk = W[0];
		tk = _mm_xor_si128(tk, _mm_slli_si128(tk, 4));
		tk = _mm_xor_si128(tk, _mm_slli_si128(tk, 4));
		tk = _mm_xor_si128(tk, _mm_slli_si128(tk, 4));

		tk = _mm_xor_si128(tk, temp);

		W[1] = tk;

		// Round 2
		temp = _mm_aeskeygenassist_si128(W[1],0x02);
		temp = _mm_shuffle_epi32(temp, _MM_SHUFFLE(3,3,3,3));

		tk = W[1];
		tk = _mm_xor_si128(tk, _mm_slli_si128(tk, 4));
		tk = _mm_xor_si128(tk, _mm_slli_si128(tk, 4));
		tk = _mm_xor_si128(tk, _mm_slli_si128(tk, 4));

		tk = _mm_xor_si128(tk, temp);

		W[2] = tk;

		// Round 3
		temp = _mm_aeskeygenassist_si128(W[2],0x04);
		temp = _mm_shuffle_epi32(temp, _MM_SHUFFLE(3,3,3,3));

		tk = W[2];
		tk = _mm_xor_si128(tk, _mm_slli_si128(tk, 4));
		tk = _mm_xor_si128(tk, _mm_slli_si128(tk, 4));
		tk = _mm_xor_si128(tk, _mm_slli_si128(tk, 4));

		tk = _mm_xor_si128(tk, temp);

		W[3] = tk;

		// Round 4
		temp = _mm_aeskeygenassist_si128(W[3],0x08);
		temp = _mm_shuffle_epi32(temp, _MM_SHUFFLE(3,3,3,3));

		tk = W[3];
		tk = _mm_xor_si128(tk, _mm_slli_si128(tk, 4));
		tk = _mm_xor_si128(tk, _mm_slli_si128(tk, 4));
		tk = _mm_xor_si128(tk, _mm_slli_si128(tk, 4));

		tk = _mm_xor_si128(tk, temp);

		W[4] = tk;

		// Round 5
		temp = _mm_aeskeygenassist_si128(W[4],0x10);
		temp = _mm_shuffle_epi32(temp, _MM_SHUFFLE(3,3,3,3));

		tk = W[4];
		tk = _mm_xor_si128(tk, _mm_slli_si128(tk, 4));
		tk = _mm_xor_si128(tk, _mm_slli_si128(tk, 4));
		tk = _mm_xor_si128(tk, _mm_slli_si128(tk, 4));

		tk = _mm_xor_si128(tk, temp);

		W[5] = tk;

		// Round 6
		temp = _mm_aeskeygenassist_si128(W[5],0x20);
		temp = _mm_shuffle_epi32(temp, _MM_SHUFFLE(3,3,3,3));

		tk = W[5];
		tk = _mm_xor_si128(tk, _mm_slli_si128(tk, 4));
		tk = _mm_xor_si128(tk, _mm_slli_si128(tk, 4));
		tk = _mm_xor_si128(tk, _mm_slli_si128(tk, 4));

		tk = _mm_xor_si128(tk, temp);

		W[6] = tk;

		// Round 7
		temp = _mm_aeskeygenassist_si128(W[6],0x40);
		temp = _mm_shuffle_epi32(temp, _MM_SHUFFLE(3,3,3,3));

		tk = W[6];
		tk = _mm_xor_si128(tk, _mm_slli_si128(tk, 4));
		tk = _mm_xor_si128(tk, _mm_slli_si128(tk, 4));
		tk = _mm_xor_si128(tk, _mm_slli_si128(tk, 4));

		tk = _mm_xor_si128(tk, temp);

		W[7] = tk;

		// Round 8
		temp = _mm_aeskeygenassist_si128(W[7],0x80);
		temp = _mm_shuffle_epi32(temp, _MM_SHUFFLE(3,3,3,3));

		tk = W[7];
		tk = _mm_xor_si128(tk, _mm_slli_si128(tk, 4));
		tk = _mm_xor_si128(tk, _mm_slli_si128(tk, 4));
		tk = _mm_xor_si128(tk, _mm_slli_si128(tk, 4));

		tk = _mm_xor_si128(tk, temp);

		W[8] = tk;


		// Round 9
		temp = _mm_aeskeygenassist_si128(W[8],0x1B);
		temp = _mm_shuffle_epi32(temp, _MM_SHUFFLE(3,3,3,3));

		tk = W[8];
		tk = _mm_xor_si128(tk, _mm_slli_si128(tk, 4));
		tk = _mm_xor_si128(tk, _mm_slli_si128(tk, 4));
		tk = _mm_xor_si128(tk, _mm_slli_si128(tk, 4));

		tk = _mm_xor_si128(tk, temp);

		W[9] = tk;


		// Round 10
		temp = _mm_aeskeygenassist_si128(W[9],0x36);
		temp = _mm_shuffle_epi32(temp, _MM_SHUFFLE(3,3,3,3));

		tk = W[9];
		tk = _mm_xor_si128(tk, _mm_slli_si128(tk, 4));
		tk = _mm_xor_si128(tk, _mm_slli_si128(tk, 4));
		tk = _mm_xor_si128(tk, _mm_slli_si128(tk, 4));

		tk = _mm_xor_si128(tk, temp);

		W[10] = tk;

		return W;
	}

	// 256 bit key HW scheduler
	__m128i* AES::genKey256_HW(__m128i key1, __m128i key2){
		__m128i* W = new __m128i[15];
		W[0] = key1;
		W[1] = key2;

		// Round 1
		__m128i temp1 = W[0];
		__m128i temp2 = W[1];

		__m128i t1 = _mm_aeskeygenassist_si128(temp2, 0x01);

		t1 = _mm_shuffle_epi32(t1, _MM_SHUFFLE(3,3,3,3));

		temp1 = _mm_xor_si128(temp1, _mm_slli_si128(temp1, 4));
		temp1 = _mm_xor_si128(temp1, _mm_slli_si128(temp1, 4));
		temp1 = _mm_xor_si128(temp1, _mm_slli_si128(temp1, 4));
		temp1 = _mm_xor_si128(temp1, t1);

		W[2] = temp1;

		temp2 = _mm_xor_si128(temp2, _mm_slli_si128(temp2, 4));
		temp2 = _mm_xor_si128(temp2, _mm_slli_si128(temp2, 4));
		temp2 = _mm_xor_si128(temp2, _mm_slli_si128(temp2, 4));
		W[3] = _mm_xor_si128(temp2, W[2]);



		// Round 2
		temp1 = W[2];
		temp2 = W[3];

		t1 = _mm_aeskeygenassist_si128(temp2, 0x02);

		t1 = _mm_shuffle_epi32(t1, _MM_SHUFFLE(3,3,3,3));

		temp1 = _mm_xor_si128(temp1, _mm_slli_si128(temp1, 4));
		temp1 = _mm_xor_si128(temp1, _mm_slli_si128(temp1, 4));
		temp1 = _mm_xor_si128(temp1, _mm_slli_si128(temp1, 4));
		temp1 = _mm_xor_si128(temp1, t1);

		W[4] = temp1;

		temp2 = _mm_xor_si128(temp2, _mm_slli_si128(temp2, 4));
		temp2 = _mm_xor_si128(temp2, _mm_slli_si128(temp2, 4));
		temp2 = _mm_xor_si128(temp2, _mm_slli_si128(temp2, 4));
		W[5] = _mm_xor_si128(temp2, W[4]);


		// Round 3
		temp1 = W[4];
		temp2 = W[5];

		t1 = _mm_aeskeygenassist_si128(temp2, 0x04);

		t1 = _mm_shuffle_epi32(t1, _MM_SHUFFLE(3,3,3,3));

		temp1 = _mm_xor_si128(temp1, _mm_slli_si128(temp1, 4));
		temp1 = _mm_xor_si128(temp1, _mm_slli_si128(temp1, 4));
		temp1 = _mm_xor_si128(temp1, _mm_slli_si128(temp1, 4));
		temp1 = _mm_xor_si128(temp1, t1);

		W[6] = temp1;

		temp2 = _mm_xor_si128(temp2, _mm_slli_si128(temp2, 4));
		temp2 = _mm_xor_si128(temp2, _mm_slli_si128(temp2, 4));
		temp2 = _mm_xor_si128(temp2, _mm_slli_si128(temp2, 4));
		W[7] = _mm_xor_si128(temp2, W[6]);



		// Round 4
		temp1 = W[6];
		temp2 = W[7];

		t1 = _mm_aeskeygenassist_si128(temp2, 0x08);

		t1 = _mm_shuffle_epi32(t1, _MM_SHUFFLE(3,3,3,3));

		temp1 = _mm_xor_si128(temp1, _mm_slli_si128(temp1, 4));
		temp1 = _mm_xor_si128(temp1, _mm_slli_si128(temp1, 4));
		temp1 = _mm_xor_si128(temp1, _mm_slli_si128(temp1, 4));
		temp1 = _mm_xor_si128(temp1, t1);

		W[8] = temp1;

		temp2 = _mm_xor_si128(temp2, _mm_slli_si128(temp2, 4));
		temp2 = _mm_xor_si128(temp2, _mm_slli_si128(temp2, 4));
		temp2 = _mm_xor_si128(temp2, _mm_slli_si128(temp2, 4));
		W[9] = _mm_xor_si128(temp2, W[8]);



		// Round 5
		temp1 = W[8];
		temp2 = W[9];

		t1 = _mm_aeskeygenassist_si128(temp2, 0x10);

		t1 = _mm_shuffle_epi32(t1, _MM_SHUFFLE(3,3,3,3));

		temp1 = _mm_xor_si128(temp1, _mm_slli_si128(temp1, 4));
		temp1 = _mm_xor_si128(temp1, _mm_slli_si128(temp1, 4));
		temp1 = _mm_xor_si128(temp1, _mm_slli_si128(temp1, 4));
		temp1 = _mm_xor_si128(temp1, t1);

		W[10] = temp1;

		temp2 = _mm_xor_si128(temp2, _mm_slli_si128(temp2, 4));
		temp2 = _mm_xor_si128(temp2, _mm_slli_si128(temp2, 4));
		temp2 = _mm_xor_si128(temp2, _mm_slli_si128(temp2, 4));
		W[11] = _mm_xor_si128(temp2, W[10]);


		// Round 6
		temp1 = W[10];
		temp2 = W[11];

		t1 = _mm_aeskeygenassist_si128(temp2, 0x20);

		t1 = _mm_shuffle_epi32(t1, _MM_SHUFFLE(3,3,3,3));

		temp1 = _mm_xor_si128(temp1, _mm_slli_si128(temp1, 4));
		temp1 = _mm_xor_si128(temp1, _mm_slli_si128(temp1, 4));
		temp1 = _mm_xor_si128(temp1, _mm_slli_si128(temp1, 4));
		temp1 = _mm_xor_si128(temp1, t1);

		W[12] = temp1;

		temp2 = _mm_xor_si128(temp2, _mm_slli_si128(temp2, 4));
		temp2 = _mm_xor_si128(temp2, _mm_slli_si128(temp2, 4));
		temp2 = _mm_xor_si128(temp2, _mm_slli_si128(temp2, 4));
		W[13] = _mm_xor_si128(temp2, W[12]);


		// Round 7
		temp1 = W[12];
		temp2 = W[13];

		t1 = _mm_aeskeygenassist_si128(temp2, 0x40);

		t1 = _mm_shuffle_epi32(t1, _MM_SHUFFLE(3,3,3,3));

		temp1 = _mm_xor_si128(temp1, _mm_slli_si128(temp1, 4));
		temp1 = _mm_xor_si128(temp1, _mm_slli_si128(temp1, 4));
		temp1 = _mm_xor_si128(temp1, _mm_slli_si128(temp1, 4));
		W[14] = _mm_xor_si128(temp1, t1);


		return W;

	}

#elif defined(__aarch64__) || defined(__arm64__)
	uint8x16_t* AES::genKey128_HW_ARM(unsigned char* key){

		// has 44 times 4 bytes
		unsigned int* temp_key = genKey128(key); 

		// convert key format
		// 11 rounds of 16 bytes --> need 16 byte allignment
		uint8x16_t* res_key = static_cast<uint8x16_t*>(std::aligned_alloc(16, 176));

		for(int i = 0; i < 11; i++){
			uint8_t temp[16];

			for(int j = 0; j < 4; j++){
				uint32_t single_word = temp_key[i * 4 + j];

				temp[j * 4 + 0] = (single_word >> 24) & 0xFF;
				temp[j * 4 + 1] = (single_word >> 16) & 0xFF;
				temp[j * 4 + 2] = (single_word >> 8) & 0xFF;
				temp[j * 4 + 3] = single_word & 0xFF;
			}

			res_key[i] = vld1q_u8(temp);
		}

		delete[] temp_key;
		return res_key;
	}

	uint8x16_t* AES::genKey192_HW_ARM(unsigned char* key){

		// has 44 times 4 bytes
		unsigned int* temp_key = genKey192(key); 

		// convert key format
		// 11 rounds of 16 bytes --> need 16 byte allignment
		uint8x16_t* res_key = static_cast<uint8x16_t*>(std::aligned_alloc(16, 208));

		for(int i = 0; i < 13; i++){
			uint8_t temp[16];

			for(int j = 0; j < 4; j++){
				uint32_t single_word = temp_key[i * 4 + j];

				temp[j * 4 + 0] = (single_word >> 24) & 0xFF;
				temp[j * 4 + 1] = (single_word >> 16) & 0xFF;
				temp[j * 4 + 2] = (single_word >> 8) & 0xFF;
				temp[j * 4 + 3] = single_word & 0xFF;
			}

			res_key[i] = vld1q_u8(temp);
		}

		delete[] temp_key;
		return res_key;
	}

	uint8x16_t* AES::genKey256_HW_ARM(unsigned char* key){
		
		unsigned int* temp_key = genKey256(key); 

		// convert key format
		// 11 rounds of 16 bytes --> need 16 byte allignment
		uint8x16_t* res_key = static_cast<uint8x16_t*>(std::aligned_alloc(16, 240));

		for(int i = 0; i < 15; i++){
			uint8_t temp[16];

			for(int j = 0; j < 4; j++){
				uint32_t single_word = temp_key[i * 4 + j];

				temp[j * 4 + 0] = (single_word >> 24) & 0xFF;
				temp[j * 4 + 1] = (single_word >> 16) & 0xFF;
				temp[j * 4 + 2] = (single_word >> 8) & 0xFF;
				temp[j * 4 + 3] = single_word & 0xFF;
			}

			res_key[i] = vld1q_u8(temp);
		}

		delete[] temp_key;
		return res_key;
	}
#endif

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

#if defined(__x86_64__) || defined(__i386__)
	void AES::HW_GHASH(unsigned char* prev_g, unsigned char* input, int input_index, const unsigned char* HASH_SUBKEY){

		__m128i prev_g_temp = _mm_loadu_si128(reinterpret_cast<const __m128i*>(prev_g));
		__m128i input_temp = _mm_loadu_si128(reinterpret_cast<const __m128i*>(input+input_index));
		__m128i HASH_SUBKEY_temp = _mm_loadu_si128(reinterpret_cast<const __m128i*>(HASH_SUBKEY));

		prev_g_temp = _mm_xor_si128(prev_g_temp, input_temp);


		// gm multiplication
		__m128i X_high = _mm_unpackhi_epi64(prev_g_temp, prev_g_temp);
		__m128i X_low = _mm_unpacklo_epi64(prev_g_temp, prev_g_temp);

		__m128i H_high = _mm_unpackhi_epi64(HASH_SUBKEY_temp, HASH_SUBKEY_temp);
		__m128i H_low = _mm_unpacklo_epi64(HASH_SUBKEY_temp, HASH_SUBKEY_temp);

		__m128i Z0 = _mm_clmulepi64_si128(prev_g_temp, HASH_SUBKEY_temp, 0x00);
		__m128i Z1 = _mm_xor_si128(
			_mm_clmulepi64_si128(prev_g_temp, HASH_SUBKEY_temp, 0x01),
			_mm_clmulepi64_si128(prev_g_temp, HASH_SUBKEY_temp, 0x10)
		);

		__m128i Z2 = _mm_clmulepi64_si128(prev_g_temp, HASH_SUBKEY_temp, 0x11);

		__m128i T1 = _mm_slli_si128(Z1, 8);
		__m128i T2 = _mm_slli_si128(Z1, 8);

		__m128i acc_lo = _mm_xor_si128(Z0, T1);
		__m128i acc_hi = _mm_xor_si128(Z2, T2);

		const __m128i R = _mm_set_epi32(0xE1000000, 0, 0, 0);

		__m128i V1 = _mm_srli_epi64(acc_hi, 63);
		__m128i V2 = _mm_slli_epi64(V1, 1);    
		__m128i V3 = _mm_slli_epi64(V1, 2);     
		__m128i V7 = _mm_slli_epi64(V1, 7);


		__m128i reduction = _mm_xor_si128(_mm_xor_si128(V2, V3), V7);
		__m128i result = _mm_xor_si128(acc_lo, reduction);

		_mm_storeu_si128(reinterpret_cast<__m128i*>(prev_g), result);
	}
#elif defined(__aarch64__) || defined(__arm64__)
	void AES::HW_ARM_GHASH(unsigned char* prev_g, unsigned char* input, int input_index, const unsigned char* HASH_SUBKEY){

		uint8x16_t prev_g_temp = vld1q_u8(reinterpret_cast<const uint8_t*>(prev_g));
		uint8x16_t input_temp = vld1q_u8(reinterpret_cast<const uint8_t*>(input+input_index));
		uint8x16_t HASH_SUBKEY_temp = vld1q_u8(reinterpret_cast<const uint8_t*>(HASH_SUBKEY));

		prev_g_temp = veorq_u8(prev_g_temp, input_temp);

		// gm multiplication
		poly64x2_t x_poly = vreinterpretq_p64_u8(prev_g_temp);
    	poly64x2_t h_poly = vreinterpretq_p64_u8(HASH_SUBKEY_temp);

	
		poly64_t x_lo = vget_lane_p64(vget_low_p64(x_poly),0);
		poly64_t x_hi = vget_lane_p64(vget_high_p64(x_poly),0);
		poly64_t h_lo = vget_lane_p64(vget_low_p64(h_poly),0);
		poly64_t h_hi = vget_lane_p64(vget_high_p64(h_poly),0);

		poly128_t R00 = vmull_p64(x_lo, h_lo); 
		poly128_t R01 = vmull_p64(x_lo, h_hi); 
		poly128_t R10 = vmull_p64(x_hi, h_lo); 
		poly128_t R11 = vmull_p64(x_hi, h_hi); 

		poly128_t R_mid = vreinterpretq_p128_u8(
			veorq_u8(
				vreinterpretq_u8_p128(R01),
				vreinterpretq_u8_p128(R10)
			)
		);


		uint8x16_t R_lo = vreinterpretq_u8_p128(R00);
		uint8x16_t R_mid_u8 = vreinterpretq_u8_p128(R_mid);
		uint8x16_t R_hi = vreinterpretq_u8_p128(R11);

		uint8x16_t tmp = veorq_u8(
			veorq_u8(
				veorq_u8(
					vshlq_n_u8(R_hi, 1),
					vshlq_n_u8(R_hi, 2)
				),
				vshlq_n_u8(R_hi, 7)
			),
			R_hi
		);

		tmp = veorq_u8(tmp, vextq_u8(R_mid_u8, R_mid_u8, 8));

		uint8x16_t result = veorq_u8(R_lo, tmp);

		vst1q_u8(reinterpret_cast<uint8_t*>(prev_g), result);
	}
#endif

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


void AES::HW_auth_tag(unsigned char* nonce, unsigned char* key, const int& keySize, unsigned char* AD, int AD_size, unsigned char* Y, const int Y_size, unsigned char* TAG){
	unsigned char HASH_SUBKEY[16]{0};
	unsigned char g[16]{0};

	#if defined(__x86_64__) || defined(__i386__)
		HW_ENCRYPT_CTR(g, key, keySize, HASH_SUBKEY);
	#elif defined(__aarch64__) || defined(__arm64__)
		HW_ARM_ENCRYPT_CTR(g, key, keySize, HASH_SUBKEY);
	#endif

	int original_AD_size = AD_size;
	int padded_size = 0;
	unsigned char* temp = nullptr;

	// Pad AD
	if(AD_size > 0){
		temp = pad_AD(AD, AD_size, padded_size);
		delete[] AD;

		// Process AD
		#if defined(__x86_64__) || defined(__i386__)
			for(int i = 0; i < padded_size; i+=16){
				HW_GHASH(g, temp, i, HASH_SUBKEY);
			}
		#elif defined(__aarch64__) || defined(__arm64__)
			for(int i = 0; i < padded_size; i+=16){
				HW_ARM_GHASH(g, temp, i, HASH_SUBKEY);
			}
		#endif

		delete[] temp;
	}

	// Process Y
	#if defined(__x86_64__) || defined(__i386__)
		for(int i = 0; i < Y_size; i+=16){
			HW_GHASH(g, Y, i, HASH_SUBKEY);
		}
	#elif defined(__aarch64__) || defined(__arm64__)
		for(int i = 0; i < Y_size; i+=16){
			HW_ARM_GHASH(g, Y, i, HASH_SUBKEY);
		}
	#endif

	// AD + text length addition
	unsigned char len_block[16]{0};
	int bit_size1 = original_AD_size*8;
	int bit_size2 = Y_size*8;
	
	for(int i = 0; i < 8; i++){
		len_block[7-i] = (bit_size1>>(i*8))&0xFF;
		len_block[15-i] = (bit_size2>>(i*8))&0xFF;
	}

	#if defined(__x86_64__) || defined(__i386__)
		HW_GHASH(g, len_block, 0, HASH_SUBKEY);
	#elif defined(__aarch64__) || defined(__arm64__)
		HW_ARM_GHASH(g, len_block, 0, HASH_SUBKEY);
	#endif

	// encrypt (Nonce||0^31||1);
	unsigned char J[16]{0};

	memcpy(J, nonce, 12);
	J[15] = 0x01;

	unsigned char tag_mask[16]{0};
	#if defined(__x86_64__) || defined(__i386__)
		HW_ENCRYPT_CTR(J, key, keySize, tag_mask);
	#elif defined(__aarch64__) || defined(__arm64__)
		HW_ARM_ENCRYPT_CTR(J, key, keySize, tag_mask);
	#endif

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

#if defined(__x86_64__) || defined(__i386__)
	// HW CTR encryption
	void AES::HW_ENCRYPT_CTR(unsigned char* nonce, unsigned char* key, const int& keySize, unsigned char* buffer){

		// load none
		__m128i block = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nonce));

		// load key
		__m128i key0  = _mm_loadu_si128(reinterpret_cast<const __m128i*>(key));
		__m128i key1;

		if(keySize == 256){
			key1 =  _mm_loadu_si128(reinterpret_cast<const __m128i*>(key+16));
		}

		// expand key
		__m128i* expanded_key = nullptr;

		if(keySize == 128){
			expanded_key = genKey128_HW(key0);
		}
		else{
			expanded_key = genKey256_HW(key0, key1);
		}

		// Key Whitening
		block = _mm_xor_si128(block, expanded_key[0]);

		// Perform encryption

		// All round except last
		for(int i = 1; i < (keySize == 128 ? 10 : 14); i++){
			block = _mm_aesenc_si128(block, expanded_key[i]);
		}

		// last round 
		block = _mm_aesenclast_si128(block, expanded_key[(keySize == 128 ? 10 : 14)]);

		// XOR with text/ciphertext
		__m128i temp_buffer = _mm_loadu_si128(reinterpret_cast<const __m128i*>(buffer));

		block = _mm_xor_si128(block, temp_buffer);

		// store in output
		_mm_storeu_si128(reinterpret_cast<__m128i*>(buffer), block);

		delete[] expanded_key;
		expanded_key = nullptr;
	}

#elif defined(__aarch64__) || defined(__arm64__)
	void AES::HW_ARM_ENCRYPT_CTR(unsigned char* nonce, unsigned char* key, const int& keySize, unsigned char* buffer){
		
		// load nonce
		uint8x16_t block = vld1q_u8(reinterpret_cast<const uint8_t*>(nonce));

		// expand key
		uint8x16_t* expanded_key = nullptr;

		if(keySize == 128){
			expanded_key = genKey128_HW_ARM(key);
		}
		else if(keySize == 192){
			expanded_key = genKey192_HW_ARM(key);
		}
		else{
			expanded_key = genKey256_HW_ARM(key);
		}

		// key whitening
		block = veorq_u8(block, expanded_key[0]);

		// Encryption (except last round)
		for(int i = 1; i < (keySize == 128 ? 10 : (keySize == 192) ? 12 : 14); i++){
			block = vaeseq_u8(block, expanded_key[i]);
			block = vaesmcq_u8(block);
		}

		// last round
		block = vaeseq_u8(block, expanded_key[(keySize == 128 ? 10 : (keySize == 192) ? 12 : 14)]);

		// XOR with text/ciphertext
		uint8x16_t temp_buffer = vld1q_u8(reinterpret_cast<const uint8_t*>(buffer));

		block = veorq_u8(block, temp_buffer);

		// store output
		vst1q_u8(reinterpret_cast<uint8_t*>(buffer), block);

		delete[] expanded_key;
		expanded_key = nullptr;
	}
#endif


