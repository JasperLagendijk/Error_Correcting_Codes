#ifndef LDPC_DECODER_OO
#define LDPC_DECODER_OO


typedef struct LDPC_decoder LDPC_decoder;

typedef struct variableNode variableNode;

typedef struct checkNode  checkNode;

struct LDPC_decoder {
	variableNode  * var[COLS];
	checkNode * check[ROWS];
};

struct variableNode {
	int connectedNodes;
	double incoming_bit;
	int index[MAX_CHECK_NODE];
	double messages[MAX_CHECK_NODE]; //Represents both the incoming or outgoing messages, doing it this way allows for easy multithreading/cuda support
	checkNode * chNode[MAX_CHECK_NODE];	
};

typedef struct checkNode {
	int connectedNodes;
	int index[MAX_VAR_NODE];
	double messages[MAX_VAR_NODE]; //Represents both the incoming and outgoing messages.
	variableNode * varNode[MAX_VAR_NODE];
} checkNode;


LDPC_decoder construct_decoder();


void decode_ldpc_oo(double message[ROWS], double codeword[COLS], LDPC_decoder * dec, char setting, double constant);

#endif
