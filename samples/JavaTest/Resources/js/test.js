var titleArr = new Array("ListViewTest"
                         ,"ScrollViewTest"
                         ,"LabelTest"
                         ,"TextFieldTest"
                         ,"ButtonTest"
                         ,"PageViewTest"
                         ,"SwitchTest"
                         ,"HttpTest"
                         ,"SocketTest");
var thirdViewController = ca.JSViewController.extend({
viewDidLoad:function(){
    var rect = this.getView().getBounds();
    var size =   cc.size(rect.width,rect.height);
    var tableview = ca.TableView.createWithFrame(cc.rect( 0
                                                         ,0
                                                         ,size.width
                                                         ,size.height));
    tableview.setDataSource(this);
    tableview.setDelegate(this);
    tableview.setAllowsSelection(true);
    this.getView().addSubview(tableview);
},
viewDidUnload:function(){
    
},
tableViewHeightForRowAtIndexPath:function(table,section,row){
    return 640*0.2;
},
tableCellAtIndex:function(table,cellSize,section,row){
    var cell = table.dequeueReusableCellWithIdentifier("abc");
    var label;
    var num = row;
    if(!cell)
    {
        cell = ca.TableViewCell.create("abc");
        var size =   ca.Application.getApplication().getWinSize();
        label = ca.Label.createWithCenter(cc.rect(size.width/2
                                                  ,64
                                                  ,400
                                                  ,50));
        label.setTag(1);
        label.setTextAlignment(ca.TextAlignmentCenter);
        label.setFontSize(40);
        label.setColor(cc.c4b(51,204,255,255));
        
        label.setText(titleArr[row]);
        cell.addSubview(label);
        
    }
    else
    {
        label = cell.getSubviewByTag(1);
        label.setText(titleArr[row]);
    }
    return cell;
},
numberOfRowsInSection:function(table,section){
    return titleArr.length;
},
tableViewDidSelectRowAtIndexPath:function(table,section,row)
{
	if(row ==4)
	{
	   var e = new buttonViewController();
	   e.init();
   	   this.getNavigationController().pushViewController(e,true);
	}
	else if(row == 0)
	{
       var a = new ListViewController();
       a.init();
       this.getNavigationController().pushViewController(a,true);
	}
    else if(row ==1)
	{
       var b = new scrollViewController();
	   b.init();
   	   this.getNavigationController().pushViewController(b,true);
	}
    else if(row == 2)
    {
       var c = new labelViewController();
       c.init();
       this.getNavigationController().pushViewController(c,true);
    }
    else if(row ==3)
    {
       var d = new textfieldController();
       d.init();
       this.getNavigationController().pushViewController(d,true);
    }
    else if(row == 5)
    {
       var f = new pageViewController();
       f.init();
       this.getNavigationController().pushViewController(f,true);
    }
    else if(row == 6)
    {
       var g = new switchViewController();
       g.init();
       this.getNavigationController().pushViewController(g,true);
    }else if(row == 7)
    {
       var h = new httpViewController();
       h.init();
       this.getNavigationController().pushViewController(h,true);
    }else if(row == 8)
    {
       var j = new socketViewController();
       j.init();
       this.getNavigationController().pushViewController(j,true);
    }
}                                      
                                                      
});