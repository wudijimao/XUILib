#include "../core/XResManager.hpp"

XResource::XString XResManager::pathForResource(const char *fileName, const char *ext) {
	std::string path = "X:\\Users\\wudijimao\\Documents\\GCD_dispatch\\res.bundle\\";
	path.append(fileName);
	path.append(".");
	path.append(ext);
	XResource::XString str(path);
	return str;
}