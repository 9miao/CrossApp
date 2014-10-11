var buttonViewController =  ca.JSViewController.extend({
_textLabe:null,
ctor:function () {
    this._super();
    var drawer = ca.Application.getApplication().getRootWindow().getRootViewController();
    drawer.setTouchMoved(false);
},
viewDidLoad:function(){
    var rect = this.getView().getBounds();
    var size =   cc.size(rect.width,rect.height);
    _textLabe = ca.Label.createWithCenter(cc.rect(size.width/2,size.height/4,400,80));
    _textLabe.setColor(ca.RED);
    _textLabe.setVerticalTextAlignmet(ca.VerticalTextAlignmentCenter);
    _textLabe.setTextAlignment(ca.TextAlignmentCenter);
    _textLabe.setFontSize(60);
    this.getView().addSubview(_textLabe);
    
    var button = ca.Button.createWithCenter(cc.rect(size.width/2,size.height/2,200,70),ca.BUTTONTYPEROUNDEDRECT);
    button.setTitleForState(ca.ControlStateNormal,"我是按钮1");
    button.addTarget(this,this.btnCallBack,ca.ControlEventTouchUpInSide);
    this.getView().addSubview(button);
    
    var button1 = ca.Button.createWithCenter(cc.rect(size.width/2,size.height*3/4,200,70),ca.BUTTONTYPECUSTOM);
    button1.setTitleForState(ca.ControlStateNormal,"我是按钮2");
                                                       
    var scaleview =ca.Scale9ImageView.create();
    scaleview.setImage(ca.Image.create("source_material/btn_rounded3D_normal.png"));
                                                       
    var scaleview1 =ca.Scale9ImageView.create();
    scaleview1.setImage(ca.Image.create("source_material/btn_rounded3D_highlighted.png"));
                                                       
    button1.setBackGroundViewForState(ca.ControlStateNormal,scaleview);
    button1.setBackGroundViewForState(ca.ControlStateHighlighted,scaleview1);
    button1.addTarget(this,this.btnCallBack1,ca.ControlEventTouchUpInSide);
    this.getView().addSubview(button1);
},
btnCallBack:function(sender,dipPos)
{
	_textLabe.setText("TouchButton1");
},
btnCallBack1:function(sender,dipPos)
{
	_textLabe.setText("TouchButton2");
},
viewDidUnload:function(){
    var drawer = ca.Application.getApplication().getRootWindow().getRootViewController();
    drawer.setTouchMoved(true)
}
});