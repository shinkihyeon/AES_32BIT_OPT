#include "main.h"
#include "OP_TABLE.h"

//암호화 연산에 쓰이는 사전계산 테이블
//AES-32bit ver TE0,TE1,TE2,TE3 테이블 생성
void AES_ENC_TABLE() {
	word TE0[256];
	word TE1[256];
	word TE2[256];
	word TE3[256];
	int x;
	for (x = 0; x < 256; x++) {
		TE0[x] = (GF_xtime(sbox[x]) << 24) ^ (sbox[x] << 16) ^ (sbox[x] << 8) ^ (GF_xtime(sbox[x]) ^ sbox[x]);
		TE1[x] = (TE0[x] >> 8) ^ ((TE0[x] & 0xff) << 24);
		TE2[x] = (TE1[x] >> 8) ^ ((TE1[x] & 0xff) << 24);
		TE3[x] = (TE2[x] >> 8) ^ ((TE2[x] & 0xff) << 24);
	}
	//출력
	int i;
	for (i = 0; i < 256; i++) {
		printf("0x%08x, ", TE0[i]);
		if (i % 4 == 3) printf("\n");
	}
	printf("\n");
	for (i = 0; i < 256; i++) {
		printf("0x%08x, ", TE1[i]);
		if (i % 4 == 3) printf("\n");
	}
	printf("\n");
	for (i = 0; i < 256; i++) {
		printf("0x%08x, ", TE2[i]);
		if (i % 4 == 3) printf("\n");
	}
	printf("\n");
	for (i = 0; i < 256; i++) {
		printf("0x%08x, ", TE3[i]);
		if (i % 4 == 3) printf("\n");
	}
}


//복호화 연산에 쓰이는 사전계산 테이블
//AES-32bit ver TD0,TD1,TD2,TD3 테이블 생성
void AES_DEC_TABLE() {
	word TD0[256];
	word TD1[256];
	word TD2[256];
	word TD3[256];

	int i;
	for (i = 0; i < 256; i++) {
		byte x = isbox[i];
		byte x02 = GF_xtime(x);
		byte x04 = GF_xtime(x02);
		byte x08 = GF_xtime(x04);
		byte x0e = x08 ^ x04 ^ x02;
		byte x0b = x08 ^ x02 ^ x;
		byte x0d = x08 ^ x04 ^ x;
		byte x09 = x08 ^ x;

		TD0[i] = (x0e << 24) ^ (x09 << 16) ^ (x0d << 8) ^ x0b;
		TD1[i] = (TD0[i] >> 8) ^ (x0b << 24);
		TD2[i] = (TD1[i] >> 8) ^ (x0d << 24);
		TD3[i] = (TD2[i] >> 8) ^ (x09 << 24);
	}
	//출력
	for (i = 0; i < 256; i++) {
		printf("0x%08x, ", TD0[i]);
		if (i % 4 == 3) printf("\n");
	}
	printf("\n");
	for (i = 0; i < 256; i++) {
		printf("0x%08x, ", TD1[i]);
		if (i % 4 == 3) printf("\n");
	}
	printf("\n");
	for (i = 0; i < 256; i++) {
		printf("0x%08x, ", TD2[i]);
		if (i % 4 == 3) printf("\n");
	}
	printf("\n");
	for (i = 0; i < 256; i++) {
		printf("0x%08x, ", TD3[i]);
		if (i % 4 == 3) printf("\n");
	}
}
