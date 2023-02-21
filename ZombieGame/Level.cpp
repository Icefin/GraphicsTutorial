#include "Level.h"
#include <Gengine/Errors.h>

#include <fstream>

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
}

Level::~Level() {

}