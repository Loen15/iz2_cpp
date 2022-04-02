packages:
	sudo apt-get update -y && \
	sudo apt-get install build-essential && \
	sudo apt-get install -y cppcheck clang-tidy python3-pip libc6-dbg lcov libgtest-dev && \
	pip3 install cpplint && \
	sudo apt-get install valgrind

analysis:
	chmod +x ./linters/run.sh &&\
	./linters/run.sh

stat-build:
	mkdir build && \
	cd build && \
	cmake -DTESTS=ON ../iz2 && \
	make

stat-test:
	./build/tests/tests && \
	valgrind --tool=memcheck --leak-check=yes ./build/tests/tests

stat-report:
	lcov -t "build/tests/tests" -o coverage.info -c -d build/static_lib

din-build:
	mkdir build && \
	cd build && \
	cmake -DTESTS=ON -DSTREAMS=ON ../iz2 && \
	make
din-test:
	./build/tests/tests && \
	valgrind --tool=memcheck --leak-check=yes ./build/tests/tests

din-report:
	lcov -t "build/tests/tests" -o coverage.info -c -d build/dinamic_lib