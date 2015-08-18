

#ifndef __CC_WEBSOCKET_H__
#define __CC_WEBSOCKET_H__


#include "CrossApp.h"
#include "libwebsockets.h"
#include <list>

NS_CC_BEGIN

class WsThreadHelper;
class WsMessage;

class CC_DLL WebSocket
{
public:
    /**
     * @js ctor
     */
    WebSocket();
    /**
     *  @js NA
     *  @lua NA
     */
    virtual ~WebSocket();
    
    /**
     *  @brief Data structure for message
     */
    struct Data
    {
        Data():bytes(NULL), len(0), isBinary(false){}
        char* bytes;
        int len;
        bool isBinary;
    };
    
    /**
     *  @brief Errors in websocket
     */
    enum ErrorCode
    {
        kErrorTimeout = 0,
        kErrorConnectionFailure,
        kErrorUnknown
    };

    /**
     *  @brief The delegate class to process websocket events.
     *  @js NA
     *  @lua NA
     */
    class Delegate
    {
    public:
        virtual ~Delegate() {}
        virtual void onOpen(WebSocket* ws) = 0;
        virtual void onMessage(WebSocket* ws, const Data& data) = 0;
        virtual void onClose(WebSocket* ws) = 0;
        virtual void onError(WebSocket* ws, const ErrorCode& error) = 0;
    };
    
    
    /**
     *  @brief  The initialized method for websocket.
     *          It needs to be invoked right after websocket instance is allocated.
     *  @param  delegate The delegate which want to receive event from websocket.
     *  @param  url      The URL of websocket server.
     *  @return true: Success, false: Failure
     *  @js NA
     */
    bool init(const Delegate& delegate,
              const std::string& url,
              const std::vector<std::string>* protocols = NULL);
    
    /**
     *  @brief Sends string data to websocket server.
     */
    void send(const std::string& message);
    
    /**
     *  @brief Sends binary data to websocket server.
     */
    void send(const unsigned char* binaryMsg, unsigned int len);
    
    /**
     *  @brief Closes the connection to server.
     */
    void close();

    /**
     *  Websocket state
     */
    enum State
    {
        kStateConnecting = 0,
        kStateOpen,
        kStateClosing,
        kStateClosed
    };
    
    /**
     *  @brief Gets current state of connection.
     */
    State getReadyState();
private:
    virtual void onSubThreadStarted();
    virtual int onSubThreadLoop();
    virtual void onSubThreadEnded();
    virtual void onUIThreadReceiveMessage(WsMessage* msg);
    

    friend class WebSocketCallbackWrapper;
    int onSocketCallback(struct libwebsocket_context *ctx,
                         struct libwebsocket *wsi,
                         enum libwebsocket_callback_reasons reason,
                         void *user, void *in, size_t len);
    
private:
	State        _readyState;
    std::string  _host;
    unsigned int _port;
    std::string  _path;
    
    friend class WsThreadHelper;
    WsThreadHelper* _wsHelper;
    
    struct libwebsocket*         _wsInstance;
    struct libwebsocket_context* _wsContext;
    Delegate* _delegate;
    int _SSLConnection;
    struct libwebsocket_protocols* _wsProtocols;
};

NS_CC_END

#endif /* defined(__CC_JSB_WEBSOCKET_H__) */
