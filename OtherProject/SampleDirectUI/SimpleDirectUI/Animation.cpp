#include "stdafx.h"
#include "Animation.h"

bool Animation::Bind(XControls::BaseControlPtr ctrl)
{
	mCtrl = ctrl;
	return true;
}
bool Animation::Play()
{
	return true;
}

bool Animation::Stop()
{
	return true;
}

bool Animation::Pause()
{
	return true;
}

void Animation::Tick(int ms)
{

}
