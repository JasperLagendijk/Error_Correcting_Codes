#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "parity.h"
#include "ldpc_decoder_oo.h"

#define BP_MAX 20

LDPC_decoder construct_decoder(int ** sparse, int length)
{
	//Step 0. Find minima and maxima of sparse, initialize checkNodes and varNodes
	int cols = 0;
	int rows = 0;
	for (int i = 0; i < length; i++)
	{
		if (sparse[0][i] > cols) cols = sparse[0][i];
		if (sparse[1][i] > rows) rows = sparse[1][i];
	}
	cols++;
	rows++;
	
	int checkConnections[cols] = {0};
	int varConnections[rows] = {0};  
	for (int i = 0; i < length; i++)
	{
		checkConnections[sparse[0][i]]++;
		varConnections[sparse[1][i]]++;
	}
	
		//Initialize and malloc structs
	checkNode * check[rows];
	variableNode * var[cols];
	
	for (int i = 0; i < rows; i++)
	{
		check[i]->index = (int *) malloc(sizeof(int)*checkConnections[i]);
		check[i]->messages = (double *) malloc(sizeof(double)*checkConnections[i]);
		check[i]->varNode = (variableNode *) malloc(sizeof(variableNode*)*checkConnections[i]);
		check[i] = (checkNode *) malloc(sizeof(checkNode));
	}	
	
	for (int i = 0; i < cols; i++)
	{
		var[i]->index = (int *) malloc(sizeof(int)*varConnections[i]);
		var[i]->messages = (double *) malloc(sizeof(double)*varConnections[i]);
		var[i]->chNode = (checkNode *) malloc(sizeof(checkNode*)*checkConnections[i]);
		var[i] = (variableNode *) malloc(sizeof(variableNode));
	}
	//Step 1. Loop through sparse matrix and
}
