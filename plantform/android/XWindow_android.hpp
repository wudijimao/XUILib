#pragma once

#include "../../include/core/XWindow.hpp"

#include "GLCanvas_android.hpp"

class XWindow_android : public XWindow {
public:
    ~XWindow_android();
    XWindow_android();
    virtual void showInFront();
    bool init(struct engine *engine);
private:
    struct engine *mEngine;
};
