var labelViewController =  ca.JSViewController.extend({
ctor:function () {
    this._super();
    var drawer = ca.Application.getApplication().getRootWindow().getRootViewController();
    drawer.setTouchMoved(false);
},
viewDidLoad:function(){
    var rect = this.getView().getBounds();
    var size =   cc.size(rect.width,rect.height);

    var textalignleft = ca.Label.createWithCenter(cc.rect(size.width/6 + 5
    														,size.width*0/2+size.width/4 + 20
    														,size.width/3 - 10
    														,size.width/2 + 30));
    textalignleft.setTag(100);
    textalignleft.setTextAlignment(ca.TextAlignmentLeft);
    textalignleft.setFontSize(40);
    textalignleft.setColor(cc.c4b(0,20,255,255));
    textalignleft.setText("Please Pay attention to me!,My Name is CrossApp-js!");
    this.getView().addSubview(textalignleft);

   var textaligncenter = ca.Label.createWithCenter(cc.rect(size.width/6 +size.width/3 + 5
                                                           ,size.width/4 + 20
                                                           ,size.width/3 - 10
                                                           ,size.width/2 + 30 ));
   textaligncenter.setTag(200);
   textaligncenter.setTextAlignment(ca.TextAlignmentCenter);
   textaligncenter.setFontSize(40);
   textaligncenter.setColor(cc.c4b(130,20,255,255));
   textaligncenter.setText("Please Pay attention to me!,My Name is CrossApp-js!");
   this.getView().addSubview(textaligncenter);                                                   
                                                      
    var textalignright = ca.Label.createWithCenter(cc.rect(size.width/6 +size.width*2/3 + 5
    														,size.width/4 + 20
    														,size.width/3 - 10
    														,size.width/2 + 30));
    textalignright.setTag(200);
    textalignright.setTextAlignment(ca.TextAlignmentRight);
    textalignright.setFontSize(40);
    textalignright.setColor(cc.c4b(130,20,255,255));
    textalignright.setText("Please Pay attention to me!,My Name is CrossApp-js!");
    this.getView().addSubview(textalignright);
                                                      
},
pageViewDidSelectPageAtIndex:function(view,index){
    
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