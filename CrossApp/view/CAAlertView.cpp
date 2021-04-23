
#include "CAAlertView.h"
#include "platform/CAAlertViewImpl.h"
NS_CC_BEGIN

static CAMap<unsigned int, CAAlertView*> s_gMessageBoxs;

CAAlertView* CAAlertView::create(const std::string& title, const std::string& message)
{
    CAAlertView* box = new CAAlertView(title, message);
    box->autorelease();
    return box;
}

CAAlertView* CAAlertView::create(const std::string& title, const std::string& message, const std::vector<std::string>& buttonTitles)
{
    CAAlertView* box = new CAAlertView(title, message);
    box->m_vButtonTitles = buttonTitles;
    box->autorelease();
    return box;
}

CAAlertView::CAAlertView(const std::string& title, const std::string& message)
:m_sTitle(title)
,m_sMessage(message)
,m_callback(nullptr)
{
    
}

void CAAlertView::setButtonTitles(const std::vector<std::string>& buttonTitles)
{
    m_vButtonTitles = buttonTitles;
}

void CAAlertView::addButtonTitle(const std::string& buttonTitle)
{
    m_vButtonTitles.push_back(buttonTitle);
}

void CAAlertView::show()
{
    s_gMessageBoxs.insert(m_u__ID, this);
    std::vector<std::string> buttonTitles = m_vButtonTitles;
    if (buttonTitles.empty())
    {
        buttonTitles.push_back("OK");
    }
    __show_alertView(m_sTitle, m_sMessage, buttonTitles, [&](int index)
    {
        s_gMessageBoxs.erase(m_u__ID);
    });
    
}
void CAAlertView::show(const std::function<void(int)>& callback)
{
    if (callback) m_callback = callback;
    s_gMessageBoxs.insert(m_u__ID, this);
    
    std::vector<std::string> buttonTitles = m_vButtonTitles;
    if (buttonTitles.empty())
    {
        buttonTitles.push_back("OK");
    }
    __show_alertView(m_sTitle, m_sMessage, buttonTitles, [&](int index)
    {
        if (m_callback)
        {
            m_callback(index);
        }
        s_gMessageBoxs.erase(m_u__ID);
    });
    
}

NS_CC_END
