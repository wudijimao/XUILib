//
//  XResManager.cpp
//  XDUILib
//
//  Created by ximiao on 16/3/27.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#include "XResManager.hpp"
#include <sys/stat.h>
#include <sys/types.h>  //TODO::put into .cpp
#ifdef TARGET_OS_MSWINDOWS
#include "../win32/dirent.h"
//#include <direct.h>
#include  <io.h>
#include  <stdio.h>
#include  <stdlib.h>
#include <sys/stat.h>  

#else
#include <dirent.h>
#define _mkdir(str) mkdir(str, S_IRWXU | S_IRWXG | S_IRWXO)
#endif

std::map<std::string, std::shared_ptr<XDictionaryInfo>>  XDictionaryInfo::sDictMap;

std::map<std::string, std::shared_ptr<XFileInfo>> XFileInfo::sFilesMap;

const XResource::XString& XPathInfo::getPath() {
	return mPath;
}
XPathInfo::~XPathInfo() {};
bool XPathInfo::isExist() {
	if (!mHasCheckExist) {
		int a = access(mPath.getUTF8String().c_str(), 1);
		setIsExist(a == 0);
	}
	return mIsExist;
}
bool XPathInfo::init(const char* fullPath) {
	mPath = fullPath;
	mPath.replaceAll("\\", "/");
	mHasCheckExist = false;
	return true;
}

void XPathInfo::setIsExist(bool isExist) {
	mIsExist = isExist;
	mHasCheckExist = true;
}



XDictionaryInfo::XDictionaryInfo(const char* fullPath) {
	init(fullPath);
}
#ifdef TARGET_OS_MSWINDOWS
bool XDictionaryInfo::isExist() {
	WIN32_FIND_DATA  wfd;
	bool rValue = false;
	HANDLE hFind = FindFirstFile(mPath.UTF8CStr(), &wfd);
	if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		rValue = true;
	}
	FindClose(hFind);
	return rValue;
}
#endif // TARGET_OS_MSWINDOWS

bool XDictionaryInfo::init(const char* fullPath) {
	if (XPathInfo::init(fullPath)) {
		mHasReadChildren = false;
		mFiles.clear();
		mDicts.clear();
	}
	return false;
}
const std::vector<std::shared_ptr<XFileInfo>> XDictionaryInfo::allFilesInfo() {
	return mFiles;
}
const std::vector<std::shared_ptr<XDictionaryInfo>> XDictionaryInfo::allDictionarysInfo() {
	return mDicts;
}
std::shared_ptr<XFileInfo> XDictionaryInfo::fileInfoFor(const char *fullFileName) {
	if (fullFileName != nullptr) {
		auto path = mPath + XResource::XString("/") + fullFileName;
		auto ret = XFileInfo::fileInfoForPath(path.UTF8CStr());
		return ret;
	}
	return nullptr;
}
//if not found return nullptr
std::shared_ptr<XFileInfo> XDictionaryInfo::fileInfoFor(const char *name, const char *ext) {
	if (name != nullptr && ext != nullptr) {
		std::string str(name);
		str.append(".");
		str.append(ext);
		return fileInfoFor(str.c_str());
	}
	return nullptr;
}
std::vector<std::shared_ptr<XFileInfo>> XDictionaryInfo::searchFilesInfoFor(const char *ext) {
	std::vector<std::shared_ptr<XFileInfo>> files;
	if (ext != nullptr) {
		for (auto file : mFiles) {

		}
	}
	return files;
}

std::shared_ptr<XDictionaryInfo> XDictionaryInfo::dictInfoForPath(const char *fullPath) {
	auto iter = sDictMap.find(fullPath);
	if (iter != sDictMap.end()) {
		return iter->second;
	}
	else {
		auto dictInfo = std::make_shared<XDictionaryInfo>(fullPath);;
		sDictMap[fullPath] = dictInfo;
		return dictInfo;
	}
}


bool XDictionaryInfo::readChildren() {
	if (mHasReadChildren) {
		return true;
	}
	if (isExist()) {
		const char *path = mPath.UTF8CStr();
		DIR *pDir = opendir(path);
		if ((pDir) == NULL)
		{
			return false;
		}
		struct dirent* ent = NULL;
		char dir[1024];
		struct _stat statbuf;
		while ((ent = readdir(pDir)) != NULL)
		{ //统计当前文件夹下有多少文件（不包括文件夹）
		  //得到读取文件的绝对路径名
			snprintf(dir, 512, "%s/%s", path, ent->d_name);
			//得到文件信息
			_stat(dir, &statbuf);
			if (S_ISDIR(statbuf.st_mode))
			{
				mDicts.push_back(std::make_shared<XDictionaryInfo>(dir));
			}
			else {
				mFiles.push_back(std::make_shared<XFileInfo>(dir));
			}
		} //while
		closedir(pDir);
		mHasReadChildren = true;
		return true;
	}
	return false;
}


std::shared_ptr<XFileInfo> XFileInfo::fileInfoForPath(const char *fullPath) {
	auto iter = sFilesMap.find(fullPath);
	if (iter != sFilesMap.end()) {
		return iter->second;
	}
	else {
		auto fileInfo = std::make_shared<XFileInfo>(fullPath);
		sFilesMap[fullPath] = fileInfo;
		return fileInfo;
	}
}

#ifdef TARGET_OS_MSWINDOWS
bool XFileInfo::isExist() {
	FILE *file = fopen(mPath.UTF8CStr(), "r");
	if (file)
	{
		fclose(file);
		return true;
	}
	return false;
}
#endif // TARGET_OS_MSWINDOWS

XFileInfo::XFileInfo(const char *path) {
	init(path);
}
bool XFileInfo::init(const char *path) {
	if (XPathInfo::init(path)) {
		unsigned long pos = mPath.find("/", XResource::XStringCompareOptions::SearchFromBackward);
		if (pos > 0) {
			mDictPath = mPath.subStr(0, pos);
			++pos;
			mFullName = mPath.subStr(pos, mPath.length() - pos);
			pos = mFullName.find(".", XResource::XStringCompareOptions::SearchFromBackward);
			if (pos > 0) {
				mName = mFullName.subStr(0, pos);
				++pos;
				mExt = mFullName.subStr(pos, mFullName.length() - pos);
			}
			else {
				mName = mFullName;
				mExt = XResource::XString::emptyStr();
			}
		}
		return true;
	}
	return false;
}
const XResource::XString& XFileInfo::ext() {
	return mExt;
}
const XResource::XString& XFileInfo::name() {
	return mName;
}
const XResource::XString& XFileInfo::fullName() {
	return mFullName;
}
const XResource::XString& XFileInfo::dictPath() {
	return mDictPath;
}


inline XSandBox &XSandBox::sharedSandBox() {
	static XSandBox sandBox;
	return sandBox;
}
//app Install Dict
std::shared_ptr<XDictionaryInfo> XSandBox::cacheDict() {
	if (!mCacheDict) {
		auto path = homeDict()->getPath();
		path.append("/Library/Caches");
		mCacheDict = std::make_shared<XDictionaryInfo>(path.UTF8CStr());
		if ((int)mCacheDict->create() > 0) {
			mCacheDict.reset();
		}
	}
	return mCacheDict;
}
std::shared_ptr<XDictionaryInfo> XSandBox::documentDict() {
	if (!mDocDict) {
		auto path = homeDict()->getPath();
		path.append("/Documents");
		mDocDict = std::make_shared<XDictionaryInfo>(path.UTF8CStr());
		if ((int)mDocDict->create() > 0) {
			mDocDict.reset();
		}
	}
	return mDocDict;
}
std::shared_ptr<XDictionaryInfo> XSandBox::libaryDict() {
	if (!mLibaryDict) {
		auto path = homeDict()->getPath();
		path.append("/Library");
		mLibaryDict = std::make_shared<XDictionaryInfo>(path.UTF8CStr());
		if ((int)mLibaryDict->create() > 0) {
			mLibaryDict.reset();
		}
	}
	return mLibaryDict;
}
std::shared_ptr<XDictionaryInfo> XSandBox::tmpDict() {
	if (!mTmpDict) {
		auto path = homeDict()->getPath();
		path.append("tmp");
		mTmpDict = std::make_shared<XDictionaryInfo>(path.UTF8CStr());
		if ((int)mTmpDict->create() > 0) {
			mTmpDict.reset();
		}
	}
	return mTmpDict;
}


const std::shared_ptr<XBundle>& XBundle::mainBundle() {
	auto homeDict = XSandBox::sharedSandBox().appDict();
	static std::shared_ptr<XBundle> bundle = std::make_shared<XBundle>(homeDict->getPath().UTF8CStr());
	return bundle;
}
const std::shared_ptr<XBundle>& XBundle::libBundle() {
	auto path = XSandBox::sharedSandBox().appDict()->getPath();
	path.append("/XDUILIB.bundle");
	static std::shared_ptr<XBundle> bundle = std::make_shared<XBundle>(path.UTF8CStr());
	return bundle;
}

XResource::XString XBundle::pathForResource(const char *resName, const char *ext) {
	auto fileInfo = mDict->fileInfoFor(resName, ext);
	if (fileInfo) {
		return fileInfo->getPath();
	}
	return XResource::XString::emptyStr();
}
//const char* pathForResource(const char *fileName, const char *ext);
XBundle::XBundle(const char *bundlePath) {
	mDict = XDictionaryInfo::dictInfoForPath(bundlePath);
}

