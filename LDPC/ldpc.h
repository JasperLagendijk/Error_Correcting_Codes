#ifndef LDPC_ENCODER
#define LDPC_ENCODER

struct decoder {
	struct variableNode  * var[COLS];
	struct checkNode * check[ROWS];
};

struct variableNode {
	int connectedNodes;
	int incoming_bit;
	int index[MAX_CHECK_NODE];
	int messages[MAX_CHECK_NODE]; //Represents both the incoming or outgoing messages, doing it this way allows for easy multithreading/cuda support
	struct checkNode * chNode[MAX_CHECK_NODE];	
};

struct checkNode {
	int connectedNodes;
	int index[MAX_VAR_NODE];
	int messages[MAX_VAR_NODE]; //Represents both the incoming and outgoing messages.
	struct variableNode * varNode[MAX_VAR_NODE];
};


#endif
