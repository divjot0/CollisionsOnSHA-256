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

def getRandomSubVector(e8Vec):
	for i in range(0, 1000):
		e7 = random.randrange(0, 2**32)
		e7Vec = numToArray(e7)
		if checkSubset(e7Vec, e8Vec):
			return e7Vec
	return np.array([])

# def ifSubset(A, B):
# 	ifDiff = np.array([0]*32)
# 	for i in range(0, 32):
# 		if B[i]==1:
# 			if A[i]==1:
# 				ifDiff[i]=1;
# 			elif A[i]==0:
# 				if i==31:
# 					return ifDiff
# 				if B[i+1]==1:
# 					continue
# 				elif A[]

def subsetExtra(A, B):
	extra = np.array([0]*32)
	l = False
	for i in range(0, 32):
		if A[i]==0 and B[i]==0:
			l = False
		elif ((A[i]==1 and B[i]==0) or (A[i]==0 and B[i]==1)):
			if l==True:
				continue
			else:
				extra[i]=1
		else:
			l==True
	return extra


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
argBest7 = -1
argBest8 = -1
prevArgBest7 = -1
same = 0

# currentE7 = 68157439
# e7Vec = numToArray(currentE7)
# Sig0E7 = np.matmul(e7Vec, CAP_SIGMA_0)
# print(Sig0E7)
# print(e7Vec)
found = 0
printed = []
while trials!=2**32:
	trials = trials+1
	currentE8 = random.randrange(0, 2**32)
	e8Vec = numToArray(currentE8)
	e7Vec = getRandomSubVector(e8Vec)
	e7UnionE8 = ((e8Vec+e7Vec)!=0).astype(int)
	smallSig1E7 = np.matmul(e7Vec, SMALL_SIGMA_1)%2
	check1 = checkSubset(smallSig1E7, e7UnionE8)
	if check1!=-1:
		curBad = check1
		capSig1E8 = np.matmul(e8Vec, CAP_SIGMA_1)%2
		check2 = checkSubset(capSig1E8, e7UnionE8)
		if check2!=-1:
			curBad = curBad + check2
			capSig0E7 = np.matmul(e7Vec, CAP_SIGMA_0)%2
			maj7 = subsetExtra(capSig0E7, e8Vec)
			check3 = checkSubset(maj7, e7Vec)
			if check3!=-1:
				curBad = curBad + check3
				capSig1E7 = np.matmul(e7Vec, CAP_SIGMA_1)%2
				xor = capSig1E7^e8Vec
				check4 = checkSubset(xor, e7Vec)
				if check4!=-1:
					curBad = curBad + check4
					found = found+1
					totalX = 3*np.sum(e7Vec) + np.sum(e8Vec)
					count5 = count5+1
					currentE7 = arrayToNum(e7Vec)
					currentE8 = arrayToNum(e8Vec)
					if totalX < bestTotal:
						bestTotal = totalX
						bestBad = curBad
						argBest7 = currentE7
						argBest8 = currentE8
					elif totalX==bestTotal:
						bestTotal = totalX
						bestBad = curBad
						argBest7 = currentE7
						argBest8 = currentE8

	# if argBest not in printed:
	# 	print(bestTotal),
	# 	print(bestBad),
	# 	print(argBest)
	# 	printed.append(argBest)	
		
	# if trials%1000000==0:
	# 	print(trials)
	if trials%1000000==0:
		# print("found = "),
		# print(found)
		if prevArgBest7 == argBest7:
			same = same + 1
		else:
			same = 0
		if bestTotal!=1000:
			print(same),
			print(bestTotal),
			print(bestBad),
			print(argBest7),
			print(argBest8)
		else:
			print("None found here")
		if same==20:
			bestTotal = 1000
			bestBad = 1000
			argBest7 = -1
			argBest8 = -1
			same = 0
		prevArgBest7 = argBest7