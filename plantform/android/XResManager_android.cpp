//
// Created by ximiao on 16/7/12.
//

#include "../../include/core/XResManager.hpp"


//#include <direct.h>


XPathCrateResault XDictionaryInfo::create() {
    if (isExist()) {
        return XPathCrateResault::Exists;
    }
   // int status = _mkdir(mPath.getUTF8String().c_str());
    //if (status == 1) {
    //    return XPathCrateResault::Success;
    //}
    return XPathCrateResault::NoPermission;
}

std::shared_ptr<XDictionaryInfo> XSandBox::appDict() {
    if (!mAppDict)
    {
        char buf[1024];
        //_getcwd(buf, sizeof(buf));
        std::string str = std::string("App").c_str();
        mAppDict.reset(new XDictionaryInfo(str.c_str()));
    }
    return mAppDict;
}
std::shared_ptr<XDictionaryInfo> XSandBox::homeDict() {
    if (!mHomeDict)
    {
        //char buf[1024];
        //_getcwd(buf, sizeof(buf));
        std::string path = "SandBox";
        path.append("/Home");
        mHomeDict.reset(new XDictionaryInfo(path.c_str()));
    }
    return mHomeDict;
}
