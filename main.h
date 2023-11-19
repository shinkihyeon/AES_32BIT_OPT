#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef unsigned char byte;
typedef unsigned int word;

//Trans_Data_Type
word GETU32(byte* b);
void int2byte(word* input, byte* output);
void byte2int(byte* input, word* output);

//Key_Schedule_Func
word RotWord(word input);
word SubWord(word input);
word KeySchedule_F(word input, int rn);
void AES_KeySchedule(uint8_t* key, uint32_t rk[11][4]);
void Dec_keyschedule(byte* mk, word dec_rk[11][4]);

//GF_Func
byte GF_add(byte op1, byte op2);
byte GF_xtime(byte gf);
byte GF_mul(byte op1, byte op2);

//Gen_TE_TD_TABLE
void AES_ENC_TABLE();
void AES_DEC_TABLE();

//Enc
void ENC_ROUND(word* input, word* rk);
void AES_ENC(byte* pt, byte* mk, byte* ct);

//Dec
void DEC_ROUND(word* state, word* rk);
void AES_DEC(byte* ct, byte* mk, byte* pt);
