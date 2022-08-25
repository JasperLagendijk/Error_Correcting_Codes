#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ECC_Config.h"
//#ifdef USE_LDPC
#include "parity.h"
#include "ldpc_encoder.h"
#include "ldpc_decoder_fast.h"
#include "ldpc_decoder_oo.h"
//#endif

#define NSIMS 1000


double * decode_ldpc(double LLRs[648], char s, double c)
{	
	//printf("test");
	double * messages = (double *) calloc(324, sizeof(double));
	LDPC_decoder dec = construct_decoder();
	decode_ldpc_oo(messages, LLRs, &dec, 's', c);
	return messages;
}


int * encode(int message[ROWS])
{
	int * codeword = (int *) calloc(648, sizeof(int));
	//printf("[");
	//for (int i = 0; i < 324; i++) printf("%d ", message[i]);
	//printf("]\n");	
	encode_ldpc(message, codeword);
	return codeword;
}
/*int main(int argc, char * argv[]) 
{
	
	int msg[ROWS] = {};
	int message[COLS] = {};
	int codeword[COLS];
	int codeword_enc[COLS];
	double BER = 0;
	double BER_EXP = 0;
	LDPC_decoder dec;
	dec = construct_decoder();
	//for (int i = 0; i < ROWS; i++) printf("%d ", msg[i]);
	//printf("\n");
	srand (time(NULL));
	
		
	
	
	
	for (int j = 0; j < NSIMS; j++)
	{
		// Generate message
		for (int i = 0; i < ROWS; i++) msg[i] = rand() % 2;

		//Encode, transmit and decode message
		encode_ldpc(msg, codeword);
		codeword[0] = 1-codeword[0];
		for (int i = 0; i < COLS; i++) codeword[i] = -2*( (codeword[i] * 2 )-1); //- (rand() % 10)-5;
		for (int i = 0; i < COLS; i++) codeword_enc[i] = codeword[i];
		decode_ldpc_oo(message, codeword, &dec, 'g');
		//decode_ldpc_fast(message, codeword);
		
		//Compare Bit error rates
		for (int i = 0; i < ROWS; i++) 
		{
			if (message[i] < 0) message[i] = 1;
			else message[i] = 0;
			if (message[i] != msg[i]) BER++;
			
			if (codeword_enc[i] < 0) codeword_enc[i] = 1;
			else codeword_enc[i] = 0;
			if (codeword_enc[i] != msg[i]) BER_EXP++;
		}
	
	}
	printf("BER: %f\n",  BER/(324*NSIMS));
	printf("BER EXP: %f\n", BER_EXP/(324*NSIMS));
	
}*/
