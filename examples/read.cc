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

	file >> rec;
	// or rec.parse(file)

	std::cout << rec << std::endl;

	return EXIT_SUCCESS;
}
