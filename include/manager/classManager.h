#pragma once

#include <vector>
#include <string>

struct ClassType;

namespace ClassManager
{
	std::vector<ClassType*> getClasses();
	ClassType* getClass(const std::string& id);
};
