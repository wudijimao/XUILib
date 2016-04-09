//
//  XResManager.cpp
//  XDUILib
//
//  Created by ximiao on 16/3/27.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#include "XResManager.hpp"


std::map<std::string, std::shared_ptr<XDictionaryInfo>>  XDictionaryInfo::sDictMap;

std::map<std::string, std::shared_ptr<XFileInfo>> XFileInfo::sFilesMap;


std::shared_ptr<XFileInfo> XDictionaryInfo::fileInfoFor(const char *fullFileName) {
    if (fullFileName != nullptr) {
        readChildren();
        for (auto file : mFiles) {
            if (file->fullName().isEqualTo(fullFileName)) {
                return file;
            }
        }
    }
    return nullptr;
}