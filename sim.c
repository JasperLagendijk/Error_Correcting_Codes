#include <stdio.h>
#include <stdlib.h>

#include "ECC_Config.h"
//#ifdef USE_LDPC
#include "parity.h"
#include "ldpc.h"
//#endif

int main(int argc, char * argv[]) 
{
	
	int msg[ROWS] = {};
	int message[COLS] = {};
	int codeword[COLS];
	double BER = 0;
	LDPC_decoder dec;
	dec = construct_decoder();
	//for (int i = 0; i < ROWS; i++) printf("%d ", msg[i]);
	//printf("\n");
	srand (2);
	
		
	
	
	
	for (int j = 0; j < 10000; j++)
	{
		// Generate message
		for (int i = 0; i < ROWS; i++) msg[i] = rand() % 2;
		
		//Encode, transmit and decode message
		encode_ldpc(msg, codeword);
		
		for (int i = 0; i < COLS; i++) codeword[i] =4* ( (codeword[i] * 2 )-1) + (rand() % 11)-5;
		decode_ldpc(message, codeword, &dec);
		
		//Compare Bit error rates
		for (int i = 0; i < ROWS; i++) 
		{
			if (message[i] > 0) message[i] = 1;
			else message[i] = 0;
			if (message[i] != msg[i]) BER++;
		}
	
	}
	printf("BER: %f", BER/(324*1000));
}
