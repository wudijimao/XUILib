#pragma once
#include "IBaseControl.h"

class IAnimation
{
public:
	virtual bool Play() = 0;
	virtual bool Stop() = 0;
	virtual bool Pause() = 0;
	virtual void Tick(int ms) = 0;
	std::function<void(XControls::IBaseControl&)> Completed;
};
