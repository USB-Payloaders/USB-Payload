#ifndef PACKAGE_H
#define PACKAGE_H
#include <string>
#
#include "b64String.h"

class Package
{
private:
	std::string name;
	b64String encoded_code;
	bool had_ran;
public:
	Package (std::string name , b64String encoded_code);
	Package();
	void setName(std::string name);
	void setCode(b64String code);
	void changeState(); //Turning had_run to true;
	bool dump_toFile();
	bool get_hadRan();
	std::string getName();
	void Run();
};


#endif 