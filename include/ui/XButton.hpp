#pragma once
#include "../core/UIView.hpp"
#include "XTextView.hpp"

namespace XUI
{
	enum class ButtonStates
	{
		NORMAL = 0,
        HOVER,
		DOWN,
		SELECTED,
        SELECTED_HOVER,
        SELECTED_DOWN,
		DISABLE,
	};
    
    struct XButtonResHolder {
        bool mSetted = false;
        std::shared_ptr<XResource::IXImage> mBackGroundImage;
        std::shared_ptr<XResource::IXImage> mImage;
        std::shared_ptr<XResource::XUIColor> mBackGroundColor;
        std::shared_ptr<XResource::XUIColor> mTextColor;
        std::shared_ptr<XResource::XFont> mFont;
        std::shared_ptr<XResource::XString> mText;
        std::shared_ptr<XResource::XAttributedString> mAttrText;
        void copyFrom(const XButtonResHolder &res);
    };
    
	class SIMPLEDIRECTUI_API XButton : public UIView
	{
	public:
        XButton();
        ~XButton(){};
        virtual void onTouch(const std::vector<std::shared_ptr<XTouch>> &touch) override;
        virtual void onMouseEvent(const std::vector<std::shared_ptr<XMouse>> &mouseEvent) override;
        void select(bool bSel);
        bool isSelected();
        void setEnable(bool enable);
        bool isEnable();
        
        void setBackgroundImage(ButtonStates state, const std::shared_ptr<XResource::IXImage> &backgroundImage);
        void setIcon(ButtonStates state, const std::shared_ptr<XResource::IXImage> &image);
        void setBackgroundColor(ButtonStates state, const std::shared_ptr<XResource::XUIColor> &backgroundColor);
        void setTitleColor(ButtonStates state, const std::shared_ptr<XResource::XUIColor> &textColor);
        void setTitleFont(ButtonStates state, const std::shared_ptr<XResource::XFont> &font);
        void setTitle(ButtonStates state, const std::shared_ptr<XResource::XString> &text);
        void setTitle(ButtonStates state, const std::shared_ptr<XResource::XAttributedString> &attrText);
        
        virtual void layoutSubViews() override;
	private:
        bool mIsSelected = false;
        bool mIsEnable = true;
        void _setState(ButtonStates state);
        void _refreshStatus(); 
        void _down();
        void _up();
        bool mIsPushingDown = false;
        void _over();
        void _out();
        bool mIsOvering = false; // is mouse overing
        ButtonStates _getResExistState(ButtonStates state);
        XButtonResHolder * _getRes(ButtonStates state);
        std::shared_ptr<UITextView> mTextView;
		ButtonStates mState = ButtonStates::DISABLE;
        ButtonStates mUsedResState = ButtonStates::DISABLE;
        XButtonResHolder mResMap[(int)ButtonStates::DISABLE +1];
	};
}

