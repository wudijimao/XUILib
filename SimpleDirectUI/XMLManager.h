#pragma once
#include "DllExport.h"
#include "IBaseWindow.h"
#include "tinyxml2.h"
#include <string>
#include <stdio.h>
#include "XRect.h"


class SIMPLEDIRECTUI_API XMLManager
{
public:
	XMLManager();
	~XMLManager();
	bool LoadXML(const std::wstring& fileName)
	{
		FILE *file = _wfopen(fileName.c_str(), L"r" );
		mXML.LoadFile(file);
		fclose(file);
		return true;
	}
	bool SaveXML(const std::wstring& fileName)
	{
		FILE *file = _wfopen(fileName.c_str(), L"w");
		mXML.SaveFile(file);
		fclose(file);
		return true;
	}
private:
	bool CreateBaseXML()
	{
		mXML.Clear();
		auto node = mXML.NewElement("Window");
		mXML.InsertFirstChild(node);
		node->SetAttribute("ID", L"");
		return true;
	}
	void SetAttriblte(tinyxml2::XMLElement& node, const XResource::XRect& rect)
	{
		node.SetAttribute("Width", rect.Width());
		node.SetAttribute("Height", rect.Height());
		node.SetAttribute("X", rect.X());
		node.SetAttribute("Y", rect.Y());
	}
	
	bool CheckXML()
	{
		return true;
	}
	tinyxml2::XMLDocument mXML;
	std::shared_ptr<IXWindow> mRoot;
};