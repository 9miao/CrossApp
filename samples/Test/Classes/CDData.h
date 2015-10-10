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
	std::string m_imageUrl;
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
	"__all__", "news_entertainment", "news_fashion", "news_car", "news_society"
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

static const CHAR* adressTag[34] =
{
    _T("北京市"),
    _T("天津市"),
    _T("上海市"),
    _T("重庆市"),
    _T("河北省"),
    _T("山西省"),
    _T("辽宁省"),
    _T("吉林省"),
    _T("黑龙江省"),
    _T("江苏省"),
    _T("浙江省"),
    _T("安徽省"),
    _T("福建省"),
    _T("江西省"),
    _T("山东省"),
    _T("河南省"),
    _T("湖北省"),
    _T("湖南省"),
    _T("广东省"),
    _T("海南省"),
    _T("四川省"),
    _T("贵州省"),
    _T("云南省"),
    _T("陕西省"),
    _T("甘肃省"),
    _T("青海省"),
    _T("台湾省"),
    _T("广西壮族自治区"),
    _T("内蒙古自治区"),
    _T("西藏自治区"),
    _T("宁夏回族自治区"),
    _T("新疆维吾尔自治区"),
    _T("香港特别行政区"),
    _T("澳门特别行政区"),
};

static const char* iconTag[26] =
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
    "image/CollectionView.png",
    "image/ScrollView.png",
    "image/WebView.png",
    "image/FlashView.png",
    "image/GifView.png",
    "image/PickerView.png",
    "image/Stepper.png",
    "image/RenderImage.png",
    "image/Video.png",
    "image/Video.png"
};