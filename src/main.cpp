#include <iostream>

#include "./image.hpp"
#include "./scan.hpp"

int main(int argc, char** argv) {
	FileNamesList skinsFileNames;
	if(!scanDirectoryForPNGs("in/skins/", skinsFileNames)) {
		return 1;
	}
	FileNamesList suitsFileNames;
	if(!scanDirectoryForPNGs("in/suits/", suitsFileNames)) {
		return 1;
	}
	FileNamesList tailsFileNames;
	if(!scanDirectoryForPNGs("in/tails/", tailsFileNames)) {
		return 1;
	}
	
	bool isTailed = false;
	for(const std::string& filenameSkin : skinsFileNames) {
		std::cout << filenameSkin;
		for(const std::string& filenameTail : tailsFileNames) {
			if(filenameSkin == filenameTail) {
				std::cout << " <has a tail>";
				isTailed = true;
			}
		}
		std::cout << ":" << std::endl;

		for(const std::string& filenameSuit : suitsFileNames) {
			Image skin;
			Image suit;
			Image tail;

			if(!skin.load("in/skins/" + filenameSkin)) {
				std::cerr << "SKIN HAS AN INAPPROPRIATE RESOLUTION <" << filenameSkin << "> - 64x64 required!" << std::endl;
				continue;
			}
			if(!suit.load("in/suits/" + filenameSuit)) {
				std::cerr << "SUIT HAS AN INAPPROPRIATE RESOLUTION <" << filenameSuit << "> - 64x64 required!" << std::endl;
				continue;
			}
			
			if(isTailed) {
				if(!tail.load("in/tails/" + filenameSkin)) {
				std::cerr << "TAIL  HAS AN INAPPROPRIATE RESOLUTION <" << filenameSkin << "> - 64x64 required!" << std::endl;
				continue;
				}
				
				suit -= tail;
			}
			skin += suit;

			std::string finalName = "out/" + filenameSkin.substr(0, filenameSkin.length() - 4) + "_" + filenameSuit;
			skin.save(finalName);
			std::cout << "Created: " << finalName << std::endl;
		}

		isTailed = false;
	}
	

	return 0;
}