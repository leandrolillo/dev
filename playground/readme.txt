'cmake .' for generating make file
make for generating executable file in target directory

especie de rebuild and run:
- modo debug
	rm -rf CMakeCache.txt target/*   ; cmake -DCMAKE_BUILD_TYPE=Debug ; make ; ./target/playground
- modo release
	rm -rf CMakeCache.txt target/*   ; cmake -DCMAKE_BUILD_TYPE=Release ; make ; ./target/playground
