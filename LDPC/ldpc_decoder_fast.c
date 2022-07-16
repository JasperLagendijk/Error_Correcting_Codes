#include <stdio.h>
#include <stdlib.h>
#include "parity.h"

#define UP = 0;
#define DOWN = 1;


//int messages [LEN_PARITY][2] = {0}; 


void message_up(int mu[2][LEN_PARITY], int codeword[COLS]) //Calculates messages going from variable nodes to check nodes using min_sum algorithm
{
	//Step 1. Precompute messages
	int pre_computed[COLS] = {0};
	for (int i = 0; i < LEN_PARITY; i++) pre_computed[parity[UP][i]] += mu[DOWN][i];
	//Step 2. Set outgoing messages
	for (int i  = 0; i < LEN_PARITY; i++) mu[UP][i] = pre_computed[parity[UP][i]] - mu[DOWN][i];
}

void message_down_min_sum(int mu[2][LEN_PARITY)  //Calculates messages from  check nodes to variable nodes using Min-Sum parity checking
{
	//Step 1. Precompute messages
	int pre_computed_sign[ROWS] = {-1};
	int pre_computed_min[ROWS] = {0};
	for (int i = 0; i < LEN_PARITY; i++)
	{
					//Find absolute value of check->messages
			int x = mu[UP][i];
			int mask = x >> 31; //Set parity bit as first bit, set all other bits as 0
			x = x ^ mask; // Flip all bits, to change to 2's complement
			x = x - mask; // Fix first bit

			//x is absolute value
			pre_computed_sign[parity[DOWN][i]] *= (mask & 1); // Only returns 1 if the first bit of mask, the sign is 1; Not certain if this actually works
			if (x < pre_computed_min[parity[DOWN][i]])  pre_computed_min[parity[DOWN][i]] = x;
		
	}
	
	//Step2. Calculate outgoing messages
	
	
	
}


void decode_ldpc_fast(int message, int codeword[COLS], char decode_method)
{
	//Step 1. initialize messages
	int mu[2][LEN_PARITY] = {0};

	//Step 2. Belief propagation
	
	//Step 3. Return actual messages
}

