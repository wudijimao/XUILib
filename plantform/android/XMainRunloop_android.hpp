//
// Created by ximiao on 16/7/12.
//

#pragma once

#include <core/MutiThread/XRunLoop.hpp>
#include "XAndroidEnv.hpp"

void initMainRunloop(struct android_app *state);

void doMainRunloop();