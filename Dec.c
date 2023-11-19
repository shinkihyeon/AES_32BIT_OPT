#include "main.h"
#include "OP_TABLE.h"

//1R~9R Inv_SubBytes, Inv_ShiftRows, Inv_MixColumn, Add_RoundKey 과정을 사전계산한 TABLE(TD0, TD1, TD2, TD3)을 이용해서 구현
void DEC_ROUND(word* state, word* rk) {
	word temp[4] = { 0, };
	*(temp + 0) = TD0[*(state + 0) >> 24] ^ TD1[(*(state + 3) >> 16) & 0xff] ^ TD2[(*(state + 2) >> 8) & 0xff] ^ TD3[*(state + 1) & 0xff] ^ *(rk + 0);
	*(temp + 1) = TD0[*(state + 1) >> 24] ^ TD1[(*(state + 0) >> 16) & 0xff] ^ TD2[(*(state + 3) >> 8) & 0xff] ^ TD3[*(state + 2) & 0xff] ^ *(rk + 1);
	*(temp + 2) = TD0[*(state + 2) >> 24] ^ TD1[(*(state + 1) >> 16) & 0xff] ^ TD2[(*(state + 0) >> 8) & 0xff] ^ TD3[*(state + 3) & 0xff] ^ *(rk + 2);
	*(temp + 3) = TD0[*(state + 3) >> 24] ^ TD1[(*(state + 2) >> 16) & 0xff] ^ TD2[(*(state + 1) >> 8) & 0xff] ^ TD3[*(state + 0) & 0xff] ^ *(rk + 3);
	memcpy(state, temp, 4 * sizeof(word));
}

//AES-32bit ver 복호화 함수
void AES_DEC(byte* ct, byte* mk, byte* pt) {
	//ct 형태를 16*byte -> 4*word
	word word_ct[4];
	byte2int(ct, word_ct);
	//Dec key-schedule
	word dec_rk[11][4];
	Dec_keyschedule(mk, dec_rk);
	//0R
	word_ct[0] ^= dec_rk[10][0];
	word_ct[1] ^= dec_rk[10][1];
	word_ct[2] ^= dec_rk[10][2];
	word_ct[3] ^= dec_rk[10][3];
	//1R ~ 9R
	int i;
	for (i = 9; i > 0; i--) {
		DEC_ROUND(word_ct, dec_rk[i]);
	}
	//10R
	word temp[4] = { 0, };
	*(temp + 0) = isbox[*(word_ct + 0) >> 24] << 24 ^ isbox[(*(word_ct + 3) >> 16) & 0xff] << 16 ^ isbox[(*(word_ct + 2) >> 8) & 0xff] << 8 ^ isbox[*(word_ct + 1) & 0xff] ^ dec_rk[0][0];
	*(temp + 1) = isbox[*(word_ct + 1) >> 24] << 24 ^ isbox[(*(word_ct + 0) >> 16) & 0xff] << 16 ^ isbox[(*(word_ct + 3) >> 8) & 0xff] << 8 ^ isbox[*(word_ct + 2) & 0xff] ^ dec_rk[0][1];
	*(temp + 2) = isbox[*(word_ct + 2) >> 24] << 24 ^ isbox[(*(word_ct + 1) >> 16) & 0xff] << 16 ^ isbox[(*(word_ct + 0) >> 8) & 0xff] << 8 ^ isbox[*(word_ct + 3) & 0xff] ^ dec_rk[0][2];
	*(temp + 3) = isbox[*(word_ct + 3) >> 24] << 24 ^ isbox[(*(word_ct + 2) >> 16) & 0xff] << 16 ^ isbox[(*(word_ct + 1) >> 8) & 0xff] << 8 ^ isbox[*(word_ct + 0) & 0xff] ^ dec_rk[0][3];
	memcpy(word_ct, temp, 4 * sizeof(word));
	//복호화된 word_ct를 16*byte 형태인 pt에 갖다써줘야!
	int2byte(word_ct, pt);
}