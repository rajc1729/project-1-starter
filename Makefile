
CLANG_FLAGS = -std=c++17 -Wall -O -g

GTEST_FLAGS = -lpthread -lgtest_main -lgtest

print_score: rubricscore balance_rubric.json balance_test.xml
	./rubricscore balance_rubric.json balance_test.xml

rubricscore: rubricscore.cpp
		clang++ ${CLANG_FLAGS} rubricscore.cpp -o rubricscore

balance_test.xml: balance_test
	# || true allows make to continue the build even if some tests fail
	./balance_test --gtest_output=xml:./balance_test.xml || true

balance_test: gtest_lib balance.hpp balance_test.cpp
	clang++ ${CLANG_FLAGS} ${GTEST_FLAGS} balance_test.cpp -o balance_test

gtest_lib: /usr/lib/libgtest.a

/usr/lib/libgtest.a:
	@echo -e "google test library not installed\n"
	@echo -e "Installing libgtest-dev. Please provide the password when asked\n"
	@sudo apt-get -y install libgtest-dev cmake
	@sudo apt-get install cmake # install cmake
	@echo -e "\nConfiguring libgtest-dev\n"
	@cd /usr/src/gtest; sudo cmake CMakeLists.txt; sudo make; sudo cp *.a /usr/lib
	@echo -e "Finished installing google test library\n"

balance_timing: timer.hpp balance.hpp balance_timing.cpp
	clang++ ${CLANG_FLAGS} balance_timing.cpp -o balance_timing

clean:
		rm -f rubricscore balance_test balance_test.xml balance_timing
