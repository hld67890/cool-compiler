import os
import subprocess

path = os.getcwd()

files = subprocess.run ( ['ls' , path + '/tests' ] , capture_output = True).stdout.decode().split()

# print ( files )

mylex = os.getcwd () + '/lexer'
stdlex = os.getcwd () + '/../../bin/lexer'

print ( "cases: " , files )

for case in files:
	out = subprocess.run ( [mylex , path + '/tests/' + case ] , capture_output = True).stdout.decode()
	ans = subprocess.run ( [stdlex , path + '/tests/' + case ] , capture_output = True).stdout.decode()
	if not out == ans:
		print ( "WA on testcase " , case )
		print ( out )
	if case == 'null3.cl': print ( ans )

