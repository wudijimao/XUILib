#include "XNode.h"

class XRoot
{
public:
	XRoot();
	~XRoot();
	std::string Name;
	float Width;
	float Height;
private:
	PNode mControls;
};

XRoot::XRoot() :Width(0.0f), Height(0.0f)
{
}

XRoot::~XRoot()
{
}