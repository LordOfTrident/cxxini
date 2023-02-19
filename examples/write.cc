#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cxxini.hh>

int main() {
	ini::record rec({
		{"favorite", {
			{"color",  "blue"},
			{"animal", "dog"},
			{"food",   "pizza"},
		}},
	});

	rec[ini::default_section]["foo"] = "bar";
	rec["favorite"]["animal"]        = "cat";

	std::cout << rec << std::endl;

	return EXIT_SUCCESS;
}
