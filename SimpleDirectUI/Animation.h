#pragma once
#include "IAnimation.h"
#include "IBaseWindow.h"

typedef void(*SetFun)(double);
typedef double(*GetFun)();

struct AnimateInfo
{
	double mBeginVal, mEndVal;
	SetFun Set;
	GetFun Get;
};
class Animation : public IAnimation
{
public:
	bool Bind(XControls::BaseControlPtr ctrl);
	virtual bool Play();
	virtual bool Stop();
	virtual bool Pause();
	boost::signals2::signal<void(XControls::IBaseControl&)> Completed;
protected:
	void Tick(int ms);
private:
	XControls::BaseControlPtr mCtrl;
};
