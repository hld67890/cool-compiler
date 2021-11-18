import os
import subprocess

path = os.getcwd()

files = subprocess.run ( ['ls' , path + '/tests' ] , capture_output = True).stdout.decode().split()

# print ( files )

# mylex = os.getcwd () + '/lexer'
# stdlex = os.getcwd () + '/../../bin/lexer'

print ( "cases: " , files )

for case in files:
	out = subprocess.getoutput ( './checker ' + path + '/tests/' + case )
	print ( 'case' , case , ':' , out , '\n' )
