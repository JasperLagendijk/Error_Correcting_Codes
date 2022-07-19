#ifndef PARITY
#define PARITY 

#define ROWS 324
#define COLS 648
#define GAP 27
#define MAX_VAR_NODE 8
#define MAX_CHECK_NODE 12

#define LEN_PARITY 2376
#define LEN_A 1566
#define LEN_B 54
#define LEN_T_INV 1782
#define LEN_P1_ENC 1512

#define bit_size short int 
#define BP_MAX 20	
	extern unsigned int parity[2][2376];
	extern unsigned bit_size A[2][1566];
	extern unsigned bit_size B[2][54];
	extern bit_size T_inv[3][1782];
	extern bit_size p1_encoder[3][1512];

	int fast_abs(int x);
	int sign(int x);
#endif
