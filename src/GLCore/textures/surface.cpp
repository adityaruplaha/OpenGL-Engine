#include "surface.h"

#include <Common/log.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

std::map<std::string, Surface*> Surface::surface_cache{};

Surface* Surface::create(std::string path)
{
	auto iterator = surface_cache.find(path);
	if (iterator == surface_cache.end())
	{
		// Not found in the cache. Create surface & return it.
		return new Surface(path);
	}

	// Ok, surface exists. Now to check if its valid...
	auto* surface = iterator->second;
	if (surface && surface->getData())
	{
		LOG_INFO("Pre-cached surface found for %s: 0x%p", path.c_str(), surface_cache[path]);
		return surface_cache[path];
	}
	// Surface is invalid. Delete it and return a fresh one.
	LOG_WARNING("Pre-cached surface found for %s: 0x%p but it is invalid. Regenerating...", path.c_str(), surface_cache[path]);
	delete surface_cache[path];
	return Surface::create(path);
}

Surface::Surface(std::string path) : path(path)
{
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (!data)
	{
		LOG_ERROR("0x%p: Failed to load image %s.", this, path.c_str());
		return;
	}
	LOG_SUCCESS("0x%p: Loaded image %s.", this, path.c_str());

	surface_cache[path] = this;
}

Surface::~Surface()
{
	stbi_image_free(data);
}

unsigned char* Surface::getData()
{
	return data;
}

std::string Surface::getPath()
{
	return path;
}

std::string Surface::getPath() const
{
	return path;
}

void Surface::clearCache()
{
#ifdef SURFACE_CACHE_CLEAR_NO_WARNINGS
#else
	LOG_WARNING("SURFACE: The surface cache was manually cleared. You generally don't need to do this."
		"Use '#define SURFACE_CACHE_CLEAR_NO_WARNINGS' to disable this warning.");
#endif
	surface_cache.clear();
}