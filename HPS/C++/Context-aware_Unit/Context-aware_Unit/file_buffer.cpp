#include "file_buffer.h"

file_buffer::file_buffer(std::string path) {
	FILE* f = fopen(path.c_str(), "rb");
	if (f == NULL) {
		Printm('e', "File buffer failed to load " + path + ".");
	}
	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, 0, SEEK_SET);
	data = new char[size + 1];
	fread(data, 1, size, f);
	fclose(f);
}

file_buffer::~file_buffer() {
	delete data;
}