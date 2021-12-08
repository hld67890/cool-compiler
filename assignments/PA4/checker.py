import os
import subprocess

path = os.getcwd()

files = subprocess.run ( ['ls' , path + '/tests/bad' ] , capture_output = True).stdout.decode().split()

# print ( files )

# mylex = os.getcwd () + '/lexer'
# stdlex = os.getcwd () + '/../../bin/lexer'

print ( "cases: " , files )

for case in files:
	out = subprocess.getoutput ( './mysemant ' + path + '/tests/bad/' + case )
	ans = subprocess.getoutput ( './stdsemant ' + path + '/tests/bad/' + case )
	# print ( out )
	# print ( ans )
	dout = set(out.split('\n'))
	dans = set(ans.split('\n'))
	
	print ( case + ":" )
	print ( "errors only in your output:" )
	for item in dout:
		if item not in dans:
			print ( item )
	print ( "errors only in answer:" )
	for item in dans:
		if item not in dout:
			print ( item )
	print ( "" )
	

files = subprocess.run ( ['ls' , path + '/tests/good' ] , capture_output = True).stdout.decode().split()

print ( "cases: " , files )

for case in files:
	out = subprocess.getoutput ( './mysemant ' + path + '/tests/good/' + case )
	ans = subprocess.getoutput ( './stdsemant ' + path + '/tests/good/' + case )
	# print ( out )
	# print ( ans )
	if out != ans :
		print ( case , "wrong:" )
		print ( "your output:" )
		print ( out )
		print ( "answer:" )
		print ( ans )
	else:
		print ( case , " passed" )
