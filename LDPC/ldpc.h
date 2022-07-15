#ifndef LDPC
#define LDPC


typedef struct LDPC_decoder LDPC_decoder;

typedef struct variableNode variableNode;

typedef struct checkNode  checkNode;

struct LDPC_decoder {
	variableNode  * var[COLS];
	checkNode * check[ROWS];
};

struct variableNode {
	int connectedNodes;
	int incoming_bit;
	int index[MAX_CHECK_NODE];
	int messages[MAX_CHECK_NODE]; //Represents both the incoming or outgoing messages, doing it this way allows for easy multithreading/cuda support
	checkNode * chNode[MAX_CHECK_NODE];	
};

typedef struct checkNode {
	int connectedNodes;
	int index[MAX_VAR_NODE];
	int messages[MAX_VAR_NODE]; //Represents both the incoming and outgoing messages.
	variableNode * varNode[MAX_VAR_NODE];
} checkNode;


LDPC_decoder construct_decoder();

void encode_ldpc(int msg[ROWS], int codeword[COLS]);

void decode_ldpc(int message[ROWS], int codeword[COLS], LDPC_decoder * dec);

#endif
