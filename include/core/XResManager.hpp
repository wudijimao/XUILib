//
//  XResManager.hpp
//  XDUILib
//
//  Created by ximiao on 16/3/27.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
#pragma once

#include "stdafx.hpp"
#include "../res/XString.hpp"

class XFileInfo {
	XFileInfo(const XResource::XString &path) {
		init(path);
	}
	bool init(const XResource::XString &path) {
		return true;
	}
	XResource::XString mFullPath;
	XResource::XString mPath;
	XResource::XString mFullName;
	XResource::XString mName;
	XResource::XString mExt;
};

class XResManager {
public:
	static const char* pathForResource(const char *fileName, const char *ext);
	void openRes();
	//const char* pathForResource(const char *fileName, const char *ext);
};
