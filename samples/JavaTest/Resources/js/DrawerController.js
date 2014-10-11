var titleArr = new Array("ListViewTest"
                         ,"ScrollViewTest"
                         ,"LabelTest"
                         ,"TextFieldTest"
                         ,"ButtonTest"
                         ,"PageViewTest"
                         ,"SwitchTest"
                         ,"HttpTest"
                         ,"SocketTest");
var DrawerController =  ca.JSViewController.extend({
viewDidLoad:function()
{
	var rect = this.getView().getBounds();
    var size =   cc.size(rect.width,rect.height);
    var tableview = ca.TableView.createWithFrame(cc.rect( 0
                                                         ,0
                                                         ,300
                                                         ,size.height));
    tableview.setDataSource(this);
    tableview.setDelegate(this);
    tableview.setAllowsSelection(true);
    this.getView().addSubview(tableview);
},tableViewHeightForRowAtIndexPath:function(table,section,row){
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
        label = ca.Label.createWithCenter(cc.rect(150
                                                  ,64
                                                  ,300
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
    var con = ca.Application.getApplication().getRootWindow().getRootViewController();
    if(con.isShowLeftViewController())
    {
       con.hideLeftViewController(true);
    }
},
viewDidUnload:function()
{
  
}
});