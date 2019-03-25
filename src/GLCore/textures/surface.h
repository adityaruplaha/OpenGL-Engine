#pragma once

#include <map>
#include <string>

class Surface
{
public:
	static Surface* create(std::string path);

	Surface(std::string path);
	~Surface();
	int width, height, nrChannels;
	unsigned char* getData();
	std::string getPath();
	std::string getPath() const;

	// Clears the surface cache.
	static void clearCache();

private:
	const std::string path;
	unsigned char* data;

	static std::map<std::string, Surface*> surface_cache;
};

