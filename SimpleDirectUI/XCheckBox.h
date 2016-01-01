#include "XButton.h"
#include "XTextBox.h"
#include "DllExport.h"

namespace XControls
{
	class SIMPLEDIRECTUI_API XCheckBox : public BaseControl
	{
	public:
		XCheckBox(const std::wstring& id, bool isChecked = false);
		XResource::XText& Text();
		bool IsChecked();
	private:
		void OnMouseUp(IBaseControl&);
		XButton mBtnChecked, mBtnUnChecked;
		XTextBox mText;
		bool mIsChecked;
	};
}