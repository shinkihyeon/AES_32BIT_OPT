#include "main.h"

//GF(2^8)상에서 연산진행시 필요한 함수들 
byte GF_add(byte op1, byte op2) {
	return (op1 ^ op2);
}

byte GF_xtime(byte gf) {
	return ((((gf >> 7) & 0x01) == 1) ? (gf << 1) ^ 0x1b : gf << 1);
}

byte GF_mul(byte op1, byte op2) {
	int i;
	byte temp = 0;
	for (i = 7; i > 0; i--) {
		temp = GF_xtime((((op1 >> i) & 0x01) * op2) ^ temp);
	}
	temp ^= (op1 & 0x01) * op2;
	return temp;
}