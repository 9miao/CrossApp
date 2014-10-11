var textfieldController =  ca.JSViewController.extend({
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
	var textfield = ca.TextField.createWithCenter(cc.rect(size.width/2,size.height/4,500,80));
	textfield.setPlaceHolder("Please Enter Some Number");
    textfield.setTag(1);
	textfield.setDelegate(this);
	textfield.setSpaceHolderColor(cc.c4b(255,0,0,255));
	textfield.setKeyboardType(KEY_BOARD_TYPE_NUMBER);
	this.getView().addSubview(textfield);
                                                      
    var textfield1 = ca.TextField.createWithCenter(cc.rect(size.width/2,size.height/2,500,80));
    textfield1.setPlaceHolder("Please Enter Some Alphabet");
    textfield1.setTag(2);
    textfield1.setDelegate(this);
    textfield1.setSpaceHolderColor(cc.c4b(0,255,0,255));
    textfield1.setKeyboardType(KEY_BOARD_TYPE_ALPHABET);
    this.getView().addSubview(textfield1);
                                                      
    var textfield2 = ca.TextField.createWithCenter(cc.rect(size.width/2,size.height*3/4,500,80));
    textfield2.setPlaceHolder("Please Enter");
    textfield2.setTag(3);
    textfield2.setDelegate(this);
    textfield2.setSpaceHolderColor(cc.c4b(0,0,255,255));
    textfield2.setKeyboardType(KEY_BOARD_TYPE_NORMAL);
    this.getView().addSubview(textfield2);
},
viewDidUnload:function()
{
    var drawer = ca.Application.getApplication().getRootWindow().getRootViewController();
    drawer.setTouchMoved(true);
},
onTextFieldInsertText:function(textfield,text,len)
{
    
},
onTextFieldDeleteBackward:function(textfield,deltext,len)
{

},
getKeyBoardHeight:function(textfield,height)
{
   
}
});