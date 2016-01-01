#pragma once

#include "../View/View.h"

class ViewControler
{
public:
	ViewControler();
	~ViewControler();
	View& view();
	//need over write
	void willDisplayWithAni(bool ani);
	void viewDidLoad();
private:
	View *mView;
};

ViewControler::ViewControler() : mView(0)
{
}

ViewControler::~ViewControler()
{
	delete this->mView;
	this->mView = 0;
}

View& ViewControler::view(){
	if (this->mView == 0)
	{
		this->mView = new View();
		this->viewDidLoad();
	}
	return *(this->mView);
}