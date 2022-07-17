#include "parity.h"

void encode_ldpc(int msg[ROWS], int codeword[COLS])
{
	// Find p1
	// p1 = p1_encode * msg
	int p1[GAP] = {0};
	int p2[ROWS-GAP] = {0};
	
	for (int j = 0; j < LEN_P1_ENC; j++) p1[p1_encoder[1][j]] += p1_encoder[2][j]*msg[p1_encoder[0][j]];
	for (int i = 0; i < GAP; i++) p1[i] = fast_abs(p1[i] % 2);
	
	// Find p2
	int As[ROWS-GAP] = {0};
	int Bp[ROWS-GAP] = {0};
	
	for (int j = 0; j < LEN_A; j++) As[A[1][j]] += msg[A[0][j]];
	for (int j = 0; j < LEN_B; j++) Bp[B[1][j]] += p1[B[0][j]];
	for (int j = 0; j < LEN_T_INV; j++) p2[T_inv[1][j]] += T_inv[2][j]*(As[T_inv[0][j]]+Bp[T_inv[0][j]]);
	
	for (int i = 0; i < ROWS-GAP; i++) p2[i] = fast_abs(p2[i]) % 2;
	 //Codeword consists of 3 parts :[msg, p1, p2]
	for (int i = 0; i < ROWS; i++) codeword[i] = msg[i];
	for (int i = ROWS; i < ROWS+GAP; i++) codeword[i] = p1[i-ROWS];
	for (int i = ROWS+GAP; i < COLS; i++) codeword[i] = p2[i-ROWS-GAP];
}
