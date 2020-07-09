# this program is to do further analysis and see how the components of sigma functions work

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
	THRESHOLD = 20
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
		# print(i)

	k = Counter(distribution)
	high = k.most_common(80)
	good_delta_sigs = []
	THRESHOLD = 20
	for delta_sig in high:
		if delta_sig[1]>THRESHOLD:
			good_delta_sigs.append([delta_sig[0], delta_sig[1]])
	# print(high)
	# print(good_delta_xs)
	return good_delta_sigs

def sortByHW(sub_li):
	sub_li.sort(key = lambda x: sum(x)) 
	return sub_li

def convertToModulo(matrix):
	for i in range(0, len(matrix)):
		for j in range(0, len(matrix[0])):
			matrix[i][j] = matrix[i][j]%2
	return

# this function gives the number of chunks we have in x(a number), helpful for calculating the least hamming weight
def countChunks(x):
	x_arr = numToArray(x)
	chunkRunning = False
	totalChunks = 0
	i=0
	while i<len(x_arr):
		totalChunks = totalChunks+1
		lengthOfChunk = 0
		while i<len(x_arr) and (x_arr[i]==x_arr[i-1] or lengthOfChunk==0 or lengthOfChunk==1):
			i=i+1
			lengthOfChunk = lengthOfChunk+1
		lengthOfChunk = 0
	return totalChunks-1

def Sort(sub_li):
	sub_li.sort(key = lambda x: -x[1]) 
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
sigma_1 = intializeSigma(17, 19, 10)
inv_sigma_0_helper = modMatInv(sigma_0, 2)
inv_sigma_1_helper = modMatInv(sigma_1, 2)
moduloInv = np.vectorize(moduloHelper)
inv_sigma_0 = moduloInv(inv_sigma_0_helper)
inv_sigma_1 = moduloInv(inv_sigma_1_helper)

# sig1sig0sig0 = np.matmul(sigma_1, np.matmul(sigma_0, sigma_0))
# convertToModulo(sig1sig0sig0)

# l = modMatInv(sig1sig0sig0, 2)

# for i in sig1sig0sig0:
# 	print(i)
# print("Printing eigen vectors now")
# r = np.linalg.eig(sig1sig0sig0)
# for i in r[1]:
# 	print(i)

# starting_number = np.array([0]*32)
# # t=0
# # starting_number[t+0] = 1
# # starting_number[(t+11)%32] = 1
# # starting_number[(t+28)%32] = 1
# t=0
# starting_number[(t+7)%32] = 1
# starting_number[(t+18)%32] = 1
# starting_number[(t+3)%32] = 1
# t=1
# starting_number[(t+7)%32] = 1
# starting_number[(t+18)%32] = 1
# starting_number[(t+3)%32] = 1
# t=2
# starting_number[(t+7)%32] = 1
# starting_number[(t+18)%32] = 1
# starting_number[(t+3)%32] = 1

# starting_number[13] = 0
###############
# test = np.array([1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,0,0,1,0,0,0,0,0,1,0])
# test = arrayToNum(test)
# starting_number = 134350849#2**1 + 2**7 + 2**12 + 2**21 + 2**31
# current_deltaSigX = 16777221#2**1 + 2**7 + 2**12 + 2**21 + 2**31
# print(current_deltaSigX)
# print(numToArray(current_deltaSigX))
# print(countChunks(current_deltaSigX))

# print('-------')
# print(numToArray(test))
# print(countChunks(test))

# goodValues = deltaSigGivenDeltaX(sigma_1, inv_sigma_1, current_deltaSigX, 10**5)
# good = []
# for g in goodValues:
# 	good.append(numToArray(g[0]))

# sortByHW(good)
# for g in good:
# 	print(g)

# print(len(good))
#########
# for i in range(0, 1000):
# 	x = random.randint(0, 2**32-1)
# 	x = numToArray(x)
# 	x[4] = 0
# 	x[5] = 0
# 	x[6] = 0
# 	x = arrayToNum(x)
# 	x_prime = x
# 	x_prime = numToArray(x_prime)
# 	x_prime[4] = 1
# 	x_prime[5] = 1
# 	x_prime[6] = 1
# 	x_prime = arrayToNum(x_prime)
# 	sig_x = calculate(sigma_0, x)
# 	sig_x_prime = calculate(sigma_0, x_prime)
# 	val = (2**32 - sig_x + sig_x_prime)%2**32
# 	print(val)
################
# print(goodValues)
#########

starting_number = np.array([0]*32)
starting_number[31-6] = 1
starting_number[31-14] = 1
starting_number[31-16] = 1
starting_number[31-17] = 1
starting_number[31-21] = 1
starting_number[31-25] = 1
starting_number[31-27] = 1
# starting_number[0] = 1
# starting_number[7] = 1
# starting_number[5] = 1

# starting_number = arrayToNum(starting_number)

sig00 = np.matmul(sigma_0, sigma_0)
sig11 = np.matmul(sigma_1, sigma_1)
sig100 = np.matmul(sigma_1, sig00)

for i in range(0, 32):
	for j in range(0, 32):
		sig100[i][j] = sig100[i][j]%2

# for i in sig100:
# 	print(i)
# s[0]=0
# s[4]=0
# s[12]=0

count=0
while True:
	hammingWeight = np.random.randint(1, 32)
	number = 0
	for i in range(0, hammingWeight):
		number = (number + 2**np.random.randint(0,32))%2**32
	starting_number = numToArray(number)
	a = np.matmul(starting_number, inv_sigma_1)
	a = numToArray(calculate(inv_sigma_1, arrayToNum(starting_number)))
	b = np.matmul(starting_number, sig00)
	can = True
	for i in range(0, 32):
		if a[i]%2!=b[i]%2:
			can = False
	if can==True:
		print("FOund")
		print(starting_number)
		print(a)
		print(b)
	count = count+1
	if count%1000==0:
		print(count)