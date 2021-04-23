

#include "../CAAlertViewImpl.h"
#include "basics/CAApplication.h"

NS_CC_BEGIN




void __show_alertView(const std::string& title, const std::string& message, const std::vector<std::string>& buttonTitles, const std::function<void(int)>& callback)
{
	MessageBoxA(nullptr, message.c_str(), title.c_str(), MB_OK);
}

NS_CC_END
