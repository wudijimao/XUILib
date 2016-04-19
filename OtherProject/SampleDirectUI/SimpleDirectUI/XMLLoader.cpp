#include "stdafx.h"
#include "XMLLoader.h"
#include "XControls.h"
#include "tinyxml2.h"
#include <sstream>
#include <locale>
#include <codecvt>

using namespace std;
using namespace XControls;

tinyxml2::XMLDocument mXML;

IBaseControl *CreateInstance(const char* name, const char* id)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>>cvt_utf8;//
	wstring sId = cvt_utf8.from_bytes(id);
	if (strcmp(name, "BaseControl") == 0)
	{
		return new BaseControl(sId);
	}
	else if (strcmp(name, "XButton") == 0)
	{
		return new XButton(sId);
	}
}

void XMLLoader::·ÖÎö()
{
	auto root = mXML.RootElement();
	if (strcmp(root->Value(), "Window") == 0)
	{
		XResource::XRect rect;
		auto width = root->Attribute("Width");
		int iWidth = atoi(width);
		auto height = root->Attribute("Height");
		int iHeight = atoi(height);
		rect.X(0);
		rect.Y(0);
		rect.Width(iWidth);
		rect.Height(iHeight);
		mWindow = new XWindow(rect);
		auto child = root->FirstChildElement();
		if (child)
		{
			child->QueryIntAttribute("Width", &iWidth);
			child->QueryIntAttribute("Height", &iHeight);
			auto id = child->Attribute("ID");
			//auto control = CreateInstance(child->Name(), id);
			mWindow->Contrls().GetRoot()->Rect(0, 0, iWidth, iHeight);
			mWindow->Contrls().GetRoot()->BkgColor(255, 0, 255, 0);
			child = child->FirstChildElement();
			if (child)
			{
				child->QueryIntAttribute("Width", &iWidth);
				child->QueryIntAttribute("Height", &iHeight);
				auto id = child->Attribute("ID");
				auto control1 = CreateInstance(child->Name(), id);
				mWindow->Contrls().GetRoot()->AddChild(control1);
				control1->DragWindowEnable(true);
				control1->Rect(0, 0, iWidth, iHeight);
				control1->BkgColor(255, 0, 0, 255);
				control1->Rect(0, 0, iWidth, iHeight);
			}
		}
	}
}

bool XMLLoader::Load(const char* fileName)
{
	FILE *file = fopen(fileName, "rb");
	if (file)
	{
		auto err = mXML.LoadFile(file);
		if (err == tinyxml2::XMLError::XML_NO_ERROR)
		{
			·ÖÎö();
		}
		fclose(file);
		return true;
	}
	return false;
}
