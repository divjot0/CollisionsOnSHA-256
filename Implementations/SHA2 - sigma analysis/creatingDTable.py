import numpy as np

def arrayToNum(vec):
	return vec.dot(2**np.arange(vec.size)[::-1])

def numToArray(n):
	return bin_array(n, 32)

def bin_array(num, m):
	"""Convert a positive integer num into an m-bit bit vector"""
	return np.array(list(np.binary_repr(num).zfill(m))).astype(np.int64)

def rotateAndAddToMatrix(A, k):
	for i in range(0, 32):
		A[i][(32+i+k)%32] = A[i][(32+i+k)%32]^1

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

def convertCrossToNumArray(crossArray):
	numArray = np.array([0]*32)
	for i in range(0, 32):
		if crossArray[i]!="-":
			numArray[i] = 1
	return numArray

def convertNumArrayToCross(numArray):
	crossArray = np.array(["-"]*32)
	for i in range(0, 32):
		if numArray[i]==1:
			crossArray[i] = "x"
	return crossArray

def printXPositions(crossArray, toPrint=True):
	total = 0
	xPos = []
	if toPrint==True:
		print("Printing cross positions for"),
		print(''.join(j for j in crossArray))
	for i in range(0, 32):
		if crossArray[i]!="-":
			if toPrint==True:
				print(i),
			total = total+1
			xPos.append(i)
	if toPrint==True:
		print("\nDone total = "),
		print(total)
	return xPos

def moduloHelper(x):
	return int(x)%2

# as in code theory
def applyMatrix(A, x):
	result = np.matmul(x, A)
	moduloInv = np.vectorize(moduloHelper)
	result = moduloInv(result)
	return result

def calculate(sigma, x):
	vec = numToArray(x)
	vec_new = applyMatrix(sigma, vec)
	vec_new_num = arrayToNum(vec_new)
	return vec_new_num

def addConditionBecauseOfIf(conditions, deltaE, startStep, pos, sameOrDiff):
	if sameOrDiff=="same":
		if deltaE[startStep][pos]=="-":
			if deltaE[startStep-1][pos]=="-":
				conditions.add("E"+ str(startStep) +"[" + str(pos) + "]=1")
			else:
				conditions.add("E"+ str(startStep) +"[" + str(pos) + "]=0")
		else:
			if deltaE[startStep-1][pos]=="-" and deltaE[startStep-2][pos]=="-":
				conditions.add("E"+ str(startStep-1) +"[" + str(pos) + "]" + "=" + "E"+ str(startStep-2) +"[" + str(pos) + "]")
			elif deltaE[startStep-1][pos]!="-" and deltaE[startStep-2][pos]!="-":
				conditions.add("E"+ str(startStep-1) +"[" + str(pos) + "]" + "!=" + "E"+ str(startStep-2) +"[" + str(pos) + "]")

	if sameOrDiff=="different":
		if deltaE[startStep][pos]=="-":
			if deltaE[startStep-1][pos]=="-":
				conditions.add("E"+ str(startStep) +"[" + str(pos) + "]=0")
			else:
				conditions.add("E"+ str(startStep) +"[" + str(pos) + "]=1")
		else:
			if deltaE[startStep-1][pos]=="-" and deltaE[startStep-2][pos]=="-":
				conditions.add("E"+ str(startStep-1) +"[" + str(pos) + "]" + "!=" + "E"+ str(startStep-2) +"[" + str(pos) + "]")
			elif deltaE[startStep-1][pos]!="-" and deltaE[startStep-2][pos]!="-":
				conditions.add("E"+ str(startStep-1) +"[" + str(pos) + "]" + "=" + "E"+ str(startStep-2) +"[" + str(pos) + "]")
	return

def fillE7Cross(conditions, deltaE, deltaW):
	return printXPositions(deltaW[7], False)

def fillE8Cross(conditions, deltaE, deltaW):
	xPosInE8 = []
	eNum = convertCrossToNumArray(deltaE[7])
	e7 = arrayToNum(eNum)
	S1e7 = calculate(Sigma_1, e7)
	S1e7cross = convertNumArrayToCross(numToArray(S1e7))
	xPosInSig1E7 = printXPositions(S1e7cross, False)
	xPosInW8 = printXPositions(deltaW[8], False)
	for i in range(0, 32):
		if ((i in xPosInSig1E7) and (i not in xPosInW8)) or ((i not in xPosInSig1E7) and (i in xPosInW8)):
			xPosInE8.append(i)
	return xPosInE8

def fillE9Cross(conditions, deltaE, deltaW):
	xPosInE9 = []
	eNum = convertCrossToNumArray(deltaE[8])
	e8 = arrayToNum(eNum)
	S1e8 = calculate(Sigma_1, e8)
	S1e8cross = convertNumArrayToCross(numToArray(S1e8))
	xPosInSig1E8 = printXPositions(S1e8cross, False)
	xPosInE8 = printXPositions(deltaE[8], False)
	xPosInE7 = printXPositions(deltaE[7], False)
	xPosInE6 = printXPositions(deltaE[6], False)
	
	# -1 means can be both, 0 means is going to be same, 1 means is going to be different
	ifFunctionCanBe = [-1]*32
	for i in range(0, 32):
		if i not in xPosInE8:
			if (((i in xPosInE7) and (i not in xPosInE6)) or ((i not in xPosInE7) and (i in xPosInE6))):
				ifFunctionCanBe[i]=-1
			elif ((i in xPosInE6) and (i in xPosInE7)):
				ifFunctionCanBe[i]=1
			else:
				ifFunctionCanBe[i]=0
	
	for i in range(0, 32):
		if (i in xPosInSig1E8):
			if (ifFunctionCanBe[i]==0):
				xPosInE9.append(i)
			elif (ifFunctionCanBe[i]==-1):
				addConditionBecauseOfIf(conditions, deltaE, 8, i, "different")
		else:
			if ifFunctionCanBe[i]==1:
				xPosInE9.append(i)
			elif ifFunctionCanBe[i]==-1:
				addConditionBecauseOfIf(conditions, deltaE, 8, i, "same")
	return xPosInE9

def fillE10Cross(conditions, deltaE, deltaW):
	xPosInE10 = []
	eNum = convertCrossToNumArray(deltaE[9])
	e9 = arrayToNum(eNum)
	S1e9 = calculate(Sigma_1, e9)
	S1e9cross = convertNumArrayToCross(numToArray(S1e9))
	xPosInSig1E9 = printXPositions(S1e9cross, False)
	xPosInE9 = printXPositions(deltaE[9], False)
	xPosInE8 = printXPositions(deltaE[8], False)
	xPosInE7 = printXPositions(deltaE[7], False)
	
	# -1 means can be both, 0 means is going to be same, 1 means is going to be different
	ifFunctionCanBe = [-1]*32
	for i in range(0, 32):
		if i not in xPosInE9:
			if (((i in xPosInE8) and (i not in xPosInE7)) or ((i not in xPosInE8) and (i in xPosInE7))):
				ifFunctionCanBe[i]=-1
			elif ((i in xPosInE7) and (i in xPosInE8)):
				ifFunctionCanBe[i]=1
			else:
				ifFunctionCanBe[i]=0
	
	for i in range(0, 32):
		if (i in xPosInSig1E9):
			if (ifFunctionCanBe[i]==0):
				xPosInE10.append(i)
			elif (ifFunctionCanBe[i]==-1):
				addConditionBecauseOfIf(conditions, deltaE, 9, i, "different")
		else:
			if ifFunctionCanBe[i]==1:
				xPosInE10.append(i)
			elif ifFunctionCanBe[i]==-1:
				addConditionBecauseOfIf(conditions, deltaE, 9, i, "same")
	return xPosInE10


def fillE13Cross(conditions, deltaE, deltaW):
	return printXPositions(deltaW[17], False)

def fillE11Cross(conditions, deltaE, deltaW):
	#smartly
	xPosInIf = [3,14,15,19,24,25,26,28]#[3,4,14,15,18,19,24,25,26,28]
	conditions.add("E10[21]!=E10[22]")
	conditions.add("E10[29]!=E10[30]")
	eNum = convertCrossToNumArray(deltaE[13])
	e13 = arrayToNum(eNum)
	S1e13 = calculate(Sigma_1, e13)
	S1e13cross = convertNumArrayToCross(numToArray(S1e13))
	xPosInSig1E13 = printXPositions(S1e13cross, False)
	xPosInE10 = printXPositions(deltaE[10], False)
	# for i in range(0, 32):
	# 	if ((i in xPosInSig1E13) and (i not in xPosInE10)) or ((i not in xPosInSig1E13) and (i in xPosInE10)):
	# 		xPosInIf.append(i)
	print("This is E10")
	print(xPosInE10)
	print("This is Sig1E13 difference")
	print(xPosInSig1E13)
	# print("This is if difference")
	# print(xPosInIf)

	xPosInE11 = []
	xPosInE13 = printXPositions(deltaE[13], False)
	xPosInE12 = printXPositions(deltaE[12], False)
	print("This is E12 difference")
	print(xPosInE12)
	print("This is E13 difference")
	print(xPosInE13)
	xPosInW15 = printXPositions(deltaW[15], False)
	print("This is W15 difference")
	print(xPosInW15)
	# print("This is 11,12,13")
	# print(''.join(j for j in deltaE[11]))
	# print(''.join(j for j in deltaE[12]))
	# print(''.join(j for j in deltaE[13]))
	for i in range(0, 32):
		if i==29 or i==30:
			continue
		if i==0:
			xPosInE11.append(i)
			conditions.add("E"+ str(14) +"[" + str(i) + "]=0")
		elif i==3:
			xPosInE11.append(i)
			conditions.add("E"+ str(14) +"[" + str(i) + "]=0")
		elif i==4:
			xPosInE11.append(i)
			conditions.add("E"+ str(14) +"[" + str(i) + "]=0")
		elif i==18:
			xPosInE11.append(i)
			conditions.add("E"+ str(14) +"[" + str(i) + "]=0")
		elif (i in xPosInW15):
			if ((i in xPosInE13) and (i not in xPosInE12)):
				conditions.add("E"+ str(14) +"[" + str(i) + "]=1")
			elif ((i not in xPosInE13) and (i in xPosInE12)):
				conditions.add("E"+ str(14) +"[" + str(i) + "]=0")
			elif ((i not in xPosInE13) and (i not in xPosInE12)):
				xPosInE11.append(i)
		else:
			if ((i in xPosInE13) and (i not in xPosInE12)):
				conditions.add("E"+ str(14) +"[" + str(i) + "]=0")
			elif ((i not in xPosInE13) and (i in xPosInE12)):
				conditions.add("E"+ str(14) +"[" + str(i) + "]=1")
			elif ((i in xPosInE13) and (i in xPosInE12)):
				xPosInE11.append(i)
	conditions.add("E14[29]=1")
	conditions.add("E13[29]=W15[29]")
	conditions.add("W15[29]!=W15[30]")

	for i in range(0, 32):
		if i in xPosInIf:
			if i in xPosInE13:
				if (i not in xPosInE12) and (i not in xPosInE11):
					conditions.add("E"+ str(13-1) +"[" + str(i) + "]" + "!=" + "E"+ str(13-2) +"[" + str(i) + "]")
			else:
				if i in xPosInE12:
					conditions.add("E"+ str(13) +"[" + str(i) + "]=1")
				else:
					if i not in xPosInE11:
						print("SOMETHING WORMNG")
					conditions.add("E"+ str(13) +"[" + str(i) + "]=0")
		else:
			if i in xPosInE13:
				if (i not in xPosInE12) and (i not in xPosInE11):
					conditions.add("E"+ str(13-1) +"[" + str(i) + "]" + "=" + "E"+ str(13-2) +"[" + str(i) + "]")
			else:
				if i not in xPosInE12:
					conditions.add("E"+ str(13) +"[" + str(i) + "]=1")
				else:
					if i in xPosInE11:
						print("SOMETHING WORMNG")
					conditions.add("E"+ str(13) +"[" + str(i) + "]=0")
	
	print("This is E11 difference")
	print(xPosInE11)
	
	return xPosInE11

def conditionsForA13(conditions, deltaE, deltaW):
	eNum = convertCrossToNumArray(deltaE[12])
	e12 = arrayToNum(eNum)
	S1e12 = calculate(Sigma_1, e12)
	S1e12cross = convertNumArrayToCross(numToArray(S1e12))
	xPosInS1e12 = printXPositions(S1e12cross, False)

	xPosInE9 = printXPositions(deltaE[9], False)
	xPosInE10 = printXPositions(deltaE[10], False)
	xPosInE11 = printXPositions(deltaE[11], False)
	xPosInE12 = printXPositions(deltaE[12], False)
	conditions.add("E9[3]!=E11[3]")
	conditions.add("E12[0]=1")
	conditions.add("E12[3]=1")
	conditions.add("E12[4]=1")
	conditions.add("E12[8]=1")
	conditions.add("E12[10]=1")
	conditions.add("E12[12]=0")
	conditions.add("E12[17]=0")
	conditions.add("E12[18]=1")
	conditions.add("E12[19]=0")
	conditions.add("E12[20]=0")
	conditions.add("E12[21]=1")
	conditions.add("E12[22]=1")
	conditions.add("E12[24]=1")
	conditions.add("E11[25]=E10[25]")
	conditions.add("E12[29]=1")
	conditions.add("E12[30]=1")
	conditions.add("E12[31]=0")
	return


def fillE12Cross(conditions, deltaE, deltaW):
	xPosInE12 = [25, 26]
	xPosInE13 = printXPositions(deltaE[13], False)

	for i in range(0, 32):
		if (i in xPosInE13) and (i!=25):
			conditions.add("E"+ str(15) +"[" + str(i) + "]=1")
		elif i==25:
			conditions.add("E"+ str(15) +"[" + str(i) + "]=0")
			conditions.add("E12[25]!=E12[26]")

	return xPosInE12

def fillACross(conditions, deltaE, deltaW, position):
	e12Num = convertCrossToNumArray(deltaE[position+3])
	e12 = arrayToNum(e12Num)
	S1e12 = calculate(Sigma_1, e12)
	S1e12cross = convertNumArrayToCross(numToArray(S1e12))
	xPosInSig1E12 = printXPositions(S1e12cross, False)
	xPosInE9 = printXPositions(deltaE[position], False)
	xPosInE13 = printXPositions(deltaE[position+4], False)
	count = [0]*32
	for i in range(0, 32):
		if i in xPosInSig1E12:
			count[i]=count[i]+1
		if i in xPosInE9:
			count[i]=count[i]+1
		if i in xPosInE13:
			count[i]=count[i]+1
	print("This is count array for " + str(position))
	print(count)
	return []


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

def addRestOfConditions(conditions, deltaE, deltaW):
	xPosInE13 = printXPositions(deltaE[13], False)
	for i in xPosInE13:
		conditions.add("E13["+str(i)+"]"+"!="+"W17["+str(i)+"]")

	xPosInE7 = printXPositions(deltaE[7], False)
	for i in xPosInE7:
		conditions.add("E7["+str(i)+"]"+"!="+"W7["+str(i)+"]")

	# eNum = convertCrossToNumArray(deltaE[7])
	# e7 = arrayToNum(eNum)
	# S1e7 = calculate(Sigma_1, e7)
	# S1e7cross = convertNumArrayToCross(numToArray(S1e7))
	# xPosInSig1E7 = printXPositions(S1e7cross, False)
	# print("this is pos sig1E7: ")
	# print(xPosInSig1E7)
	# xPosInW8 = printXPositions(deltaW[8], False)
	# xPosInE8 = printXPositions(deltaE[8], False)
	# print("This is W8 difference")
	# print(xPosInW8)
	# print("This is E8 difference")
	# print(xPosInE8)

	# eNum = convertCrossToNumArray(deltaE[8])
	# e8 = arrayToNum(eNum)
	# S1e8 = calculate(Sigma_1, e8)
	# S1e8cross = convertNumArrayToCross(numToArray(S1e8))
	# xPosInSig1E8 = printXPositions(S1e8cross, False)
	# print("this is pos sig1E8: ")
	# print(xPosInSig1E8)
	# xPosInE6 = printXPositions(deltaE[6], False)
	# xPosInE7 = printXPositions(deltaE[7], False)
	# xPosInE8 = printXPositions(deltaE[8], False)
	# print("This is E6 difference")
	# print(xPosInE6)
	# print("This is E7 difference")
	# print(xPosInE7)
	# print("This is E8 difference")
	# print(xPosInE8)
	# xPosInE9 = printXPositions(deltaE[9], False)
	# print("This is E9 difference")
	# print(xPosInE9)
	



Sigma_0 = intializeSigma(2, 13, 22)
Sigma_1 = intializeSigma(6, 11, 25)


inv_Sigma_1_helper = modMatInv(Sigma_1, 2)
moduloInv = np.vectorize(moduloHelper)
inv_Sigma_1 = moduloInv(inv_Sigma_1_helper)

r = np.array([0]*32)
# r[31-28] = 1;
# r[31-26] = 1;

# [1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
# [1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0]
temp = np.array([1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0])

r = np.matmul(inv_Sigma_1, temp);
r = moduloInv(r)
l = []
for i in range(0, 32):
	if r[i]==1:
		l.append(i)
print("E8: "),
print(r)
print("E8 (l): "),
print(l)

# THIS IS GOOD AND SHOULD WORK
e7 = np.array([0]*32)
e7[3] = 1
e7[4] = 1
e7[5] = 1
e7[6] = 1
e7[7] = 1
e7[8] = 1
e7[9] = 1
e7[10] = 1
e7[11] = 1
e7[12] = 1
e7[13] = 1
e7[14] = 1
e7[15] = 1
e7[20] = 1
e7[21] = 1
e7[22] = 1
e7[23] = 1
e7[24] = 1
e7[25] = 1
e7[26] = 1
e7[27] = 1
e7[31] = 1
r = np.matmul(Sigma_1, e7)
r = moduloInv(r)
l = []
for i in range(0, 32):
	if r[i]==1:
		l.append(i)
print("sigE7: "),
print(r)
print("sigE7 (l): "),
print(l)
r = e7
l = []
for i in range(0, 32):
	if r[i]==1:
		l.append(i)
print("E7: "),
print(r)
print("E7 (l): "),
print(l)


# deltaW = []
# for i in range(0, 23):
# 	row = ["-"]*32
# 	deltaW.append(row)

# deltaW8 = [2,3,4,6,7,8,10,12,13,16,17,18,20,21, 23,24,27,28,29,30,31]
# deltaW7 = [0,2,3,4,5,7,9,10,12,13,15,17,20,21,23,24,25,27,29,30]#[3, 20, 31]
# deltaW15 = [1,2,4,6,8,11,13,14,17,18,19,25,27,28,30,31]
# deltaW17 = [2,3,4,6,7,8,10,12,13,16,17,18,20,21, 23,24,27,28,29,30,31]
# deltaW12 = [5,9,10,11,15,16,17,19,21,22,26,27,28,29,30]

# for i in deltaW7:
# 	deltaW[7][31-i] = "x"
# for i in deltaW8:
# 	deltaW[8][31-i] = "x"
# for i in deltaW12:
# 	deltaW[12][31-i] = "x"
# for i in deltaW15:
# 	deltaW[15][31-i] = "x"
# for i in deltaW17:
# 	deltaW[17][31-i] = "x"	


# deltaE = []
# for i in range(0, 23):
# 	row = ["-"]*32
# 	deltaE.append(row)

# conditions = set()

# ### to find E8
# # print("CHeck this")
# # deltaCheck = ["-"]*32
# # deltaCheck[0] = "x"
# # eNum = convertCrossToNumArray(deltaCheck)
# # e7 = arrayToNum(eNum)
# # S1e7 = calculate(Sigma_1, e7)
# # S1e7cross = convertNumArrayToCross(numToArray(S1e7))
# # print(''.join(j for j in S1e7cross))
# # print(''.join(j for j in deltaW[8]))
# # printXPositions(S1e7cross)
# E7Positions = fillE7Cross(conditions, deltaE, deltaW)
# for i in E7Positions:
# 	deltaE[7][i] = "x"

# E8Positions = fillE8Cross(conditions, deltaE, deltaW)
# for i in E8Positions:
# 	deltaE[8][i] = "x"

# E9Positions = fillE9Cross(conditions, deltaE, deltaW)
# for i in E9Positions:
# 	deltaE[9][i] = "x"

# E10Positions = fillE10Cross(conditions, deltaE, deltaW)
# for i in E10Positions:
# 	deltaE[10][i] = "x"

# E13Positions = fillE13Cross(conditions, deltaE, deltaW)
# for i in E13Positions:
# 	deltaE[13][i] = "x"

# E12Positions = fillE12Cross(conditions, deltaE, deltaW)
# for i in E12Positions:
# 	deltaE[12][i] = "x"

# deltaE[12][25] = "u"
# deltaE[12][26] = "n"
# deltaE[13][25] = "u"
# deltaW[17][25] = "n"
# deltaW[8][25] = "u"

# E11Positions = fillE11Cross(conditions, deltaE, deltaW)
# for i in E11Positions:
# 	deltaE[11][i] = "x"

# conditionsForA13(conditions, deltaE, deltaW)
# addRestOfConditions(conditions, deltaE, deltaW)

# conditionsInList = list(conditions)
# conditionsInList.sort()
# print(conditionsInList)
# # eNum = convertCrossToNumArray(deltaE[9])
# # e9 = arrayToNum(eNum)
# # S1e9 = calculate(Sigma_1, e9)
# # S1e9cross = convertNumArrayToCross(numToArray(S1e9))
# # print(''.join(j for j in S1e9cross))
# # # print(''.join(j for j in deltaW[8]))
# # printXPositions(S1e9cross)

# print("\n\n")
# eNum = convertCrossToNumArray(deltaE[9])
# e9 = arrayToNum(eNum)
# S1e9 = calculate(Sigma_1, e9)
# S1e9cross = convertNumArrayToCross(numToArray(S1e9))
# xPosInSig1E9 = printXPositions(S1e9cross, False)
# print("this is pos sig1E9: ")
# print(xPosInSig1E9)
# xPosInE7 = printXPositions(deltaE[7], False)
# xPosInE8 = printXPositions(deltaE[8], False)
# print("This is E7 difference")
# print(xPosInE7)
# print("This is E8 difference")
# print(xPosInE8)
# xPosInE9 = printXPositions(deltaE[9], False)
# print("This is E9 difference")
# print(xPosInE9)
# xPosInE10 = printXPositions(deltaE[10], False)
# print("This is E10 difference")
# print(xPosInE10)
# print("\n\n")
# eNum = convertCrossToNumArray(deltaE[8])
# e8 = arrayToNum(eNum)
# S1e8 = calculate(Sigma_1, e8)
# S1e8cross = convertNumArrayToCross(numToArray(S1e8))
# xPosInSig1E8 = printXPositions(S1e8cross, False)
# print("this is pos sig1E8: ")
# print(xPosInSig1E8)
# xPosInE6 = printXPositions(deltaE[6], False)
# xPosInE7 = printXPositions(deltaE[7], False)
# print("This is E6 difference")
# print(xPosInE6)
# print("This is E7 difference")
# print(xPosInE7)
# xPosInE8 = printXPositions(deltaE[8], False)
# print("This is E8 difference")
# print(xPosInE8)
# xPosInE9 = printXPositions(deltaE[9], False)
# print("This is E9 difference")
# print(xPosInE9)
# xPosInW17 = printXPositions(deltaW[17], False)
# print("This is W17 difference")
# print(xPosInW17)
# print("\n\n")


# eNum = convertCrossToNumArray(deltaE[12])
# e12 = arrayToNum(eNum)
# S1e12 = calculate(Sigma_1, e12)
# S1e12cross = convertNumArrayToCross(numToArray(S1e12))
# xPosInS1e12 = printXPositions(S1e12cross, False)
# print("This is s1e12 difference")
# print(xPosInS1e12)
# print(''.join(j for j in S1e12cross))
# print(''.join(j for j in deltaE[9]))
# print(''.join(j for j in deltaE[10]))
# print(''.join(j for j in deltaE[11]))
# print(''.join(j for j in deltaE[12]))



# ##### STARTING AAAAAA
# findA9 = fillACross(conditions, deltaE, deltaW, 9)
# findA8 = fillACross(conditions, deltaE, deltaW, 8)
# findA7 = fillACross(conditions, deltaE, deltaW, 7)


# print("Printing deltaW:")
# for i in range(0, 23):
# 	print("%02d" % (i,)),
# 	deltaRow = ''.join(j for j in deltaW[i])
# 	print(deltaRow)

# print("\nPrinting deltaE:")
# for i in range(0, 23):
# 	print("%02d" % (i,)),
# 	deltaRow = ''.join(j for j in deltaE[i])
# 	print(deltaRow)