#include "main.h"

//CT 출력 함수
void CT_printstate(byte* data) {
	int cnt_i;
	printf("CT: ");
	for (cnt_i = 0; cnt_i < 16; cnt_i++) {
		printf("%02X ", *(data+cnt_i));
	}
	printf("\n");
	printf("\n");
}

//복호화된 PT 출력 함수
void DEC_PT_printstate(byte* data) {
	int cnt_i;
	printf("DEC_PT: ");
	for (cnt_i = 0; cnt_i < 16; cnt_i++) {
		printf("%02X ", *(data + cnt_i));
	}
	printf("\n");
	printf("\n");
}


int main() {
	//ECB 모드 기준으로 진행
	byte PT[16] = { 0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff };
	byte MASTER_KEY[16] = { 0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f };
	byte CT[16] = { 0x00, };
	byte DEC_PT[16] = { 0X00, };

	//암호화 연산
	AES_ENC(PT,MASTER_KEY,CT);
	CT_printstate(CT);
	
	//복호화 연산
	AES_DEC(CT, MASTER_KEY, DEC_PT);
	DEC_PT_printstate(DEC_PT);
}



