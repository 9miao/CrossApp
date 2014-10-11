require('js/header.js');
var TestController = ca.JSViewController.extend({
ctor:function () {
   this._super();
                                                
},
                                                
viewDidLoad:function(){
   var rect = this.getView().getBounds();
   var size =   cc.size(rect.width,rect.height);
   var label = ca.Label.createWithCenter(cc.rect(size.width/2,size.height/2,400,100));
   label.setText("Hello World!");
   label.setFontSize(60);
   label.setTextAlignment(1);
   this.getView().addSubview(label);
},
                                                
viewDidUnload:function(){
     
}
                                                
});

var dir = ca.Application.getApplication();
var window = ca.Window.create();
var test= new TestController();
test.init();
window.setRootViewController(test);
dir.runWindow(window);

