#include "main.h"
#include "OP_TABLE.h"

//1R~9R SB -> SR -> MC -> AR 과정을 사전계산한 TABLE(TE0, TE1, TE2, TE3)을 이용해서 구현
void ENC_ROUND(word* input, word* rk) {
	word temp[4] = { 0, };
	*(temp + 0) = TE0[(*(input + 0)) >> 24] ^ TE1[((*(input + 1)) >> 16) & 0xff] ^ TE2[((*(input + 2)) >> 8) & 0xff] ^ TE3[(*(input + 3)) & 0xff] ^ *(rk + 0);
	*(temp + 1) = TE0[(*(input + 1)) >> 24] ^ TE1[((*(input + 2)) >> 16) & 0xff] ^ TE2[((*(input + 3)) >> 8) & 0xff] ^ TE3[(*(input + 0)) & 0xff] ^ *(rk + 1);
	*(temp + 2) = TE0[(*(input + 2)) >> 24] ^ TE1[((*(input + 3)) >> 16) & 0xff] ^ TE2[((*(input + 0)) >> 8) & 0xff] ^ TE3[(*(input + 1)) & 0xff] ^ *(rk + 2);
	*(temp + 3) = TE0[(*(input + 3)) >> 24] ^ TE1[((*(input + 0)) >> 16) & 0xff] ^ TE2[((*(input + 1)) >> 8) & 0xff] ^ TE3[(*(input + 2)) & 0xff] ^ *(rk + 3);
	//input값을 update
	memcpy(input, temp, 4 * sizeof(word));
}


//AES-32bit ver 암호화 함수
void AES_ENC(byte* pt, byte* mk, byte* ct) {
	word state[4] = { 0, };
	byte2int(pt, state);
	//mk -> rk
	word rk[11][4];
	AES_KeySchedule(mk, rk);
	//0 Round
	*(state + 0) ^= rk[0][0];
	*(state + 1) ^= rk[0][1];
	*(state + 2) ^= rk[0][2];
	*(state + 3) ^= rk[0][3];
	//1R ~ 9R
	int i;
	for (i = 1; i < 10; i++) {
		ENC_ROUND(state, rk[i]);
	}
	//10R
	word temp[4] = { 0, };
	*(temp + 0) = sbox[(*(state + 0)) >> 24] << 24 ^ sbox[((*(state + 1)) >> 16) & 0xff] << 16 ^ sbox[((*(state + 2)) >> 8) & 0xff] << 8 ^ sbox[(*(state + 3)) & 0xff] ^ rk[10][0];
	*(temp + 1) = sbox[(*(state + 1)) >> 24] << 24 ^ sbox[((*(state + 2)) >> 16) & 0xff] << 16 ^ sbox[((*(state + 3)) >> 8) & 0xff] << 8 ^ sbox[(*(state + 0)) & 0xff] ^ rk[10][1];
	*(temp + 2) = sbox[(*(state + 2)) >> 24] << 24 ^ sbox[((*(state + 3)) >> 16) & 0xff] << 16 ^ sbox[((*(state + 0)) >> 8) & 0xff] << 8 ^ sbox[(*(state + 1)) & 0xff] ^ rk[10][2];
	*(temp + 3) = sbox[(*(state + 3)) >> 24] << 24 ^ sbox[((*(state + 0)) >> 16) & 0xff] << 16 ^ sbox[((*(state + 1)) >> 8) & 0xff] << 8 ^ sbox[(*(state + 2)) & 0xff] ^ rk[10][3];
	//temp -> state
	memcpy(state, temp, 4 * sizeof(word));
	//state -> ct
	int2byte(state, ct);
}