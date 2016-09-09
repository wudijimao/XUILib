//
// Created by ximiao on 16/7/8.
//

#include "XWindow_android.hpp"

std::shared_ptr<IXWindow> IXWindow::createWindow() {
    return std::make_shared<XWindow_android>();
}

XWindow_android::~XWindow_android() {

}
XWindow_android::XWindow_android() {

}
void XWindow_android::showInFront() {

}
bool XWindow_android::init(struct engine *engine) {
    auto canvas = std::make_shared<GLCanvas_android>();
    canvas->init(engine);
    _canvas = canvas;
    initFinished();
    this->update();
    return true;
}