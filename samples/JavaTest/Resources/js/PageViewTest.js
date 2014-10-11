var pageViewController =  ca.JSViewController.extend({
ctor:function () {
   this._super();
   var drawer = ca.Application.getApplication().getRootWindow().getRootViewController();
   drawer.setTouchMoved(false);
},
viewDidLoad:function()
{
	var rect = this.getView().getBounds();                                       
    var size =   cc.size(rect.width,rect.height);
    var arr = Array();
    var pageview = ca.PageView.createWithCenter(cc.rect(size.width/2,size.height/2,size.width,size.height),0);
    pageview.setDelegate(this);
    var view1 =ca.ImageView.createWithFrame(cc.rect(0,0,400,200));
    view1.setImage(ca.Image.create("source_material/image030.jpg"));
    var view2 =ca.ImageView.createWithFrame(cc.rect(0,0,400,200));
    view2.setImage(ca.Image.create("source_material/image031.jpg"));
    var view3 =ca.ImageView.createWithFrame(cc.rect(0,0,400,200));
    view3.setImage(ca.Image.create("source_material/image032.jpg"));
    var view4 =ca.ImageView.createWithFrame(cc.rect(0,0,400,200));
    view4.setImage(ca.Image.create("source_material/image033.jpg"));
    arr.push(view1);
    arr.push(view2);
    arr.push(view3);
    arr.push(view4);
    pageview.setViews(arr);
    this.getView().addSubview(pageview);
},
pageViewDidSelectPageAtIndex:function(view,index)
{
    //cc.log(index);
},
viewDidUnload:function()
{
    var drawer = ca.Application.getApplication().getRootWindow().getRootViewController();
    drawer.setTouchMoved(true);
}
});