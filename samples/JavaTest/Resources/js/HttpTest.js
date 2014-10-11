var httpViewController =  ca.JSViewController.extend({
ctor:function () {
    this._super();
    var drawer = ca.Application.getApplication().getRootWindow().getRootViewController();
    drawer.setTouchMoved(false);
},
viewDidLoad:function(){
    var rect = this.getView().getBounds();
    var size =   cc.size(rect.width,rect.height);
    var xhr = new XMLHttpRequest();
    xhr.open("POST", "http://httpbin.org/post");
    xhr.onreadystatechange = function() {
        var httpStatus = xhr.statusText;
        var response = xhr.responseText.substring(0,800) + "...";
        cc.log(response);
    }; 
    xhr.send("test=ok");
},
viewDidUnload:function(){
    var drawer = ca.Application.getApplication().getRootWindow().getRootViewController();
    drawer.setTouchMoved(true)
}
});