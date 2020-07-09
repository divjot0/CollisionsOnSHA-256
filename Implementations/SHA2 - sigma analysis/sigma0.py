import numpy as np
import math
import random
from collections import Counter
import signal
import atexit

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

def printCompleteMatrix(A):
	for i in range(0, 32):
		for j in range(0, 32):
			print(A[i][j]),
		print("")

def rotateAndAddToMatrix(A, k):
	for i in range(0, 32):
		A[i][(i+k)%32] = A[i][(i+k)%32]^1

def shiftAndAddToMatrix(A, k):
	for i in range(0, 32):
		A[i][(i+k)%32] = A[i][(i+k)%32]^1
		if i+k==31:
			break

def intializeSigma(rotate1, rotate2, shift):
	matrix = np.array([[0]*32]*32)
	rotateAndAddToMatrix(matrix, rotate1)
	rotateAndAddToMatrix(matrix, rotate2)
	shiftAndAddToMatrix(matrix, shift)
	return matrix

def moduloHelper(x):
	return int(x)%2

# as in code theory
def applyMatrix(A, x):
	result = np.matmul(x, A)
	moduloInv = np.vectorize(moduloHelper)
	result = moduloInv(result)
	return result

def arrayToNum(vec):
	return vec.dot(2**np.arange(vec.size)[::-1])

def numToArray(n):
	return bin_array(n, 32)

def bin_array(num, m):
	"""Convert a positive integer num into an m-bit bit vector"""
	return np.array(list(np.binary_repr(num).zfill(m))).astype(np.int64)

#input is matrix and number output is number
def calculate(sigma, x):
	vec = numToArray(x)
	vec_new = applyMatrix(sigma, vec)
	vec_new_num = arrayToNum(vec_new)
	return vec_new_num

#returns the [deltaSig, top deltaX, sum of top three]
def deltaXGivenDeltaSig(sigma, inv_sigma, delta_sig, repititions):
	SIGMA_DELTA = delta_sig
	distribution = {}
	i=0
	print("Strating to check for delta_sig = ", delta_sig )
	while i!=repititions:
		x = random.randint(0, 2**32-1)
		sig_x = calculate(sigma, x)
		temp = (SIGMA_DELTA + sig_x)%(2**32)
		x_prime = calculate(inv_sigma, temp)
		delta_x = (2**32 + x_prime - x)%(2**32)
		if delta_x not in distribution:
			distribution[delta_x] = 0
		distribution[delta_x] = distribution[delta_x]+1
		i=i+1
		if i%10**4==0:
			print(i)

	k = Counter(distribution)
	high = k.most_common(80)
	print(high)
	return [SIGMA_DELTA, high[0][0], high[0][1]+high[1][1]+high[2][1], high[0][1], high[1][1], high[2][1]]

def deltaSigGivenDeltaX(sigma, inv_sigma, delta_x, repititions):
	X_DELTA = delta_x
	distribution = {}
	i=0
	print("Strating to check for delta_x = ", delta_x)
	while i!=repititions:
		x = random.randint(0, 2**32-1)
		sig_x = calculate(sigma, x)
		x_prime = (x + X_DELTA)%(2**32)
		sig_x_prime = calculate(sigma, x_prime)
		delta_sigma = (2**32 + sig_x_prime - sig_x)%(2**32)
		if delta_sigma not in distribution:
			distribution[delta_sigma] = 0
		distribution[delta_sigma] = distribution[delta_sigma]+1
		i=i+1
		print(i)

	maximum=0
	arg_max=0
	for delta_sigma in distribution:
		if(distribution[delta_sigma]>1):
			# print(delta_sigma, distribution[delta_sigma])
			if maximum<distribution[delta_sigma]:
				maximum = distribution[delta_sigma]
				arg_max = delta_sigma
	print(arg_max, maximum)

def Sort(sub_li):
	sub_li.sort(key = lambda x: x[2]) 
	return sub_li 

def handle_exit():
	global randomTrialsSig0
	randomTrialsSig0 = Sort(randomTrialsSig0)
	with open('./randomTrialsSigma0.txt', 'w+') as randomTrialsInfo:
		for item in randomTrialsSig0:
			randomTrialsInfo.write("%s\n" % item)
	print("Saved sigma_0")
	# same for sig1
	global randomTrialsSig1
	randomTrialsSig1 = Sort(randomTrialsSig1)
	with open('./randomTrialsSigma1.txt', 'w+') as randomTrialsInfo:
		for item in randomTrialsSig1:
			randomTrialsInfo.write("%s\n" % item)
	print("Saved sigma_1")
	print("Good Bye")
    # generate_output()

## main function starts here
atexit.register(handle_exit)
signal.signal(signal.SIGTERM, handle_exit)
signal.signal(signal.SIGINT, handle_exit)

#initialization of sigma and inverse functions
sigma_0 = intializeSigma(7, 18, 3)
sigma_1 = intializeSigma(17, 19, 10)
inv_sigma_0_helper = modMatInv(sigma_0, 2)
inv_sigma_1_helper = modMatInv(sigma_1, 2)
moduloInv = np.vectorize(moduloHelper)
inv_sigma_0 = moduloInv(inv_sigma_0_helper)
inv_sigma_1 = moduloInv(inv_sigma_1_helper)

#intializing dictionaries
randomTrialsSig0 = []
randomTrialsSig1 = []

#random trials to find a deltaSig that causes a very unbalanced distribution over deltaX

# i=0
# while i!=10000:
# 	temp_delta_sig = np.random.randint(1, 2**32-1)
# 	res = deltaXGivenDeltaSig(sigma_0, inv_sigma_0, temp_delta_sig, 10**5)
# 	randomTrialsSig0.append(res)
# 	i=i+1
# 	print(i)

# randomTrialsSig0 = Sort(randomTrialsSig0)

# with open('./randomTrialsSigma0.txt', 'w+') as randomTrialsInfo:
# 	for item in randomTrialsSig0:
# 		randomTrialsInfo.write("%s\n" % item)


# same for sig1
temp_delta_sig = 3287806720
res = deltaXGivenDeltaSig(sigma_1, inv_sigma_1, temp_delta_sig, 10**5)
print(res)

# look at the structure of top few