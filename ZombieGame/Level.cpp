#include "Level.h"
#include <Gengine/Errors.h>

#include <fstream>

#include <iostream>

Level::Level(const std::string& fileName) {
	std::ifstream file;
	file.open(fileName);

	if (file.fail()) {
		Gengine::FatalError("Failed to open" + fileName);
	}
	
	std::string temp;
	file >> temp >> _numHumans;

	while (std::getline(file, temp)) {
		_levelData.push_back(temp);
	}

	for (int i = 0; i < _levelData.size(); i++)
		std::cout << _levelData[i] << '\n';
}

Level::~Level() {

}