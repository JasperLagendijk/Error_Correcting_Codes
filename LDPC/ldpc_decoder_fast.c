#include <stdio.h>
#include <stdlib.h>
#include "parity.h"

#define UP 1
#define DOWN 0
#define VAR 0
#define CHECK 1


void message_up(int mu[2][LEN_PARITY], int codeword[COLS]) //Calculates messages going from variable nodes to check nodes using min_sum algorithm
{
	//Step 1. Precompute messages
	int pre_computed[COLS] = {0};
	//Incoming messages into variable nodes
	for (int i = 0; i < COLS; i++)  pre_computed[i] += codeword[i];
	
	//Incoming messages from check nodes into var nodes
	for (int i = 0; i < LEN_PARITY; i++) pre_computed[parity[VAR][i]] += mu[DOWN][i];
	
	//Step 2. Set outgoing messages
	for (int i  = 0; i < LEN_PARITY; i++) mu[UP][i] = pre_computed[parity[VAR][i]] - mu[DOWN][i];

	
}

void message_down_min_sum(int mu[2][LEN_PARITY])  //Calculates messages from  check nodes to variable nodes using Min-Sum parity checking
{
	//Step 1. Precompute messages
	int pre_computed_sign[COLS-ROWS] = {0};
	int pre_computed_min[COLS-ROWS] = {[0 ... COLS-ROWS-1] = 100000};
	for (int i = 0; i < LEN_PARITY; i++)
	{
			int x = fast_abs(mu[UP][i]);
			pre_computed_sign[parity[CHECK][i]] += fast_sign(mu[UP][i]);
			if (x < pre_computed_min[parity[CHECK][i]]) 
			{
				pre_computed_min[parity[CHECK][i]] = x;	
			//	pre_computed_index[parity[CHECK][i]] = i;
			}
	}
	
	//Step2. Calculate outgoing messages
	for (int i = 0; i < LEN_PARITY; i++)
	{
		int x = fast_sign(mu[UP][i]);
		int sign = 1 - (2 * ((x + pre_computed_sign[parity[CHECK][i]]) % 2));
		mu[DOWN][i] = sign*pre_computed_min[parity[CHECK][i]];
	}
	
}


void decode_ldpc_fast(int message_out[ROWS], int codeword[COLS])//, char decode_method)
{
	//Step 1. initialize messages
	int mu[2][LEN_PARITY] = {0};
		
	//Step 2. Belief propagation
	for (int i = 0; i < BP_MAX; i++)
	{
			//Messages up
			message_up(mu, codeword);
			
			//Messages Down
			message_down_min_sum(mu);
	}
	//Step 3. Return actual messages
	for (int i = 0; i < ROWS; i++) message_out[i] = codeword[i];
	
	for (int i = 0; i < LEN_PARITY; i++) 
	{
		message_out[parity[VAR][i]] += mu[DOWN][i];
	}
}

