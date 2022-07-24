int * RA_Encode(int message[], int b, int K)
{
		//Step 0. preallocate code word
		int * codeword = malloc(sizeof(int)*b*K);
		
		//Step 1. Repeat each message bit c/b=K times
		
		for (int i = 0; i < b; i++)
		{
				for (int j = 0; j < K; j++) codeword[(K*i)+j] = message[i];			
		}
		//Step 2. Interleaver
		
		//Step 3.  Accumulator
		for (int i = 1; i < (b*K); i++) codeword[i] += codeword[i-1];
		
		
		return codeword;
}
