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
		A[i][(i-k)%32] = A[i][(i-k)%32]^1

def shiftAndAddToMatrix(A, k):
	for i in range(0, 32):
		A[i][(i+k)%32] = A[i][(i+k)%32]^1
		if i+k==31:
			break

def intializeSigma(rotate1, rotate2, shift):
	matrix = np.array([[0]*32]*32)
	rotateAndAddToMatrix(matrix, rotate1)
	rotateAndAddToMatrix(matrix, rotate2)
	rotateAndAddToMatrix(matrix, shift)
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

#returns the [dictionary, good deltaXs]
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
		# if i%10**4==0:
		# 	print(i)

	k = Counter(distribution)
	high = k.most_common(80)
	good_delta_xs = []
	THRESHOLD = 100
	for delta_x in high:
		if delta_x[1]>THRESHOLD:
			good_delta_xs.append([delta_x[0], delta_x[1]])
	# print(high)
	# print(good_delta_xs)
	return good_delta_xs

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

def sortByHW(sub_li):
	sub_li.sort(key = lambda x: sum(x)) 
	return sub_li

# x such that sigma(x+delta_x) - sigma(x) = delta_sigma, fillEvidences returns a set of x's correspoinding to good_delta_xs
def fillEvidences(sigma, inv_sigma, delta_sigma, good_delta_xs, repititions):
	evidences = []
	for i in range(0, len(good_delta_xs)):
		evidences.append([])
	
	SIGMA_DELTA = delta_sigma
	presence = {}
	for i in range(0, len(good_delta_xs)):
		presence[good_delta_xs[i][0]] = i

	i=0
	while i!=repititions:
		x = random.randint(0, 2**32-1)
		sig_x = calculate(sigma, x)
		temp = (SIGMA_DELTA + sig_x)%(2**32)
		x_prime = calculate(inv_sigma, temp)
		delta_x = (2**32 + x_prime - x)%(2**32)
		if delta_x in presence:
			evidences[presence[delta_x]].append(x)
		i=i+1
	return evidences


#initialization of sigma and inverse functions
sigma_0 = intializeSigma(7, 18, 3)
sigma_1 = intializeSigma(6, 11, 25)
inv_sigma_0_helper = modMatInv(sigma_0, 2)
inv_sigma_1_helper = modMatInv(sigma_1, 2)
moduloInv = np.vectorize(moduloHelper)
inv_sigma_0 = moduloInv(inv_sigma_0_helper)
inv_sigma_1 = moduloInv(inv_sigma_1_helper)

t = np.array([0]*32)
# t[26]=1
t[28]=1
print(np.matmul(t, inv_sigma_1))

# Good_delta_sig1 = [2234448640, 3287806720, 2145390210, 1585445296, 2147655682] 
# Good_delta_sig0 = [2030020544,]
# DELTASIG = 2145390210
# print(numToArray(DELTASIG))

# l1 = [2684094658, 2684618942, 2684090302, 2684094530, 2684614590]
# l2 = [1610872770, 1610352830, 1610872642, 1610877118, 1610872766, 1610352706, 1610876994]

# good_delta_xs = deltaXGivenDeltaSig(sigma_1, inv_sigma_1, DELTASIG, 10**5)
# good_delta_xs.sort()
# print(good_delta_xs)
# good_delta_xs_in_bits = []
# good_delta_x_values = []
# for i in range(0, len(good_delta_xs)):
# 	good_delta_xs_in_bits.append(bin_array(good_delta_xs[i][0], 32))
# 	good_delta_x_values.append(good_delta_xs[i][0])

# good_delta_x_values.sort()
# print(good_delta_x_values)
# print("These are good_delta_xs in bits ")
# for good in good_delta_xs_in_bits:
# 	print(good)

# evidences = fillEvidences(sigma_1, inv_sigma_1, DELTASIG, good_delta_xs, 10**5)

# evidences_in_bits = []
# for i in range(0, len(evidences)):
# 	temp = []
# 	for evidenceForX in evidences[i]:
# 		temp.append([bin_array(evidenceForX, 32), bin_array((evidenceForX+good_delta_xs[i][0])%(2**32), 32)])
# 	evidences_in_bits.append(temp)

# for i in range(0, len(good_delta_xs)):
# 	print("Evidences for deltaSig="),
# 	print(good_delta_xs[i][0]),
# 	print(" "),
# 	print(good_delta_xs[i][1])
# 	print(good_delta_xs_in_bits[i])
# 	print(" ")
# 	for j in range(0, 5):
# 		print(evidences_in_bits[i][j][0])
# 		print(evidences_in_bits[i][j][1])
# 		print(" ")
# 	print(" ")
