var switchViewController =  ca.JSViewController.extend({
ctor:function () {
    this._super();
    var drawer = ca.Application.getApplication().getRootWindow().getRootViewController();
    drawer.setTouchMoved(false);
},
viewDidLoad:function(){
    var rect1 = this.getView().getBounds();
    var size = cc.size(rect1.width,rect1.height);
	var switchlabel  = ca.Label.createWithCenter(cc.rect(size.width/2,size.width/8,400,_px(80)));
	switchlabel.setText("Switch is off");
    switchlabel.setFontSize(_px(50));
    switchlabel.setTextAlignment(ca.TextAlignmentCenter);
    switchlabel.setColor(cc.c4b(0,0,0,255));
	switchlabel.setTag(500);
    this.getView().addSubview(switchlabel); 
                          
	var slider = ca.Slider.createWithCenter(cc.rect(size.width/2,size.height/2,300,80));
	slider.setTag(200);
    slider.addTarget(this,this.sliderCallBack);
    this.getView().addSubview(slider);
                                                       
    var label = ca.Label.createWithCenter(cc.rect(size.width/2,size.height*3/4,0.3125*size.height,size.width/8));
    label.setText("0.0");
    label.setTextAlignment(ca.TextAlignmentCenter);
    label.setFontSize(_px(50));
    label.setTag(300);
    label.setColor(cc.c4b(255,0,0,255));
    this.getView().addSubview(label);

	var progress = ca.Progress.create();
    progress.setTag(400);
    progress.setCenter(cc.rect(size.width/2,0.3125*size.height,400,200));
	this.getView().addSubview(progress);
	
    var switch1 = ca.Switch.createWithCenter(cc.rect(size.width/2,size.height/4,100,80));
    switch1.setTag(100);
    switch1.addTarget(this,this.switchCallBack);
    this.getView().addSubview(switch1);
                                                       
    var stepper = ca.Stepper.createWithCenter(cc.rect(size.width/2,0.8125*size.height,200,80));
    switch1.setTag(1000);
    this.getView().addSubview(stepper);
                                                       
},
viewDidUnload:function(){
    var drawer = ca.Application.getApplication().getRootWindow().getRootViewController();
    drawer.setTouchMoved(true);
},
sliderCallBack:function(sender,pos)
{
    var slider = sender;
    var value = slider.getValue();
    var label = this.getView().getSubviewByTag(300);
    label.setText(value);
    var progress = this.getView().getSubviewByTag(400);
    progress.setProgress(value);
                                                       
},
switchCallBack:function(sender,pos)
{
    var switchlabel = this.getView().getSubviewByTag(500);
    if(sender.isOn() == 0)
    {
    	switchlabel.setText("Switch is off");
    }else
    {
    	switchlabel.setText("Switch is on");
    }
    
},
updateCallBack:function(sender,pos)
{

}
});