//
//  MySocket.h
//  project
//
//  Created by 栗元峰 on 14-3-19.
//
//

#ifndef __project__MySocket__
#define __project__MySocket__

#include <iostream>
#include "SocketClient.h"
#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;


class MysocketDelegate
{
public:
    
    virtual void callBackWithSocket(int msgID, const CSJson::Value& root) = 0;
};

class MySocket
{
    
    SocketClient* m_pSocket;
    
    map<int, MysocketDelegate*> m_nMap;
    
public:
    
    MySocket();
    
    ~MySocket();
    
    static MySocket* shareMySocket();
    
    void send(map<string, string>* var, unsigned short messageID);
    
    void callBackMessage(Message* message, int msgID);
    
    void addDelegate(int msgID, MysocketDelegate* obj);
    
    void removeDelegate(int msgID, MysocketDelegate* obj);
    
};

#endif /* defined(__project__MySocket__) */
