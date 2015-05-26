#include "CommonHttpManager.h"

#define _T(x) L##x
#define CHAR    wchar_t
#define UTF8EX(x) unicode_to_utf8( x ).c_str()

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

static const CHAR* menuList[4] =
{
	_T("控件展示"), _T("应用展示"), _T("CrossApp官网"), _T("9秒官网")
};

static const CHAR* menuTitle[4] =
{
	_T("分类"), _T("新闻"), _T("图片"), _T("我的")
};

static const CHAR* newsTitle[5] =
{
	_T("头条"), _T("娱乐"), _T("体育"), _T("财经"), _T("科技")
};

static const CHAR* imageTitle[4] =
{
	_T("八卦"), _T("服饰"), _T("美体"), _T("彩妆")
};

static const char* menuTag[5] =
{
	"weixin", "news_entertainment", "news_sports", "news_car", "news_society"
};

static const char* imageTag[4] =
{
	"gossip", "style", "body", "beauty"
};

static const CHAR* aboutMenuTag[12] =
{
	_T("新用户做任务，赢大奖"),
    _T("礼品中心"),
    _T("离线下载"),
    _T("夜间模式"),
    _T("头条推送"),
    _T("仅wifi下载图片"),
    _T("正文字号"),
    _T("清除缓存"),
    _T("检查更新"),
    _T("评分"),
    _T("反馈"),
    _T("关于")
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

static const char* iconTag[19] = 
{
	ui_icon_01,
    ui_icon_02,
    ui_icon_03,
    ui_icon_04,
    ui_icon_05,
    ui_icon_06,
    ui_icon_07,
    ui_icon_08,
    ui_icon_09,
    ui_icon_10,
    ui_icon_11,
    ui_icon_12,
    ui_icon_13,
    ui_icon_14,
    ui_icon_15,
    ui_icon_16,
    ui_icon_17,
    ui_icon_18,
    ui_icon_19
};