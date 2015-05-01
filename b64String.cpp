#include "b64String.h"
#include "base64.h"

b64String::b64String()
{
	this->str = std::string("\x00");
}
b64String::b64String(std::string str)
{
	this->str = str;
}

std::string b64String::decode()
{
	return base64_decode(this->str);
}

std::string b64String::get_str()
{
	return this->str;
}