#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "./ven/dirent.h"

typedef std::vector<std::string> FileNamesList;

bool scanDirectoryForPNGs(std::string _dirName, FileNamesList& _return) {
	DIR* directory = opendir(_dirName.c_str());
	if(directory == NULL) {
		std::cerr << "directory " << _dirName << " not found" << std::endl;
		return false;
	}

	struct dirent* entity;
	entity = readdir(directory);

	while(entity != NULL) {
		std::string name(entity->d_name);
		if(name[0] != '.' && name[name.length()-4] == '.' && name[name.length()-3] == 'p' && name[name.length()-2] == 'n' && name[name.length()-1] == 'g') {
			_return.push_back(name);
		}
		entity = readdir(directory);
	}

	closedir(directory);

	return true;
}