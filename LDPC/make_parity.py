from scipy.io import loadmat
from scipy.sparse import csr_matrix
import numpy as np
def main():
	
	
	#First load and make sparse parity matrix
	annots = loadmat("parity.mat")
	parity = np.array([[element for element in upperElement] for upperElement in annots['Hldpc']])
	
	
	#Second: find size of n, m and g
	n = parity.shape[1]
	m = parity.shape[0]
	for i, par in enumerate(parity[:, -1]):
		if (par == 1):
			g = m-i-1
			break
	
	#Third, generate A - E +T
	
	A = parity[0:m-g, 0:n-m]
	B = parity[0:m-g, (n-m):(g+n-m)]
	T = parity[0:m-g, (g+n-m):(n)]
	
	C = parity[m-g:n, 0:n-m]
	D = parity[m-g:n, (n-m):(g+n-m)]
	E = parity[m-g:n, (g+n-m):(n)]
	
	#Create encoder matrix
	T_inv = np.linalg.inv(T)
	
	phi = -np.dot(np.dot(E,T_inv), B) + D
	
	if (np.linalg.det(phi) == 0):
		print("Matrix phi is singular, please use a different parity matrix.")
		return
	
	P1_encoder = -np.dot(np.linalg.inv(phi), np.dot(np.dot(E, T_inv),A)+C)
	P1_encoder_sprs = csr_matrix(P1_encoder)
	A_sprs = csr_matrix(A)
	B_sprs = csr_matrix(B)
	T_inv_sprs = csr_matrix(T_inv)
	parity_sprs = csr_matrix(parity)
	
		
	
	# Create parity.c file
	fc = open("parity.c", "w")
	fc.writelines(["#include <stdio.h>\n"])
	fc.writelines(["#include \"parity.h\" \n\n\n"])
		# parity matrix
	
	
	
	L1 = []
	L2 = []
	q = []
	q.append(0)
	for i in range(parity.shape[0]):
		for index in parity_sprs[i].indices:
			L1.append(str(index))
			L1.append(", ")
			L2.append(str(i))
			L2.append(", ")
			q[0] += 1
			
	L1.pop(-1)
	L2.pop(-1)
	fc.writelines(["unsigned int parity[2][", str(q[0]),"] = { \n \t\t\t\t\t\t\t\t\t{ "])
	fc.writelines(L1)
	fc.writelines([" },\n \t\t\t\t\t\t\t\t\t{ "])
	fc.writelines(L2)
	fc.writelines(["}\n};\n"])
	
		# A matrix
	
	L1 = []
	L2 = []
	q.append(0)
	for i in range(A.shape[0]):
		for index in A_sprs[i].indices:
			L1.append(str(index))
			L1.append(", ")
			L2.append(str(i))
			L2.append(", ")
			q[1] += 1
	fc.writelines(["unsigned bit_size A[2][", str(q[1]),"] = { \n \t\t\t\t\t\t\t\t\t{ "])
	L1.pop(-1)
	L2.pop(-1)
	fc.writelines(L1)
	fc.writelines([" },\n \t\t\t\t\t\t\t\t\t{ "])
	fc.writelines(L2)
	fc.writelines(["}\n};\n"])
	
	L4 = []
	for i  in range(int(L2[-1])):
		L4.append(str(int(L2.index(str(i))/2)))
		L4.append(", ")
	L4.append(str(q[1]))
	
	fc.writelines(["\nunsigned bit_size A_index[", str(m-g+1) , "] =\t{ "])
	fc.writelines(L4)
	fc.writelines(["};\n\n"])
		# B matrix
	
	L1 = []
	L2 = []
	q.append(0)
	for i in range(B.shape[0]):
		for index in B_sprs[i].indices:
			L1.append(str(index))
			L1.append(", ")
			L2.append(str(i))
			L2.append(", ")
			q[2] += 1	
	L1.pop(-1)
	L2.pop(-1)
	fc.writelines(["unsigned bit_size B[2][", str(q[2]),"] = { \n \t\t\t\t\t\t\t\t\t{ "])
	fc.writelines(L1)
	fc.writelines([" },\n \t\t\t\t\t\t\t\t\t{ "])
	fc.writelines(L2)
	fc.writelines(["}\n};\n"])
	
	
		# T_inv matrix
	
	L1 = []
	L2 = []
	L3 = []
	L4 = []
	q.append(0)
	for i in range(T_inv.shape[0]):
		for j, index in enumerate(T_inv_sprs[i].indices):
			L1.append(str(index))
			L1.append(", ")
			L2.append(str(i))
			L2.append(", ")
			L3.append(str(int(T_inv_sprs[i, index])))
			L3.append(", ")
			q[3] += 1
	L1.pop(-1)
	L2.pop(-1)
	fc.writelines(["bit_size T_inv[3][", str(q[3]),"] = { \n \t\t\t\t\t\t\t\t\t{ "])
	fc.writelines(L1)
	fc.writelines([" },\n \t\t\t\t\t\t\t\t\t{ "])
	fc.writelines(L2)
	fc.writelines([" },\n \t\t\t\t\t\t\t\t\t{ "])
	fc.writelines(L3)
	fc.writelines(["}\n};\n"])
	
	for i  in range(int(L2[-1])):
		L4.append(str(int(L2.index(str(i))/2)))
		L4.append(", ")
	L4.append(str(q[3]))
	
	fc.writelines(["\nunsigned bit_size T_inv_index[", str(m-g+1) , "] =\t{ "])
	fc.writelines(L4)
	fc.writelines(["};\n\n"])
	
	
	
	L1 = []
	L2 = []
	L3 = []
	L4 = []
	q.append(0)
	for i in range(P1_encoder.shape[0]):
		for j, index in enumerate(P1_encoder_sprs[i].indices):
			L1.append(str(index))
			L1.append(", ")
			L2.append(str(i))
			L2.append(", ")	
			L3.append(str(int(P1_encoder_sprs[i, index])))
			L3.append(", ")
			q[4] += 1
	L1.pop(-1)
	L2.pop(-1)
	L3.pop(-1)
	fc.writelines(["bit_size p1_encoder[3][", str((q[4])),"] = { \n \t\t\t\t\t\t\t\t\t{ "])
	fc.writelines(L1)
	fc.writelines([" },\n \t\t\t\t\t\t\t\t\t{ "])
	fc.writelines(L2)
	fc.writelines([" },\n \t\t\t\t\t\t\t\t\t{ "])
	fc.writelines(L3)
	fc.writelines(["}\n};\n"])
	
	for i  in range(int(L2[-1])):
		L4.append(str(int(L2.index(str(i))/2)))
		L4.append(", ")
	L4.append(str(q[4]))
	
	fc.writelines(["\nunsigned bit_size p1_encoder_index[", str(g+1) , "] =\t{ "])
	fc.writelines(L4)
	fc.writelines(["};\n\n"])
	
	
		
	fc.close()
	
	fh = open("parity.h", "w")
	
	fh.writelines(["#ifndef PARITY\n#define PARITY \n\n"])
	fh.writelines(["#define ROWS ", str(m), "\n"])
	fh.writelines(["#define COLS ", str(n), "\n"])
	fh.writelines(["#define GAP ", str(g), "\n"])
	fh.writelines(["#define MAX_VAR_NODE ", str(np.max(np.sum(parity, 1))), "\n"])
	fh.writelines(["#define MAX_CHECK_NODE ", str(np.max(np.sum(parity, 0))), "\n\n"])
	
	
	fh.writelines(["#define LEN_PARITY ", str(q[0]), "\n"])
	fh.writelines(["#define LEN_A ", str(q[1]), "\n"])
	fh.writelines(["#define LEN_B ", str(q[2]), "\n"])
	fh.writelines(["#define LEN_T_INV ", str(q[3]), "\n"])
	fh.writelines(["#define LEN_P1_ENC ", str(q[4]), "\n\n"])
	fh.writelines(["#define bit_size short int \n"])
	
	
	fh.writelines(["\textern unsigned int parity[2][", str(q[0]), "];\n"])
	fh.writelines(["\textern unsigned bit_size A[2][", str(q[1]), "];\n"])
	fh.writelines(["\textern unsigned bit_size B[2][", str(q[2]), "];\n"])
	fh.writelines(["\textern bit_size T_inv[3][", str(q[3]), "];\n"])
	fh.writelines(["\textern bit_size p1_encoder[3][", str(q[4]), "];\n"])
	
	fh.writelines(["\textern unsigned bit_size A_index[", str(m-g+1), "];\n"])
	fh.writelines(["\textern unsigned bit_size T_inv_index[",str(m-g+1) , "];\n"])
	fh.writelines(["\textern unsigned bit_size p1_encoder_index[",str(g+1) , "];\n"])
	
	
	fh.writelines(["\n#endif"])
	
	fh.close()
	
	#print(P1_encoder_sprs)
	
	
if __name__ == "__main__":
	main()
