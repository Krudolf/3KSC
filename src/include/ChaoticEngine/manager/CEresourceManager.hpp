 
#ifndef CERESOURCEMANAGER
#define CERESOURCEMANAGER

#include "CEresource.hpp"

class CEResourceManager{
public:
	static CEResourceManager* instance();

	CEResourceManager();
	~CEResourceManager();

	CEResource& getResource(const char* p_name);
	CEResource& checkFormat(const char* p_name);
	void deleteResources();
	void showResources();

private:
	static CEResourceManager* m_instance;
	std::vector<CEResource*> m_resources;

	const std::vector<std::string> m_types = {
		// even   odd
		"jpg", "tex", //Textures
		"jpeg", "tex",
		"png", "tex",
		"bin", "tex",
		"tga", "tex",
		"obj", "mesh", //Meshes
		"3ds", "mesh",
		"max", "mesh",
		"anim", "animation" //Animations
	};
};

#endif