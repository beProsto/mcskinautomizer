#include <iostream>
#include <algorithm>

#include "./image.hpp"
#include "./scan.hpp"

bool combineImg(std::string _filenameSkin, std::string _filenameSuit, bool _isTailed, bool _isEared) {
	Image skin;
	Image suit;
	Image tail;
	Image ear;

	if(!skin.load("in/skins/" + _filenameSkin)) {
		std::cerr << "SKIN HAS AN INAPPROPRIATE RESOLUTION <" << _filenameSkin << "> - 64x64 required!" << std::endl;
		return false;
	}
	if(!suit.load("in/suits/" + _filenameSuit)) {
		std::cerr << "SUIT HAS AN INAPPROPRIATE RESOLUTION <" << _filenameSuit << "> - 64x64 required!" << std::endl;
		return false;
	}
	
	if(_isTailed) {
		if(!tail.load("in/tails/" + _filenameSkin)) {
			std::cerr << "TAIL HAS AN INAPPROPRIATE RESOLUTION <" << _filenameSkin << "> - 64x64 required!" << std::endl;
			return false;
		}

		suit -= tail;
	}
	if(_isEared) {
		if(!ear.load("in/ears/" + _filenameSkin)) {
			std::cerr << "EARS HAVE AN INAPPROPRIATE RESOLUTION <" << _filenameSkin << "> - 64x64 required!" << std::endl;
			return false;
		}

		skin -= ear;
	}
	skin += suit;

	std::replace(_filenameSuit.begin(), _filenameSuit.end(), '/', '_');
	std::replace(_filenameSkin.begin(), _filenameSkin.end(), '/', '_');

	std::string finalName = "out/" + _filenameSkin.substr(0, _filenameSkin.length() - 4) + "_" + _filenameSuit;
	
	skin.save(finalName);
	std::cout << "Created: " << finalName << std::endl;

	return true;
}

int main(int argc, char** argv) {
	FileNamesList skinsFileNames;
	if(!scanDirectoryForPNGs("in/skins/", skinsFileNames)) {
		return 1;
	}
	FileNamesList suitsFileNames;
	if(!scanDirectoryForPNGs("in/suits/", suitsFileNames)) {
		return 1;
	}
	FileNamesList earlessSuitsFileNames;
	if(!scanDirectoryForPNGs("in/suits/ignore_ears/", earlessSuitsFileNames)) {
		return 1;
	}
	FileNamesList tailsFileNames;
	if(!scanDirectoryForPNGs("in/tails/", tailsFileNames)) {
		return 1;
	}
	FileNamesList earsFileNames;
	if(!scanDirectoryForPNGs("in/ears/", earsFileNames)) {
		return 1;
	}
	
	bool isTailed = false;
	bool isEared = false;
	for(const std::string& filenameSkin : skinsFileNames) {
		std::cout << filenameSkin;
		for(const std::string& filenameTail : tailsFileNames) {
			if(filenameSkin == filenameTail) {
				std::cout << " <has a tail>";
				isTailed = true;
			}
		}
		for(const std::string& filenameEar : earsFileNames) {
			if(filenameSkin == filenameEar) {
				std::cout << " <has ears>";
				isEared = true;
			}
		}
		std::cout << ":" << std::endl;

		for(const std::string& filenameSuit : suitsFileNames) {
			combineImg(filenameSkin, filenameSuit, isTailed, false);
		}
		for(const std::string& filenameSuit : earlessSuitsFileNames) {
			combineImg(filenameSkin, "ignore_ears/" + filenameSuit, isTailed, isEared);
		}

		isTailed = false;
		isEared = false;
	}
	

	return 0;
}