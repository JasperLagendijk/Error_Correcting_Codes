#include <stdio.h>
#include <stdlib.h>
#include "parity.h"
#include "ldpc.h"

#define BP_MAX 20



int sign(int x) {
    return (x > 0) - (x < 0);
}

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
	//for (int i = 0; i < check->connectedNodes; i++) if ( i != j && abs(check->messages[i]) < min2) min2 = abs(check->messages[i]);
	//Step 2: for each connected variable node
	for (int i = 0; i < check->connectedNodes; i++) check->messages[i] = min*sig*sign(check->messages[i])/2;
	/*{
		if (i == j) check->varNode[i]->messages[check->index[i]] = min2*-1*sig;
		else check->varNode[i]->messages[check->index[i]] = min*-1*sig;
		
	}*/
	
}

void message_out_variable(struct variableNode * var) //Calculates variable node value and places them in messages
{
	int msg = var->incoming_bit;
	for (int i = 0; i < var->connectedNodes; i++) msg += var->messages[i];
	//for (int i = 0; i < var->connectedNodes; i++) var->chNode[i]->messages[var->index[i]] = msg - var->messages[i];
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

void encode_ldpc(int msg[ROWS], int codeword[COLS])
{
	// Find p1
	// p1 = p1_encode * msg
	int p1[GAP] = {0};
	int p2[ROWS-GAP] = {0};
	
	for (int j = 0; j < LEN_P1_ENC; j++) p1[p1_encoder[1][j]] += p1_encoder[2][j]*msg[p1_encoder[0][j]];
	for (int i = 0; i < GAP; i++) p1[i] = p1[i] % 2;
	
	// Find p2
	int As[ROWS-GAP] = {0};
	int Bp[ROWS-GAP] = {0};
	
	for (int j = 0; j < LEN_A; j++) As[A[1][j]] += msg[A[0][j]];
	for (int j = 0; j < LEN_B; j++) Bp[B[1][j]] += p1[B[0][j]];
	for (int j = 0; j < LEN_T_INV; j++) p2[T_inv[1][j]] += T_inv[2][j]*(As[T_inv[0][j]]+Bp[T_inv[0][j]]);
	
	for (int i = 0; i < ROWS-GAP; i++) p2[i] = abs(p2[i]) % 2;
	 //Codeword consists of 3 parts :[msg, p1, p2]
	for (int i = 0; i < ROWS; i++) codeword[i] = msg[i];
	for (int i = ROWS; i < ROWS+GAP; i++) codeword[i] = p1[i-ROWS];
	for (int i = ROWS+GAP; i < COLS; i++) codeword[i] = p2[i-ROWS-GAP];
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




/*int main() 
{
	/*
	int msg[ROWS] = {};
	int message[COLS] = {};
	int codeword[COLS];
	double BER = 0;
	LDPC_decoder dec = construct_decoder();
	//for (int i = 0; i < ROWS; i++) printf("%d ", msg[i]);
	//printf("\n");
	srand (2);
	
		
	
	
	
	for (int j = 0; j < 100000; j++)
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
	printf("\nMessage: \n");
	/*for (int i = 0; i < ROWS; i++) printf("%d ", codeword[i]);
	printf("\nParity: \n");
	for (int i = ROWS; i < COLS; i++) printf("%d ", codeword[i]);
	
	return 0;
}*/
