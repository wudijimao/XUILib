#pragma once

#include "../../include/core/XWindow.hpp"

class XWindow_android : public XWindow {
public:
    ~XWindow_android();
    XWindow_android();
    virtual void showInFront();
    bool init();
};
