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

class XFileInfo;
class XDictionaryInfo;

class SIMPLEDIRECTUI_API XPathInfo {
public:
	const XResource::XString& getPath();
    virtual ~XPathInfo();
	virtual bool isExist();
	virtual bool init(const char* fullPath);
protected:
	void setIsExist(bool isExist);
    bool mHasCheckExist = false;
    bool mIsExist = false;
    XResource::XString mPath;
};

enum class XPathCrateResault {
    Success = 1,
    Exists = 2,
    NoPermission = -1,
};

class SIMPLEDIRECTUI_API XDictionaryInfo : public XPathInfo {
public:
	XDictionaryInfo(const char* fullPath);
#ifdef TARGET_OS_MSWINDOWS
	virtual bool isExist() override;
#endif // TARGET_OS_MSWINDOWS
	virtual bool init(const char* fullPath) override;
	const std::vector<std::shared_ptr<XFileInfo>> allFilesInfo();
	const std::vector<std::shared_ptr<XDictionaryInfo>> allDictionarysInfo();
    std::shared_ptr<XFileInfo> fileInfoFor(const char *fullFileName);
    //if not found return nullptr
	std::shared_ptr<XFileInfo> fileInfoFor(const char *name, const char *ext);
	std::vector<std::shared_ptr<XFileInfo>> searchFilesInfoFor(const char *ext);
    
	XPathCrateResault create();
	static std::shared_ptr<XDictionaryInfo> dictInfoForPath(const char *fullPath);
protected:
	bool readChildren();
private:
    bool mHasReadChildren = false;
    //not free
    static std::map<std::string, std::shared_ptr<XDictionaryInfo>> sDictMap;
    std::vector<std::shared_ptr<XFileInfo>> mFiles;
    std::vector<std::shared_ptr<XDictionaryInfo>> mDicts;
};

class SIMPLEDIRECTUI_API XFileInfo : public XPathInfo {
public:
	static std::shared_ptr<XFileInfo> fileInfoForPath(const char *fullPath);

#ifdef TARGET_OS_MSWINDOWS
	virtual bool isExist() override;
#endif // TARGET_OS_MSWINDOWS
    
	XFileInfo(const char *path);
	virtual bool init(const char *path) override;
	const XResource::XString& ext();
	const XResource::XString& name();
	const XResource::XString& fullName();
	const XResource::XString& dictPath();
private:
    bool mCanRead;
    bool mCanWrite;
    XResource::XString mDictPath;
    XResource::XString mFullName;
    XResource::XString mName;
    XResource::XString mExt;
    //not free
    static std::map<std::string, std::shared_ptr<XFileInfo>> sFilesMap;
};

class SIMPLEDIRECTUI_API XSandBox {
public:
	static inline XSandBox &sharedSandBox();
    //app Install Dict
    std::shared_ptr<XDictionaryInfo> appDict();
    std::shared_ptr<XDictionaryInfo> homeDict();
	std::shared_ptr<XDictionaryInfo> cacheDict();
	std::shared_ptr<XDictionaryInfo> documentDict();
	std::shared_ptr<XDictionaryInfo> libaryDict();
	std::shared_ptr<XDictionaryInfo> tmpDict();
protected:
    std::shared_ptr<XDictionaryInfo> mAppDict;
    std::shared_ptr<XDictionaryInfo> mHomeDict;
private:
    std::shared_ptr<XDictionaryInfo> mCacheDict;
    std::shared_ptr<XDictionaryInfo> mDocDict;
    std::shared_ptr<XDictionaryInfo> mLibaryDict;
    std::shared_ptr<XDictionaryInfo> mTmpDict;
};

//ResManager manager the res, image file depence on device screen size(@2x  @3x)
class SIMPLEDIRECTUI_API XBundle {
public:
	static const std::shared_ptr<XBundle>& mainBundle();
	static const std::shared_ptr<XBundle>& libBundle();
	XResource::XString pathForResource(const char *resName);
	XResource::XString pathForResource(const char *resName, const char *ext);
    //const char* pathForResource(const char *fileName, const char *ext);
	XBundle(const char *bundlePath);
private:
    std::shared_ptr<XDictionaryInfo> mDict;
};
