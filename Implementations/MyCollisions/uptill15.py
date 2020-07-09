import numpy as np
import random

def modMatInv(A,p):			 # Finds the inverse of matrix A mod p
	n=len(A)
	A=np.matrix(A)
	adj=np.zeros(shape=(n,n))
	for i in range(0,n):
		for j in range(0,n):
			adj[i][j]=((-1)**(i+j)*int(round(np.linalg.det(minor(A,j,i)))))%p
	return (modInv(int(round(np.linalg.det(A))),p)*adj)%p

def modInv(a,p):					# Finds the inverse of a mod p, if it exists
	for i in range(1,p):
		if (i*a)%p==1:
			return i
	raise ValueError(str(a)+" has no inverse mod "+str(p))

def minor(A,i,j):		# Return matrix A with the ith row and jth column deleted
	A=np.array(A)
	minor=np.zeros(shape=(len(A)-1,len(A)-1))
	p=0
	for s in range(0,len(minor)):
		if p==i:
			p=p+1
		q=0
		for t in range(0,len(minor)):
			if q==j:
				q=q+1
			minor[s][t]=A[p][q]
			q=q+1
		p=p+1
	return minor

def rotateAndAddToMatrix(A, k):
	for i in range(0, 32):
		A[i][(32+i-k)%32] = A[i][(32+i-k)%32]^1

def shiftAndAddToMatrix(A, k):
	for i in range(0, 32):
		if i-k>-1:
			A[i][i-k] = A[i][i-k]^1

def intializeCapSigma(rotate1, rotate2, rotate3):
	matrix = np.array([[0]*32]*32)
	rotateAndAddToMatrix(matrix, rotate1)
	rotateAndAddToMatrix(matrix, rotate2)
	rotateAndAddToMatrix(matrix, rotate3)
	return matrix

def intializeSmallSigma(rotate1, rotate2, shift1):
	matrix = np.array([[0]*32]*32)
	rotateAndAddToMatrix(matrix, rotate1)
	rotateAndAddToMatrix(matrix, rotate2)
	shiftAndAddToMatrix(matrix, shift1)
	return matrix

def arrayToNum(vec):
	return vec.dot(2**np.arange(vec.size))

def numToArray(n):
	return bin_array(n, 32)[::-1]

def bin_array(num, m):
	"""Convert a positive integer num into an m-bit bit vector"""
	return np.array(list(np.binary_repr(num).zfill(m))).astype(np.int64)

def moduloHelper(x):
	return int(x)%2

def applyMatrix(A, x):
	result = np.matmul(x, A)
	moduloInv = np.vectorize(moduloHelper)
	result = moduloInv(result)
	return result

def calculate(sigma, x):
	vec = numToArray(x)
	print(vec)
	vec_new = applyMatrix(sigma, vec)
	vec_new_num = arrayToNum(vec_new)
	print(vec_new)
	return vec_new_num

def getInverse(sigma):
	inv_sig_helper = modMatInv(sigma, 2)
	moduloInv = np.vectorize(moduloHelper)
	inv_sig = moduloInv(inv_sig_helper)
	return inv_sig	

def checkSubset(A, B):
	bad = 0
	for i in range(0, 32):
		if A[i]==1 and B[i]==0:
			j = i-1
			found = False
			while(j>-1):
				if A[j]==0 and B[j]==0:
					return -1
				if A[j]==1 and B[j]==1:
					bad = bad+1
					found = True
					break
				bad = bad+1
				j = j-1
			if found==False:
				return -1
	return bad

def hammingWeight(vec):
	r=0
	for i in vec:
		r = r + i%2
	return r


CAP_SIGMA_0 = intializeCapSigma(2, 13, 22)
CAP_SIGMA_1 = intializeCapSigma(6, 11, 25)
SMALL_SIGMA_0 = intializeSmallSigma(7, 18, 3)
SMALL_SIGMA_1 = intializeSmallSigma(17, 19, 10)

inv_SMALL_SIGMA_1 = getInverse(SMALL_SIGMA_1)

count1 = 0
count2 = 0
count3 = 0
count4 = 0
count5 = 0
trials = 1
l = []
bestTotal = 1000
bestHW = 1000
bestBad = 1000
argBest = -1
prevArgBest = -1
same = 0

# currentE7 = 68157439
# e7Vec = numToArray(currentE7)
# Sig0E7 = np.matmul(e7Vec, CAP_SIGMA_0)
# print(Sig0E7)
# print(e7Vec)
found = 0
printed = []
weight = 1
while weight!=15:
	while True:
		trials = trials+1
		currentE7 = random.randrange(0, 2**32) #random choice
		e7Vec = numToArray(currentE7)
		if hammingWeight(e7Vec)>14:
			continue;
		smallSig1E7 = np.matmul(e7Vec, SMALL_SIGMA_1)
		smallSig1E7 = smallSig1E7%2
		check1 = checkSubset(e7Vec, smallSig1E7)
		curBad = 0
		if check1!=-1:
			curBad = check1
			check2 = checkSubset(smallSig1E7, e7Vec)
			if check2!=-1:
				curBad = curBad + check2
				Sig0E7 = np.matmul(e7Vec, CAP_SIGMA_0)
				Sig0E7 = Sig0E7%2
				check3 = checkSubset(Sig0E7, e7Vec)
				if check3!=-1:
					curBad = curBad + check3
					Sig1E7 = np.matmul(e7Vec, CAP_SIGMA_1)
					Sig1E7 = Sig1E7%2
					check4 = checkSubset(Sig1E7, e7Vec)
					if check4!=-1:
						curBad = curBad + check4
						found = found+1
						totalX = 4*np.sum(e7Vec)
						count5 = count5+1
						if totalX < bestTotal:
							bestTotal = totalX
							bestBad = curBad
							argBest = currentE7
						elif totalX==bestTotal:
							bestTotal = totalX
							bestBad = curBad
							argBest = currentE7
		# if argBest not in printed:
		# 	print(bestTotal),
		# 	print(bestBad),
		# 	print(argBest)
		# 	printed.append(argBest)	
			
		# if trials%1000000==0:
		# 	print(trials)
		if trials%1000000==0:
			if prevArgBest == argBest:
				same = same + 1
			else:
				same = 0
			if bestTotal!=1000:
				print(same),
				print(bestTotal),
				print(bestBad),
				print(argBest)
			else:
				print("None found here")
			if same==20:
				bestTotal = 1000
				bestBad = 1000
				argBest = -1
				same = 0
			prevArgBest = argBest