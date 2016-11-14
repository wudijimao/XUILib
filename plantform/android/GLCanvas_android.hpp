#pragma once

#include "../../include/core/GLCanvas.hpp"
#include "XAndroidEnv.hpp"


class SIMPLEDIRECTUI_API_DEBUG GLCanvas_android : public GLCanvas {
public:
    ~GLCanvas_android();
    GLCanvas_android();
    bool init(struct engine *engine);
    virtual bool Present() override;

private:
    struct engine *mEngine;
};
