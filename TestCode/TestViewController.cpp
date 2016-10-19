//
//  TestViewController.cpp
//  TestUI
//
//  Created by ximiao on 16/6/24.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#include "TestViewController.hpp"
#include "LoginViewController.hpp"

//class TestData : public XResponseData {
//public:
//    std::string name;
//    virtual void parse(const rapidjson::Document &json) override {
//        name = json["data"]["cityname"].GetStringSafe();
//    }
//};
//
//class TestRequest : public XHTTPRequestT<TestData> {
//public:
//    TestRequest(){
//        url = "http://movie.weibo.com/movieapp/rank/weibohot";
//        //url = "http://www.baidu.com/";
//        this->method = XHTTPRequestMethod::XHTTPRequestMethod_Post;
//        this->_params.addParam("uid", "5633119670");
//        this->_params.addParam("token", "2.00B5msBDcX6kGD82b79248caxjF5WD");
//        this->_params.addParam("你好", "测试中文");
//        //this->_params.setParams("uid=5633119670&lfid=&lon=&count=2147483647&os_v=4.3&wm=44994_0001&luicode=&from=8610705010&os_n=Android&cityid=&ip=10.0.3.15&d_id=D0BE2BB07273F93&v=1.0.7&num=1&page=1&token=2.00B5msBDcX6kGD82b79248caxjF5WD&a_n=MovieSDK&d_n=Google+Nexus+4+-+4.3+-+API+18+-+768x1280&lat=&");
//        std::string str = this->_params.getStr();
//
//        int a = 0;
//    }
//};



    ViewController::~ViewController() {
    }
    void ViewController::viewDidLoad() {
        XUI::UIViewController::viewDidLoad();

//        auto jpgImg = XResource::XImage::imageNamed("test.jpg");



        auto scrollView = std::make_shared<XUI::ScrollView>();
        scrollView->setBkgColor(XResource::XUIColor::pinkColor());
        scrollView->setRect(XResource::XRectPro(20, 100, 100, 200));
        this->view()->addSubView(scrollView);

        auto textView = std::make_shared<XUI::UITextView>();
        textView->setText("泉此方测试测试 test哈哈哈啊啊啊");
        textView->setTextColor(XResource::XUIColor::blueColor());
        textView->setFont(XResource::XFont::systemFont(16));
        auto color = XResource::XUIColor::pinkColor()->copy();
        color->_color.a = 200;
        textView->setBkgColor(color);
        textView->setRect(XResource::XRectPro(0, 0, 100, 320));
        auto img = XResource::XImage::imageNamed("test.png");
        textView->setBkgImg(img);
        scrollView->setContentView(textView);

        auto btn = std::make_shared<XUI::XButton>();
        btn->setTitle("登陆页");
        btn->setBackgroundColor(XResource::XUIColor::blueColor(), XUI::ButtonStates::DOWN);
        btn->setRect(XResource::XRectPro(205, 20, 70, 25));
        this->view()->addSubView(btn);
        btn->onClick = [this](XUI::XButton&){
//            _testSubView = std::make_shared<XUI::XView>();
//            _testSubView->setBkgColor(XResource::XUIColor::redColor());
//            _testSubView->setRect(XResource::XRectPro(20, 20, 320, 120));
//            _testSubView->setBkgImg(XResource::XImage::imageNamed("test.jpg"));
//            this->view()->addSubView(_testSubView);
            this->presentViewController(std::make_shared<LoginViewController>(), XUI::PresentAnimation::Present);
        };


//        auto request = std::make_shared<TestRequest>();
//        request->onFinish = [&](TestRequest::ResponseTypePtr response) {
//            auto header = response->header();
//            std::string str = header->getStr();
//            auto image = XResource::XImage::imageFromData(response->contentData());
        //_testSubView->setBkgImg(image);
        //this->view()->addSubView(_testSubView);
//        };
//        IXHTTPClient::getSharedInstanc()->sendRequest(request);


        rapidjson::Document doc;
        std::string str = "aaa";
        if(!doc.Parse("{\"a\":\"1\", \"b\":\"2\", \"c\":\"3\"}").HasParseError()) {
            str = doc["a"].GetString();
            textView->setText(str.c_str());
            int a = 0;
        }

//        auto imgView = std::make_shared<XUI::XUIImageView>();
//        imgView->setRect(XResource::XRectPro(100, 300, 200, 200));
//        imgView->setImage(jpgImg);
//        this->view()->addSubView(imgView);
//        imgView->setScaleType(XUI::XUIImageScaleType::AspectFill);
//        imgView->setBkgColor(XResource::XUIColor::grayColor());


        //auto vc = std::make_shared<LoginViewController>();
        //this->presentViewController(vc);
        //        clock_t t1 = clock();
        //        for (int i = 0; i < 4; ++i) {
        //            XDispatch::dispatchAsnyc(XDispatch::getGlobleQueue(XDispatch::XTaskPriority::XTaskPriority_Default), [this, i, t1](){
        //                for (int j = 0; j < 100000; ++j) {
        //                    XDispatch::dispatchAfter(XDispatch::getMainQueue(), [this, i, j, t1](){
        //                        ++mNum;
        //                        XLog::sharedInstance().log(XLogLevel::Debug, "%d:%d___%d\r\n", i, j, mNum);
        //                        if (mNum == 400000) {
        //                            clock_t t2 = clock();
        //                            std::cout << "count:" << mNum << "   time:" << (t2 - t1) / (CLOCKS_PER_SEC / 1000.0) << "ms" << std::endl;
        //                        }
        //                    }, 100);
        //                }
        //            });
        //        }

        //        for (int i = 0; i < 400000; ++i) {
        //            ++mNum;
        //            XLog::sharedInstance().log(XLogLevel::Debug, "%d:%d___%d\r\n", i, i, mNum);
        //            if (mNum == 400000) {
        //                clock_t t2 = clock();
        //                std::cout << "count:" << mNum << "   time:" << (t2 - t1) / (CLOCKS_PER_SEC / 1000.0) << "ms" << std::endl;
        //            }
        //        }

        //        __block ViewController *bThis = this;
        //        for (int i = 0; i < 4; ++i) {
        //            dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        //                for (int j = 0; j < 100000; ++j) {
        //                    //XLog::sharedInstance().log(XLogLevel::Debug, "%d:%d\r\n", i, j);
        //                    void(bl)(void) = ^{
        //                        ++bThis->mNum;
        //                        //XLog::sharedInstance().log(XLogLevel::Debug, "%d:%d___%d\r\n", i, j, bThis->mNum);
        //                        if (bThis->mNum == 400000) {
        //                            clock_t t2 = clock();
        //                            std::cout << "count:" << mNum << "   time:" << (t2 - t1) / (CLOCKS_PER_SEC / 1000.0) << "ms" << std::endl;
        //                        }
        //                    }
        //                    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(100 * NSEC_PER_MSEC)), dispatch_get_main_queue(), );
        //                }
        //            });
        //        }

    }
    void ViewController::onTouch(const std::vector<std::shared_ptr<XTouch>> &touch) {
        //(*touch.begin())->_belongView->setBkgColor(XResource::XUIColor::greenColor());
    }
    void ViewController::onMouseEvent(const std::vector<std::shared_ptr<XMouse>> &mouseEvents) {
        auto event = (*mouseEvents.begin());
        if (event->eventType == MouseEventType::Up)
        {
            if (event->eventButton == MouseEventButton::Left) {
                event->_belongView->setBkgColor(XResource::XUIColor::grayColor());
            } else {
                event->_belongView->setBkgColor(XResource::XUIColor::redColor());
            }
        }
    }
