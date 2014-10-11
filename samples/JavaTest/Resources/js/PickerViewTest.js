var pickerViewController =ca.JSViewController.extend({
                                                     ctor:function () {
                                                     this._super();
                                                     var drawer = ca.Application.getApplication().getRootWindow().getRootViewController();
                                                     drawer.setTouchMoved(false);
                                                     },
scrollViewDidScroll:function(view)
{
    //cc.log("scrollViewDidScroll");
},
viewDidLoad:function()
{
                                                     cc.log("viewDidLoad");
    var rect =   this.getView().getBounds();
    var size =   cc.size(rect.width,rect.height);
	var pickerview = ca.PickerView.createWithCenter(cc.rect(size.width/2,size.height/2,size.width,size.height));
// 	pickerview.setDelegate(this);
	pickerview.setDataSource(this);
	pickerview.setFontSizeNormal(40);
	pickerview.setFontSizeSelected(40);
	pickerview.setFontColorNormal(cc.c4b(0,0,255,255));
	pickerview.setFontColorSelected(cc.c4b(0,0,255,255));
	pickerview.reloadAllComponents();
	this.getView().addSubview(pickerview);
},
numberOfRowsInComponent:function(view,component)
{
   return 10;
},
numberOfComponentsInPickerView:function(view)
{	
   return 3;
},
titleForRow:function(view,row,component)
{
   return row;
},
rowHeightForComponent:function(view,component)
{
   return 100;
},
widthForComponent:function(view,component)
{
   return 200;
},
viewDidUnload:function(){
    //cc.log("viewDidUNLoad");
}
});
