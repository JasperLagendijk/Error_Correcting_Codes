#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parity.h"

struct variableNode;
struct checkNode;

struct decoder {
	struct variableNode * var[COLS];
	struct checkNode * check[ROWS];
};

struct variableNode {
	int connectedNodes;
	int index[MAX_CHECK_NODE];
	int messages[MAX_CHECK_NODE];
	struct checkNode * chNode[MAX_CHECK_NODE];	
};

struct checkNode {
	int connectedNodes;
	int index[MAX_CHECK_NODE];
	int messages[MAX_VAR_NODE];
	struct variableNode * varNode[MAX_VAR_NODE];
};

int sign(int x) {
    return (x > 0) - (x < 0);
}

void min_sum(struct checkNode * check)
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
			sig *= sign(check->messages[i]);
			if (abs(check->messages[i]) < min) 
			{
				min = abs(check->messages[i]);
				j = i;
			}
	}
	for (int i = 0; i < check->connectedNodes; i++) if ( i != j && abs(check->messages[i]) < min2) min2 = abs(check->messages[i]);
	//Step 2: for each connected variable node
	for (int i = 0; i < check->connectedNodes; i++)
	{
		if (i == j) check->varNode[i]->messages[check->index[i]] = min2*-1*sig;
		else check->varNode[i]->messages[check->index[i]] = min*-1*sig;
		
	}
	
}



void encode_ldpc(unsigned int msg[ROWS], unsigned int codeword[COLS])
{
	// Find p1
	// p1 = p1_encode * msg
	unsigned int p1[GAP] = {0};
	unsigned int p2[ROWS-GAP] = {0};
	for (int i = 0; i < GAP; i++)
	{
		for (int k = 0; k < ROWS; k++)
		{
			for (int j = 0; j < LEN_P1_ENC; j++)
			{
					if (p1_encoder[1][j] == i && p1_encoder[0][j] == k)
					{	
						
						p1[i] += p1_encoder[2][j]*msg[k];
						break;
					}
					else if(p1_encoder[1][j] > i) break; //As p1_encoder is sorted, there are no more relevant values there
			}
		}
		p1[i] = p1[i] % 2;
	}
	
	// Find p2
	int As[ROWS-GAP] = {0};
	int Bp[ROWS-GAP] = {0};
	
	
	for (int i = 0; i < ROWS-GAP; i++) 
	{
		//As = A*s
		for (int k = 0; k < ROWS; k++)
		{
			for (int j = 0; j < LEN_A; j++)
			{
					if (A[1][j] == i && A[0][j] == k)
					{	
						
						As[i] += msg[k];
						break;
					}
					else if(A[1][j] > i) break; //As A is sorted, there are no more relevant values there
			}
		}
		//Bp = B*p1
		for (int k = 0; k < ROWS; k++)
		{
			for (int j = 0; j < LEN_B; j++)
			{
				if (B[1][j] == i && B[0][j] == k)
				{	
					
					Bp[i] += p1[k];
					break;
				}
				else if(B[1][j] > i) break; //As B is sorted, there are no more relevant values there
			}
		}
		//p2 = T_inv * (As+Bp)
		for (int k = 0; k < ROWS-GAP; k++)
		{
			for (int j = 0; j < LEN_T_INV; j++)
			{
				if (T_inv[1][j] == i && T_inv[0][j] == k)
				{	
					
					p2[i] += T_inv[2][j]*(As[k]+Bp[k]);
					break;
				}
				else if(T_inv[1][j] > i) break; //As T_inv is sorted, there are no more relevant values there
			}
		}
	
	p2[i] = p2[i] % 2;
	} //Codeword consists of 3 parts :[msg, p1, p2]
	for (int i = 0; i < ROWS; i++) codeword[i] = msg[i];
	for (int i = ROWS; i < ROWS+GAP; i++) codeword[i] = p1[i-ROWS];
	for (int i = ROWS+GAP; i < COLS; i++) codeword[i] = p2[i-ROWS-GAP];
}


struct decoder construct_decoder()
{
	struct variableNode var[COLS];
	struct checkNode check[ROWS];
	struct decoder dec;
	
	//Fill check nodes:
	int j, k;
	for (int i = 0; i < LEN_PARITY; i++)
	{
		//Just for readability, as both are used 4-5 times
		j = parity[1][i];
		k = parity[0][i];
		
		//connect varNode to Checknode and vice versa and remember index refelctively
		check[j].varNode[check[j].connectedNodes] = &var[k]; 
		check[j].index[check[j].connectedNodes] = var[k].connectedNodes;
		
		var[k].chNode[var[k].connectedNodes] = &check[j];
		var[k].index[var[k].connectedNodes] =  check[j].connectedNodes;
		
		//increase number of connected nodes
		check[parity[1][i]].connectedNodes++;
		var[parity[0][i]].connectedNodes++;
	}
	for (int i = 0; i < COLS; i++) dec.var[i] = &var[i];
	for (int i = 0; i < ROWS; i++) dec.check[i] = &check[i];
	return dec;
}

void decode_ldpc(int message, int codeword)
{

}




int main() 
{
	//int msg[ROWS] = {0};
	//msg[0] = 1;
	//int codeword[COLS];
	
	//encode_ldpc(msg, codeword);
	struct decoder dec = construct_decoder();
	//for (int i = 0; i < COLS; i++) printf("%d\n", codeword[i]);
	
	return 0;
}
