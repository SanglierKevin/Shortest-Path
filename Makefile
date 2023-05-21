CC=gcc
CFLAGS=-Wall -Werror -g -O3
LIBS=-lcunit -lpthread
INCLUDE_HEADERS_DIRECTORY=-Iinclude
INCLUDE_TEST_HEADERS_DIRECTORY=-Itests/include

sp: src/sp.c src/graph.o src/helper.o # Add your other object files needed to compile your program here. The ordering is important. If file_a.o depends on file_b.o, file_a.o must be placed before file_b.o in the list.
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c # If for example you want to compute example.c, this will create an object file called example.o in the same directory as example.c. Don't forget to clean it in your "make clean".
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o $@ -c $<

clean:
	rm -f sp
	rm -f gcov
	rm -f cppcheck.xml
	rm -f valgrind.txt
	rm -f *.out
	rm -f *.gcda
	rm -f *.gcno
	rm -f *.gcov
	rm -f *.css
	rm -f *.html
	rm -f *.bin
	rm -f src/*.o
	rm -f tests/test
	rm -f tests/src/*.o
	rm -f tests/out/*
	touch tests/out/init.txt

tests: tests/src/test_comparison.o tests/src/test_bellman_ford.o tests/src/test_get_max.o tests/src/test_read_graph.o src/graph.o src/helper.o
	$(CC) $(INCLUDE_TEST_HEADERS_DIRECTORY) $(CFLAGS) -o tests/test tests/test.c $^ $(LIBS)
	./tests/test

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem --inconclusive --xml --xml-version=2 src/*.c include/*.h tests/*.c tests/src/*.c tests/include/*.h 2> cppcheck.xml

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind.txt ./sp tests/in/g200n800l.bin

gcov: src/sp.c src/graph.c src/helper.c
	$(CC) -fprofile-arcs -ftest-coverage -pg -o gcov src/sp.c src/graph.c src/helper.c
	./gcov tests/in/g200n800l.bin -v -f tests/out/testgcov.bin
	gcov -p gcov-graph.c
	gcov -p gcov-helper.c
	gcovr -r . --html --html-details --branches --sort-percentage --keep -o coverage.html
	xdg-open coverage.html