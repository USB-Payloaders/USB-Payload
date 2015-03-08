#ifndef PACKAGEQUE
#define PACKAGEQUE
#include "Package.h"
#include <queue>

class PackageQue
{
private:
	std::queue<Package> que;
public:
	PackageQue();
	void insert(Package new_Pack);
	bool Run_All();
};


#endif