var ListViewController =  ca.JSViewController.extend({
ctor:function () 
{
    this._super();
    var drawer = ca.Application.getApplication().getRootWindow().getRootViewController();
    drawer.setTouchMoved(false);
},
viewDidLoad:function()
{
	var rect = this.getView().getBounds();                                       
    var size =   cc.size(rect.width,rect.height);
    var arr = Array();
    var listview = ca.ListView.createWithFrame(cc.rect(0,0,size.width,size.height));
    listview.setListViewDataSource(this);
                                                     listview.setListViewOrientation(0);
                                                     listview.setListHeaderHeight(1);
    this.getView().addSubview(listview);
                                                     
},
pageViewDidSelectPageAtIndex:function(view,index)
{
   
},
viewDidUnload:function()
{
    var drawer = ca.Application.getApplication().getRootWindow().getRootViewController();
    drawer.setTouchMoved(true);
},
numberOfIndex:function(listView)
{
	return 10;
},
listViewCellAtIndex:function(listView,cellSize,index)
{
	var cell = listView.dequeueReusableCellWithIdentifier("abc");
	if(!cell)
	{
		cell = ca.ListViewCell.create("abc");
	}
	return cell;
},
listViewHeightForIndex:function(listView,cellSize,index)
{
	return 150;
}
});