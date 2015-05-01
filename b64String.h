#ifndef B64_STRING
#define B64_STRING
#include <iostream>

class b64String
{
private:
	std::string str;
public:
	b64String();
	b64String(std::string str);
	std::string decode();
	std::string get_str();
};





#endif