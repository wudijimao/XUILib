//
//  XResManager.hpp
//  XDUILib
//
//  Created by ximiao on 16/3/27.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
#pragma once

#include "stdafx.hpp"
#include <sys/stat.h>
#include <sys/types.h>  //TODO::put into .cpp
#include <dirent.h>
#include "../res/XString.hpp"

class XFileInfo;
class XDictionaryInfo;

class XPathInfo {
public:
    const XResource::XString& getPath() {
        return mPath;
    }
    virtual ~XPathInfo(){};
    virtual bool isExist() {
        if (!mHasCheckExist) {
            int a = access(mPath.getUTF8String().c_str(), 1);
            mIsExist = (a == 0);
        }
        return mIsExist;
    }
    virtual bool init(const char* fullPath) {
        mPath = fullPath;
        mPath.replaceAll("\\", "/");
        mHasCheckExist = false;
        return true;
    }
protected:
    void setIsExist(bool isExist) {
        mIsExist = isExist;
        mHasCheckExist = true;
    }
    bool mHasCheckExist = false;
    bool mIsExist = false;
    XResource::XString mPath;
};

enum class XPathCrateResault {
    Success = 1,
    Exists = 2,
    NoPermission = -1,
};

class XDictionaryInfo : public XPathInfo {
public:
    XDictionaryInfo(const char* fullPath) {
        init(fullPath);
    }
    virtual bool init(const char* fullPath) override {
        if(XPathInfo::init(fullPath)) {
            mHasReadChildren = false;
            mFiles.clear();
            mDicts.clear();
        }
        return false;
    }
    const std::vector<const std::shared_ptr<XFileInfo>> allFilesInfo() {
        return mFiles;
    }
    const std::vector<const std::shared_ptr<XDictionaryInfo>> allDictionarysInfo() {
        return mDicts;
    }
    std::shared_ptr<XFileInfo> fileInfoFor(const char *fullFileName);
    //if not found return nullptr
    std::shared_ptr<XFileInfo> fileInfoFor(const char *name, const char *ext) {
        if (name != nullptr && ext != nullptr) {
            std::string str(name);
            str.append(".");
            str.append(ext);
            return fileInfoFor(str.c_str());
        }
        return nullptr;
    }
    std::vector<const std::shared_ptr<XFileInfo>> searchFilesInfoFor(const char *ext) {
        std::vector<const std::shared_ptr<XFileInfo>> files;
        if (ext != nullptr) {
            for (auto file : mFiles) {
                
            }
        }
        return files;
    }
    
    XPathCrateResault create() {
        if (isExist()) {
            return XPathCrateResault::Exists;
        }
        int status = mkdir(mPath.getUTF8String().c_str(), 1);
        if (status == 1) {
            return XPathCrateResault::Success;
        }
        return XPathCrateResault::NoPermission;
    }
    static std::shared_ptr<XDictionaryInfo> dictInfoForPath(const char *fullPath) {
        auto iter = sDictMap.find(fullPath);
        if (iter != sDictMap.end()) {
            return iter->second;
        } else {
            auto dictInfo = std::make_shared<XDictionaryInfo>(fullPath);;
            sDictMap[fullPath] = dictInfo;
            return dictInfo;
        }
    }
protected:
    
    bool readChildren() {
        if (mHasReadChildren) {
            return true;
        }
        if (isExist()) {
            const char *path = mPath.UTF8CStr();
            DIR *pDir = opendir(path);
            if ((pDir ) == NULL)
            {
                return false;
            }
            struct dirent* ent = NULL;
            char dir[1024];
            struct stat statbuf;
            while ((ent = readdir(pDir)) != NULL)
            { //统计当前文件夹下有多少文件（不包括文件夹）
                //得到读取文件的绝对路径名
                snprintf(dir, 512, "%s/%s", path, ent->d_name);
                //得到文件信息
                lstat(dir, &statbuf);
                if (S_ISDIR(statbuf.st_mode))
                {
                    mDicts.push_back(std::make_shared<XDictionaryInfo>(dir));
                } else {
                    mFiles.push_back(std::make_shared<XFileInfo>(dir));
                }
            } //while
            closedir(pDir);
            mHasReadChildren = true;
            return true;
        }
        return false;
    }
private:
    bool mHasReadChildren = false;
    //not free
    static std::map<std::string, std::shared_ptr<XDictionaryInfo>> sDictMap;
    std::vector<const std::shared_ptr<XFileInfo>> mFiles;
    std::vector<const std::shared_ptr<XDictionaryInfo>> mDicts;
};

class XFileInfo : public XPathInfo {
public:
    static std::shared_ptr<XFileInfo> fileInfoForPath(const char *fullPath) {
        auto iter = sFilesMap.find(fullPath);
        if (iter != sFilesMap.end()) {
            return iter->second;
        } else {
            auto fileInfo = std::make_shared<XFileInfo>(fullPath);
            sFilesMap[fullPath] = fileInfo;
            return fileInfo;
        }
    }
    
    XFileInfo(const char *path) {
        init(path);
    }
    virtual bool init(const char *path) override {
        if(XPathInfo::init(path)) {
            unsigned long pos =  mPath.find("/", XResource::XStringCompareOptions::SearchFromBackward);
            if(pos > 0) {
                mDictPath = mPath.subStr(0, pos);
                mFullName = mPath.subStr(pos, mPath.length() - pos);
                pos = mFullName.find(".", XResource::XStringCompareOptions::SearchFromBackward);
                if(pos > 0) {
                    mName = mFullName.subStr(0, pos);
                    mExt = mFullName.subStr(pos, mFullName.length() - pos);
                } else {
                    mName = mFullName;
                    mExt = XResource::XString::emptyStr();
                }
            }
            return true;
        }
        return false;
    }
    const XResource::XString& ext() {
        return mExt;
    }
    const XResource::XString& name() {
        return mName;
    }
    const XResource::XString& fullName() {
        return mFullName;
    }
    const XResource::XString& dictPath() {
        return mDictPath;
    }
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

class XSandBox {
public:
    static inline XSandBox &sharedSandBox() {
        static XSandBox sandBox;
        return sandBox;
    }
    //app Install Dict
    std::shared_ptr<XDictionaryInfo> appDict();
    std::shared_ptr<XDictionaryInfo> homeDict();
    std::shared_ptr<XDictionaryInfo> cacheDict() {
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
    std::shared_ptr<XDictionaryInfo> documentDict() {
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
    std::shared_ptr<XDictionaryInfo> libaryDict() {
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
    std::shared_ptr<XDictionaryInfo> tmpDict() {
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
class XBundle {
public:
    static const std::shared_ptr<XBundle>& mainBundle() {
        auto homeDict = XSandBox::sharedSandBox().appDict();
        static std::shared_ptr<XBundle> bundle = std::make_shared<XBundle>(homeDict->getPath().UTF8CStr());
        return bundle;
    }
    static const std::shared_ptr<XBundle>& libBundle() {
        auto path = XSandBox::sharedSandBox().appDict()->getPath();
        path.append("/XDUILIB.bundle");
        static std::shared_ptr<XBundle> bundle = std::make_shared<XBundle>(path.UTF8CStr());
        return bundle;
    }
    
    XResource::XString pathForResource(const char *resName, const char *ext) {
        auto fileInfo = mDict->fileInfoFor(resName, ext);
        if (fileInfo) {
            return fileInfo->getPath();
        }
        return XResource::XString::emptyStr();
    }
    //const char* pathForResource(const char *fileName, const char *ext);
    XBundle(const char *bundlePath) {
        mDict = XDictionaryInfo::dictInfoForPath(bundlePath);
    }
private:
    
    std::shared_ptr<XDictionaryInfo> mDict;
};
