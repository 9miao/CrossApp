
//  Copyright (c) 2014 http://9miao.com All rights reserved.

#include "cocos2d.h"
#include "ExtensionMacros.h"
#include <iostream>
#include <string>
#include <cctype>
#include <map>
#include <fstream>
#include "cocoa/CCObject.h"

#include "curl/curl.h"

NS_CC_EXT_BEGIN

#ifndef MY_CAHttpClient
#define MY_CAHttpClient

size_t lcbak(char *ptr,size_t size,size_t nmemb,void *userdata);
size_t dcbak(char *ptr,size_t size,size_t nmemb,void *userdata);

class CAHttpClient: public CCObject
{
private:
	CURL *curl;
	bool post;
	bool curl_already_init;
//	std::forward_list<std::string> cookie_list;
	std::string cookie;
	std::string postfields;
	std::string url;
	bool downloadbit;
	std::string httpret;
	FILE *f;
	std::map<std::string,std::string> postdata;
	void join(void);

public :
	long rescode,conncode;
    
	CAHttpClient();
	virtual ~CAHttpClient();

	CURLcode curle ;
	void fieldclean(void) ;
	void fieldset(const std::string k,const long v) ;
	void fieldset(const std::string k,const double v) ;
	void fieldset(const std::string k,const std::string v) ;
	void fieldunset(const std::string k) ;
	/*void progressbar(void *ptr,int (*func)(void *ptr,
				double dltotal,double dlnow,double ultotal,double ulnow)) ;*/
	std::string fieldget(const std::string k) ;
	void seturl(std::string url,bool post=false) ;
	void download(void);
	bool perform(std::string& data);
//	const std::string& operator[] (std::string& k) const;
//	std::string& operator[] (const std::string& k);
};

NS_CC_EXT_END

#endif


