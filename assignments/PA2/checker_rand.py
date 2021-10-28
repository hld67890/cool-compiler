import os
import subprocess
import random

path = os.getcwd()

fpath = path + '/tests/'

# print ( files )

mylex = os.getcwd () + '/lexer'
stdlex = os.getcwd () + '/../../bin/lexer'

for i in range(1000):
	nf = open ( fpath + 'rand.cl' , 'wb' )
	le = random.randint(1,1000)
	for j in range(le):
		ch = random.randint(0,127)
		nf.write(bytes([ch]))
	nf.close()
	
	out = subprocess.run ( [mylex , path + '/tests/' + 'rand.cl' ] , capture_output = True).stdout.decode()
	ans = subprocess.run ( [stdlex , path + '/tests/' + 'rand.cl' ] , capture_output = True).stdout.decode()
	if i % 100 == 0:
		print ( "testcase " , i )
	if not out == ans:
		print ( "WA on testcase " , i )
		print ( 'out:' , out )
		print ( 'ans:' , ans )
		break

