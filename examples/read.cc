#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cxxini.hh>

int main() {
	ini::record rec;

	std::ifstream file("examples/test.ini");
	if (not file.is_open()) {
		std::cerr << "Failed to open file 'test.ini'" << std::endl;
		return EXIT_FAILURE;
	}

	size_t res = rec.parse(file);
	if (res != 0) {
		std::cerr << "Error while parsing 'test.ini' at line " << res << std::endl;
		return EXIT_FAILURE;
	}
	// You can also do file >> rec, but that provides no error checking

	std::cout << rec << std::endl;

	return EXIT_SUCCESS;
}
