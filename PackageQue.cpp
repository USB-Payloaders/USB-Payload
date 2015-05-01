#include "PackageQue.h"


PackageQue::PackageQue()
{
	this->que = std::queue<Package>();
}

void PackageQue::insert(Package new_Pack)
{
	this->que.push(new_Pack);
}

bool PackageQue::Run_All()
{
	while(!this->que.empty())
	{
		try
		{
		Package temp = this->que.front();
		que.pop();
		temp.Run();
		}
		catch (int e)
		{
			std::cout << "One of the files had been corupted!";
			return false;
		}
		return true;
	}
}