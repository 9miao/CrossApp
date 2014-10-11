var socketViewController =  ca.JSViewController.extend({
_wsiSendText: null,
_wsiSendBinary: null,
_wsiError: null,
_sendTextTimes: 0,
_sendBinaryTimes: 0,
ctor:function () {
    this._super();
    var drawer = ca.Application.getApplication().getRootWindow().getRootViewController();
    drawer.setTouchMoved(false);
},
viewDidLoad:function(){
    var rect = this.getView().getBounds();
    var size =   cc.size(rect.width,rect.height);
    var button = ca.Button.createWithCenter(cc.rect(size.width/2,size.height/2,200,100),ca.BUTTONTYPEROUNDEDRECT);
    button.addTarget(this,this.onMenuSendTextClicked,ca.ControlEventTouchUpSide);
    this.getView().addSubview(button);
                                                       
    var self = this;

    this._wsiSendText = new WebSocket("ws://echo.websocket.org");
    this._wsiSendText.onopen = function(evt) {
        cc.log("Send Text WS was opened.");
    };

    this._wsiSendText.onmessage = function(evt) {
        self._sendTextTimes++;
        var textStr = "response text msg: "+evt.data+", "+self._sendTextTimes;
        cc.log(textStr);
    };
    this._wsiSendText.onerror = function(evt) {
        cc.log("sendText Error was fired");
    };
    this._wsiSendText.onclose = function(evt) {
        cc.log("_wsiSendText websocket instance closed.");
        self._wsiSendText = null;
    };
},
onMenuSendTextClicked: function(sender,pos) {
                                                       
    if (this._wsiSendText.readyState == WebSocket.OPEN)
    {
        cc.log("Send Text WS is waiting...");
        this._wsiSendText.send("Hello WebSocket中文, I'm a text message.");
    }
    else
    {
        var warningStr = "send text websocket instance wasn't ready...";
        cc.log(warningStr);
    }
},
viewDidUnload:function(){
    var drawer = ca.Application.getApplication().getRootWindow().getRootViewController();
    drawer.setTouchMoved(true)
}
});