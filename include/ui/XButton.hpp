#pragma once
#include "../core/UIView.hpp"
#include "XTextView.hpp"
#include "XUIImageView.hpp"

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
    
    struct SIMPLEDIRECTUI_API XButtonResHolder {
        bool mSetted = false;
        std::shared_ptr<XResource::IXImage> mBackGroundImage;
        std::shared_ptr<XResource::IXImage> mImage;
        std::shared_ptr<XResource::XUIColor> mBackGroundColor;
        std::shared_ptr<XResource::XUIColor> mTextColor;
        std::shared_ptr<XResource::XFont> mFont;
        XResource::XString mText;
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
        
        virtual void setBackgroundImage(const std::shared_ptr<XResource::IXImage> &backgroundImage, ButtonStates state = ButtonStates::NORMAL);
        virtual void setIcon(const std::shared_ptr<XResource::IXImage> &image, ButtonStates state = ButtonStates::NORMAL);
        virtual void setBackgroundColor(const std::shared_ptr<XResource::XUIColor> &backgroundColor, ButtonStates state = ButtonStates::NORMAL);
        virtual void setTitleColor(const std::shared_ptr<XResource::XUIColor> &textColor, ButtonStates state = ButtonStates::NORMAL);
        virtual void setTitleFont(const std::shared_ptr<XResource::XFont> &font, ButtonStates state = ButtonStates::NORMAL);
        virtual void setTitle(const XResource::XString &text, ButtonStates state = ButtonStates::NORMAL);
        virtual void setTitle(const std::shared_ptr<XResource::XAttributedString> &attrText, ButtonStates state = ButtonStates::NORMAL);
        
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
        std::shared_ptr<XUIImageView> mIconView;
		ButtonStates mState = ButtonStates::DISABLE;
        ButtonStates mUsedResState = ButtonStates::DISABLE;
        XButtonResHolder mResMap[(int)ButtonStates::DISABLE +1];
	};
}

