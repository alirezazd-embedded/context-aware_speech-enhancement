#pragma once
#include <fstream>
#include <string>
#include "printm.h"

class file_buffer
{
public:
	file_buffer(std::string path);
	~file_buffer();

	char* data;
	long size;
};