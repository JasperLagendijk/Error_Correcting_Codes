#include <stdio.h>
#include <stdlib.h>
#include "parity.h"
#include "ldpc_decoder_oo.h"

#define BP_MAX 20

int sign_b(int x)
{ 
	if (x > 0) return 1;
	else return 0;
}

void min_sum(struct checkNode * check) //Performs min-sum algorithm on values and messages and places them in messages, original incoming messages are lost
{
	int sig = 1;
	int min = 0;
	int min2 = 0;
	int j = 0;
	int l = 0;
	//Step 1: calculate mother sign
	min = abs(check->messages[0]);
	min2 = abs(check->messages[0]);
	for (int i = 0; i < check->connectedNodes; i++)
	{
			int x = check->messages[i];
			//Find absolute value of check->messages
			int mask = x >> 31;
			x = x ^ mask;
			x = x - mask;

			//x = nu abs x
			sig *= (mask & 1);
			if (x < min) 
			{
				min = x;
				j = i;
			}
	}
	for (int i = 0; i < check->connectedNodes; i++) check->messages[i] = min*sig*sign(check->messages[i])/2;
	
}

void message_out_variable(struct variableNode * var) //Calculates variable node value and places them in messages
{
	int msg = var->incoming_bit;
	for (int i = 0; i < var->connectedNodes; i++) msg += var->messages[i];
	for (int i = 0; i < var->connectedNodes; i++) var->messages[i] = msg - var->messages[i];
}

void message_check_to_var(struct checkNode * check)
{
		for (int i = 0;  i < check->connectedNodes; i++) check->varNode[i]->messages[check->index[i]] = check->messages[i];
}

void message_var_to_check(struct variableNode * var)
{
	for (int i = 0; i < var->connectedNodes; i++)  var->chNode[i]->messages[var->index[i]] = var->messages[i];
}

int  message_decoded(struct variableNode * var)
{
	int msg = var->incoming_bit; 
	for (int i = 0; i < var->connectedNodes; i++) msg += var->messages[i];
	return msg;
}

LDPC_decoder construct_decoder()
{
	struct variableNode  * var[COLS];
	struct checkNode  * check[ROWS];
	LDPC_decoder dec;
	
	//Allocate memory
	for (int i = 0; i < COLS; i++) var[i] = malloc(sizeof(*var[0]));
	for (int i = 0; i < ROWS; i++) check[i] = malloc(sizeof(*check[0]));
	
	//Fill check nodes:
	int j, k;
	for (int i = 0; i < LEN_PARITY; i++)
	{
		//Just for readability, as both are used 4-5 times
		j = parity[1][i];
		k = parity[0][i];
		
		//connect varNode to Checknode and vice versa and remember index refelctively
		check[j]->varNode[check[j]->connectedNodes] = var[k]; 
		check[j]->index[check[j]->connectedNodes] = var[k]->connectedNodes;
		
		var[k]->chNode[var[k]->connectedNodes] = check[j];
		var[k]->index[var[k]->connectedNodes] =  check[j]->connectedNodes;
		
		//increase number of connected nodes
		check[parity[1][i]]->connectedNodes++;
		var[parity[0][i]]->connectedNodes++;
	}
	for (int i = 0; i < COLS; i++) dec.var[i] = var[i];
	for (int i = 0; i < ROWS; i++) dec.check[i] = check[i];
	return dec;
}

void decode_ldpc(int message[ROWS], int codeword[COLS], LDPC_decoder * dec)
{
	//Step 1: initialize , i.e. set all values in varNodes to zero and copy LLR from message to  var
	//printf("Initializing decoder\n");
	for (int i  = 0 ; i < COLS;  i++) 
	{	
		//printf("(%d)\n", i);
		dec->var[i]->incoming_bit = codeword[i]; //set incoming bit
		for (int j = 0; j < dec->var[i]->connectedNodes; j++) dec->var[i]->messages[j] = 0; //set messages from checknode to varnode
	}
	//printf("Starting Belief Propagation\n");
	//Step 2: belief propagation
	for (int bp = 0; bp < BP_MAX; bp++)
	{
		//Calculate upward messages
		for (int i = 0; i < COLS;  i++) message_out_variable(dec->var[i]);
		
		//Copy messages from varNode to checkNode
		for (int i = 0; i < COLS; i++) message_var_to_check(dec->var[i]);
		
		//Calculate downward messages
		for (int i = 0; i < ROWS; i++) min_sum(dec->check[i]);
		
		//Copy messages from checkNode to varNode
		for (int i = 0; i < ROWS; i++) message_check_to_var(dec->check[i]);
	}
	//Step 3: calculate outgoing message
	for (int i = 0; i < ROWS; i++) message[i] = message_decoded(dec->var[i]);
	
}
