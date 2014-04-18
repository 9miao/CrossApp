//
//  MySocket.cpp
//  project
//
//  Created by 栗元峰 on 14-3-19.
//
//

#include "MySocket.h"
#include "MainInterface.h"
#include "message.h"

#define IP "172.16.18.25"

#define HOST 16009

static MySocket* _socket = NULL;

MySocket::MySocket()
{
    m_pSocket = new SocketClient(IP, HOST, 1, 1, NULL);
    m_pSocket->start();
}

MySocket::~MySocket()
{

}

MySocket* MySocket::shareMySocket()
{
    if (_socket == NULL)
    {
        _socket = new MySocket();
    }
    return _socket;
}

void MySocket::send(map<string, string>* var, unsigned short messageID)
{
    do
    {
        CC_BREAK_IF(m_pSocket->m_iState != SocketClient_DESTROY);
        m_pSocket->reconnect();
    }
    while (0);
    
    CSJson::FastWriter writer;
    CSJson::Value root;
    
    
    
    string messageStr = "";
    
    if (var)
    {
        for (map<string, string>::iterator itr=var->begin(); (var && itr!=var->end()); itr++)
        {
            root[itr->first]=atoi(itr->second.c_str());
        }
        messageStr = writer.write(root);
    }
    else
    {
        messageStr = "{}";
    }
    
    //CCLog("%s",messageStr.c_str());
    Message *msg = m_pSocket->constructMessage(messageStr.c_str(), messageID);
    m_pSocket->sendMessage_(msg, true);
}

void MySocket::callBackMessage(Message* message, int msgID)
{
    CCLog("\n\n%s\n\n", message->data);
    
    map<int, MysocketDelegate*>::iterator itr = m_nMap.find(msgID);
    do
    {
        CC_BREAK_IF(itr == m_nMap.end());
        CSJson::Reader read;
        CSJson::Value root;
        bool flag = read.parse(message->data, root);
        CC_BREAK_IF(flag == false);
        itr->second->callBackWithSocket(msgID, root);
    }
    while (0);
}

void MySocket::addDelegate(int msgID, MysocketDelegate *obj)
{
    m_nMap.erase(msgID);
    m_nMap[msgID] = obj;
}

void MySocket::removeDelegate(int msgID, MysocketDelegate *obj)
{
    m_nMap.erase(msgID);
}