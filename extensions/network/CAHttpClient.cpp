
//  Copyright (c) 2014 http://9miao.com All rights reserved.

#include "CAHttpClient.h"
#include <cstdio>

NS_CC_EXT_BEGIN

size_t lcbak(char *ptr,size_t size,size_t nmemb,void * userdata)
{
	std::string * str = static_cast<std::string*>(userdata);
	*str+=ptr;
	return size*nmemb;
}

size_t dcbak(char *ptr,size_t size,size_t nmemb,void *userdata)
{
	size_t i = size*nmemb;
	FILE *f = (FILE*)(userdata);
	fwrite((void*)ptr,size,nmemb,f);
	return i;
}
	
CAHttpClient::CAHttpClient()
:downloadbit(false)
,curl_already_init(false)
,post(false)
{

}
/*void CAHttpClient::progressbar(void *ptr,int (*func)(void *ptr,
				double dltotal,double dlnow,double ultotal,double ulnow)) {
	curl_easy_setopt(curl,CURLOPT_PROGRESSDATA,ptr);
	curl_easy_setopt(curl,CURLOPT_PROGRESSFUNCTION,func);
}*/

CAHttpClient::~CAHttpClient()
{

}


void CAHttpClient::download(void)
{
	downloadbit = true;
}

void CAHttpClient::fieldset(const std::string k,const std::string v)
{
	postdata[k] = v;
}

void CAHttpClient::fieldset(const std::string k,const long v)
{
	char str[128] = "";
	sprintf(str,"%ld",v);
	fieldset(k,str);
}

void CAHttpClient::fieldset(const std::string k,const double v)
{
	char str[128] = "";
	sprintf(str,"%lf",v);
	fieldset(k,str);
}

void CAHttpClient::fieldunset(const std::string k)
{
	postdata.erase(k);
}


void CAHttpClient::seturl(std::string url,bool post)
{
	if(!curl_already_init)
    {
		curl = curl_easy_init();
		curl_already_init = true;
	}
	curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,lcbak);
	curl_easy_setopt(curl,CURLOPT_WRITEDATA,static_cast<void*>(&this->httpret));
	curl_easy_setopt(curl,CURLOPT_URL,url.c_str());
	curl_easy_setopt(curl,CURLOPT_AUTOREFERER,1);
	curl_easy_setopt(curl,CURLOPT_COOKIEJAR,"cookie");
	curl_easy_setopt(curl,CURLOPT_COOKIEFILE,"cookie");
	this->url = url;
	if(post)
    {
		curl_easy_setopt(curl,CURLOPT_POST,1);
		curl_easy_setopt(curl,CURLOPT_POSTREDIR,CURL_REDIR_POST_ALL);
		this->post=true;
	}
}
	

void CAHttpClient::join(void)
{
	postfields = "";
	for(std::map<std::string,std::string>::iterator x = postdata.begin();x!=postdata.end();++x)
    {
		postfields.append((*x).first+"="+(*x).second+"&");
	}
	postfields.erase(postfields.end()-1);
}


void CAHttpClient::fieldclean(void)
{
	postdata.clear();
	postfields="";
}
	
	
bool CAHttpClient:: perform(std::string& data)
{
	httpret.clear();
	if(url.empty())return false;
	if(!curl_already_init)
    {
		seturl(url,post);
	}

	if(post)
    {
		join();
		curl_easy_setopt(curl,CURLOPT_POSTFIELDS,postfields.c_str());
	}
    else
    {
		curl_easy_setopt(curl,CURLOPT_HTTPGET,1);
	}

	if(downloadbit)
    {
		f = fopen(data.c_str(),"w");
		curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,dcbak);
		curl_easy_setopt(curl,CURLOPT_WRITEDATA,(void*)(&f));
	}


	bool success = true;
	if((curle = curl_easy_perform(curl)) != CURLE_OK)
    {
		std::cout << curle << std::endl;
		success = false;
	}
	curl_already_init = false;

	if(downloadbit)
    {
		fclose(f);
		downloadbit = false;
	}

	if(curl_easy_getinfo(curl,CURLINFO_RESPONSE_CODE,&rescode) == CURLE_OK)
    {
		data = httpret;
	}
    else
    {
		data.clear();
		success = false;
	}
	curl_easy_cleanup(curl);
	return success;
}

/*int func(void *ptr,double dltotal,double dlnow,double ultotal,double ulnow){
	std::cout << "download total :" << dltotal << "dlnow : " << dlnow << std::endl; 
}

int main(void){
	CAHttpClient *http = new CAHttpClient();
	std::string ret;
	http->seturl("http://127.0.0.1/hehe.php?file=gtk");
	//http.fieldset("file","gtk");
	//std::cout << "return :" << ret << std::endl;

	
	http->download();
	//http.progressbar(NULL,func);
	ret.assign("hehe.tar.xz");
	http->perform(ret);
	delete http;
	return 0;
}*/


NS_CC_EXT_END