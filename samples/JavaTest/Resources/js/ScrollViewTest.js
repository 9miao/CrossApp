var scrollViewController =  ca.JSViewController.extend({
ctor:function () {
   this._super();
   var drawer = ca.Application.getApplication().getRootWindow().getRootViewController();
   drawer.setTouchMoved(false);
},
scrollViewDidScroll:function(view)
{
    
},
viewDidLoad:function(){
	var rect = this.getView().getBounds();
    var size =   cc.size(rect.width,rect.height);
    
    var bg = ca.ImageView.createWithFrame(cc.rect(0,0,size.width,size.height));
    bg.setImage(ca.Image.create("bg.jpg"));
    
    var sc = ca.ScrollView.createWithFrame(cc.rect(0,0,size.width,size.height));
    sc.setDelegate(this);
    sc.setMinimumZoomScale(0.5);
    sc.setMaximumZoomScale(2.0);
    sc.addSubview(bg);
    this.getView().addSubview(sc);
},
viewDidUnload:function(){
    var drawer = ca.Application.getApplication().getRootWindow().getRootViewController();
    drawer.setTouchMoved(true);
}
});