'cmake-clean.sh' for cleaning cmake generated files
'cmake .' for generating make file
	'cmake -DCMAKE_BUILD_TYPE=Debug .' for debug profile
	'cmake -DCMAKE_BUILD_TYPE=Release .' for release profile
	
'make' for generating executable file in target directory


sample command line for rebuilding, running and tailing the log file:

'./cmake-clean.sh ; cmake . ; make ; ./target/playground &  tail -n200 -f ./playground.log'

