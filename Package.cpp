#include "Package.h"
#include <iostream>
#include <fstream>

Package::Package()
{
	this-> name = "temp";
	this-> encoded_code = b64String();
	this-> had_ran = false;
}

Package::Package(std::string name, b64String code)
{
	this-> name = name;
	this-> encoded_code = b64String(code.get_str());
	this-> had_ran = false;
}

void Package::setName(std::string name)
{
	this-> name = name;
}

void Package::setCode(b64String code)
{
	this-> encoded_code = b64String(code.get_str());
}

void Package::changeState()
{
	this-> had_ran = true;
}

bool Package::get_hadRan()
{
	return this->had_ran;
}

bool Package::dump_toFile()
{
	std::string path = this->name + ".exe";
	try
	{
		std::ofstream exe;
		exe.open(path, std::ios::binary);
		exe << this->encoded_code.decode();
		exe.close();
	}
	catch (int e)
	{
		std::cout<<"NO WRITE PREMMISION\n";
		return false;
	}
	return true;
}

std::string Package::getName()
{
	return this->name;
}

void Package::Run()
{
	this->dump_toFile();
	system((this->name+ ".exe").c_str());

}