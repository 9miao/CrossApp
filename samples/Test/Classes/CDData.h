#include "CommonHttpManager.h"

#define _T(x) L##x
#define CHAR    wchar_t

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

static const char* iconTag[20] =
{
	"image/AlertView.png",
    "image/button.png",
    "image/ImageView.png",
    "image/Scale9ImageView.png",
    "image/indicatorView.png",
    "image/Label.png",
    "image/Progress.png",
    "image/slider.png",
    "image/Switch.png",
    "image/TextField.png",
    "image/TextView.png",
    "image/SegmentedControl.png",
    "image/TabBar.png",
    "image/PageView.png",
    "image/TableView.png",
    "image/ListView.png",
    "image/WebView.png",
    "image/CollectionView.png",
    "image/ScrollView.png",
    "image/FlashView.png"
};