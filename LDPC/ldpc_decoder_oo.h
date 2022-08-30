#ifndef LDPC_DECODER_OO
#define LDPC_DECODER_OO


typedef struct LDPC_decoder LDPC_decoder;

typedef struct variableNode variableNode;

typedef struct checkNode  checkNode;

struct LDPC_decoder {
	int cLen;
	int vLen;
	variableNode  ** var;
	checkNode ** check;
};

struct variableNode {
	int connectedNodes;
	double incoming_bit;
	int name;
	int * index;
	double * messages; //Represents both the incoming or outgoing messages, doing it this way allows for easy multithreading/cuda support
	checkNode ** chNode;	
};

typedef struct checkNode {
	int connectedNodes;
	int * index;
	int name;
	double * messages; //Represents both the incoming and outgoing messages.
	variableNode ** varNode;
} checkNode;


LDPC_decoder construct_decoder();


//void decode_ldpc_oo(double message[ROWS], double codeword[COLS], LDPC_decoder * dec, char setting, double constant);

#endif
