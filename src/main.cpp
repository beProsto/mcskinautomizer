#include <iostream>

#include "./image.hpp"
#include "./scan.hpp"

int main(int argc, char** argv) {
	// Image skin;
	// Image suit;
	// Image tail;

	// if(!skin.load("in/skins/hooh.png")) {
	// 	return 1;
	// }
	// if(!suit.load("in/suits/1.png")) {
	// 	return 1;
	// }
	// if(!tail.load("in/tails/hooh_t.png")) {
	// 	return 1;
	// }

	// std::cout << "loaded\n";
	
	// suit -= tail;
	// skin += suit;

	// std::cout << "added\n";

	// skin.save("out/hooh_1.png");


	FileNamesList files;
	if(!scanDirectoryForPNGs("in/skins/", files)) {
		return 1;
	}
	
	for(std::string& filename : files) {
		std::cout << filename << std::endl;
	}
	

	return 0;
}