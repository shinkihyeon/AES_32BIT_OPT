#include "main.h"
#include "OP_TABLE.h"

//Ű������ �������� ���̴� �����
const word Rcon[10] = { 0x01000000,0x02000000,0x04000000,0x08000000,0x10000000,0x20000000,0x40000000,0x80000000,0x1b000000,0x36000000 };

word RotWord(word input) {
	return ((input << 8) | (input >> 24));
}

word SubWord(word input) {
	return (((sbox[input >> 24]) << 24) ^ ((sbox[(input >> 16) & 0xff]) << 16) ^ ((sbox[(input >> 8) & 0xff]) << 8) ^ (sbox[input & 0xff]));
}

word KeySchedule_F(word input, int rn) {
	return ((SubWord(RotWord(input))) ^ (Rcon[rn]));
}

//Enc�������� ���̴� KeySchedule �Լ�
void AES_KeySchedule(uint8_t* key, uint32_t rk[11][4]) {
	int cnt_i, cnt_j;
	for (cnt_i = 0; cnt_i < 4; cnt_i++) {
		rk[0][cnt_i] = GETU32(key + (4 * cnt_i));
	}

	//1ROUND~10ROUND
	for (cnt_i = 1; cnt_i < 11; cnt_i++) {
		for (cnt_j = 0; cnt_j < 4; cnt_j++) {
			if (cnt_j == 0) {
				rk[cnt_i][cnt_j] = (KeySchedule_F(rk[cnt_i - 1][3], (cnt_i - 1)) ^ rk[cnt_i - 1][0]);
			}
			else {
				rk[cnt_i][cnt_j] = (rk[cnt_i][cnt_j - 1] ^ rk[cnt_i - 1][cnt_j]);
			}
		}
	}
}

//EQ_DEC
//AdRoundKey, MixColumns������ ������ �ٲٱ� ���� ���ο� RoundKey�� ������ RoundKey�� Ȱ���ؼ� 
//���Ӱ� �����ϴ� ����
void Dec_keyschedule(byte* mk, word dec_rk[11][4]) {
	word rk[11][4];
	AES_KeySchedule(mk, rk);

	//dec_rk0,10�� �״��
	memcpy(dec_rk[0], rk[0], 4 * sizeof(word));
	memcpy(dec_rk[10], rk[10], 4 * sizeof(word));

	//dec_rk1~9 -> dec_rk=MC^-1(rk)
	int i;
	for (i = 1; i < 10; i++) {
		dec_rk[i][0] = TD0[sbox[rk[i][0] >> 24]] ^ TD1[sbox[(rk[i][0] >> 16) & 0xff]] ^ TD2[sbox[(rk[i][0] >> 8) & 0xff]] ^ TD3[sbox[(rk[i][0] & 0xff)]];
		dec_rk[i][1] = TD0[sbox[rk[i][1] >> 24]] ^ TD1[sbox[(rk[i][1] >> 16) & 0xff]] ^ TD2[sbox[(rk[i][1] >> 8) & 0xff]] ^ TD3[sbox[(rk[i][1] & 0xff)]];
		dec_rk[i][2] = TD0[sbox[rk[i][2] >> 24]] ^ TD1[sbox[(rk[i][2] >> 16) & 0xff]] ^ TD2[sbox[(rk[i][2] >> 8) & 0xff]] ^ TD3[sbox[(rk[i][2] & 0xff)]];
		dec_rk[i][3] = TD0[sbox[rk[i][3] >> 24]] ^ TD1[sbox[(rk[i][3] >> 16) & 0xff]] ^ TD2[sbox[(rk[i][3] >> 8) & 0xff]] ^ TD3[sbox[(rk[i][3] & 0xff)]];
	}
}