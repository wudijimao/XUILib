#include "../core/XResManager.hpp"

XResource::XString XResManager::pathForResource(const char *fileName, const char *ext) {
	std::string path = "C:\\Users\\wudijimao\\Documents\\GCD-dispatch-\\res.bundle\\";
	path.append(fileName);
	path.append(".");
	path.append(ext);
	XResource::XString str(path);
	return str;
}