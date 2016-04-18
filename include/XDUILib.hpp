//
//  XDUILib.h
//  XDUILib
//
//  Created by ximiao on 16/3/17.
//  Copyright © 2016年 wudijimao. All rights reserved.
//  ****Important:please set main.cpp to Object-C++ Source Type in Xcode at the FilePropty; see Xcode Setting.png in Doc******
//
#pragma once
#include "core/stdafx.hpp"
#include "core/UIView.hpp"
#include "core/XApp.hpp"
#include "core/IXWindow.hpp"
#include "res/XImage/XImage.hpp"
#include "core/XResManager.hpp"
#include "core/MutiThread/XDispatch.h"
#include "http/XHTTPClient.hpp"


#ifdef TARGET_OS_IPHONE
    #include "ios/XDUILib_ios.hpp"
    #define XDUIMain int main(int argc, char * argv[])
    #define XDUI_RUN_PARAMS argc, argv
#endif

#ifdef TARGET_OS_MAC
#endif


#ifdef TARGET_OS_MSWINDOWS
    #define XDUIMain int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
    #define XDUI_RUN_PARAMS hInstance, hPrevInstance, szCmdLine, iCmdShow
#endif

