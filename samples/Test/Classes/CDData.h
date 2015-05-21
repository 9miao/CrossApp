//
//  CDData.h
//  Test
//
//  Created by renhongguang on 15/4/22.
//
//

#ifndef Test_CDData_h
#define Test_CDData_h

#include "CrossApp.h"

struct newsPage
{
    std::string m_title;
    std::string m_url;
    std::string m_pic;
};

struct newsMsg
{
    std::string m_title;
    std::string m_url;
    std::vector<std::string > m_imageUrl;
    std::string m_desc;
};

struct newsImage
{
    std::string m_title;
    std::vector<std::string > m_imageUrl;
    std::vector<std::string > m_imageDesc;
};

static const char* menuList[4] =
{
    "控件展示", "应用展示", "CrossApp官网", "9秒官网"
};

static const char* menuTitle[4] =
{
    "分类", "新闻", "图片", "我的"
};

static const char* newsTitle[5] =
{
    "头条","娱乐","体育","财经","科技"
};

static const char* imageTitle[4] =
{
    "八卦","服饰","美体","彩妆"
};

static const char* menuTag[5] =
{
    "weixin", "news_entertainment", "news_sports", "news_car","news_society"
};

static const char* imageTag[4] =
{
    "gossip", "style", "body", "beauty"
};
static const char* aboutMenuTag[12] =
{
    "新用户做任务，赢大奖", "礼品中心", "离线下载", "夜间模式","头条推送","仅wi-fi下载图片","正文字号","清除缓存","检查更新","评分","反馈","关于"
};

static const char* ui_icon_01 = "image/AlertView.png";
static const char* ui_icon_02 = "image/button.png";
static const char* ui_icon_03 = "image/ImageView.png";
static const char* ui_icon_04 = "image/Scale9ImageView.png";
static const char* ui_icon_05 = "image/indicatorView.png";
static const char* ui_icon_06 = "image/Label.png";
static const char* ui_icon_07 = "image/Progress.png";
static const char* ui_icon_08 = "image/slider.png";
static const char* ui_icon_09 = "image/Switch.png";
static const char* ui_icon_10 = "image/TextField.png";
static const char* ui_icon_11 = "image/TextView.png";
static const char* ui_icon_12 = "image/SegmentedControl.png";
static const char* ui_icon_13 = "image/TabBar.png";
static const char* ui_icon_14 = "image/PageView.png";
static const char* ui_icon_15 = "image/TableView.png";
static const char* ui_icon_16 = "image/ListView.png";
static const char* ui_icon_17 = "image/WebView.png";
static const char* ui_icon_18 = "image/CollectionView.png";
static const char* ui_icon_19 = "image/ScrollView.png";

static const char* iconTag[19] = {
   ui_icon_01,ui_icon_02,ui_icon_03,ui_icon_04,ui_icon_05,ui_icon_06,ui_icon_07,ui_icon_08,ui_icon_09,ui_icon_10,ui_icon_11,ui_icon_12,ui_icon_13,ui_icon_14,ui_icon_15,ui_icon_16,ui_icon_17,ui_icon_18,ui_icon_19
};
#endif
