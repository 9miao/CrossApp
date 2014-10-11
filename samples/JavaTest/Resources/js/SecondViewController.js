var secondViewController =  ca.JSViewController.extend({
ctor:function () {
    this._super();
    var drawer = ca.Application.getApplication().getRootWindow().getRootViewController();
    drawer.setTouchMoved(false);
},
viewDidLoad:function(){
    var rect = this.getView().getBounds();
    var size = cc.size(rect.width,rect.height);
    var label = ca.Label.createWithCenter(cc.rect(size.width/2,100,300,100));
    label.setTag(100);
    label.setTextAlignment(ca.TextAlignmentCenter);
    label.setFontSize(40);
    label.setColor(cc.c4b(0,20,255,255));
    label.setText("HelloWorld!");
    this.getView().addSubview(label);
},
viewDidUnload:function(){
   var drawer = ca.Application.getApplication().getRootWindow().getRootViewController();
   drawer.setTouchMoved(true);
},
updateCallBack:function(dt)
{
    var label =this.getView().getSubviewByTag(100);
    label.setText("按钮已点击");
}
});