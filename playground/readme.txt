-------------
Dependencies:
-------------

already installed: openAl, openGl

brew install libjpeg
brew install libpng
brew install sdl2

-------------
Building:
-------------

'cmake-clean.sh' for cleaning cmake generated files

'cmake .' for generating make file - this does not build the executable, only puts toghether a makefile!
	'cmake -DCMAKE_BUILD_TYPE=Debug .' for debug profile
	'cmake -DCMAKE_BUILD_TYPE=Release .' for release profile
	
'make' for generating executable file in target directory


sample command line for rebuilding, running and tailing the log file:
--------------------------------------------------------------------

release: './cmake-clean.sh && cmake . && make && ./target/playground'

release & tail log: './cmake-clean.sh && cmake . && make && (./target/playground &  tail -n200 -f ./playground.log)'

debug: './cmake-clean.sh && cmake -DCMAKE_BUILD_TYPE=Debug . && make && lldb ./target/playground -S lldb.settings'