

#ifndef __CAAlertViewImpl_H__
#define __CAAlertViewImpl_H__

#include <stdio.h>
#include <string>
#include <vector>
#include <functional>
#include "ccMacros.h"
NS_CC_BEGIN

void __show_alertView(const std::string& title,
                 const std::string& message,
                 const std::vector<std::string>& buttonTitles,
                 const std::function<void(int)>& callback);

NS_CC_END

#endif    // __CAAlertViewImpl_H__
