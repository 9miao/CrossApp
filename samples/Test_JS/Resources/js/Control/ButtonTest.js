/**
 * Created by zhanglei on 16/8/2.
 */
var ButtonTest = ca.CAViewController.extend({
    ctor: function () {
        this._super();
        // var navItem = ca.CANavigationBarItem.create("按钮1");
        // this.setNavigationBarItem(navItem);
        this.setTitle("asdfasdf");
    },
    viewDidLoad: function() {



        var btn1 = ca.CAButton.create(ca.CAButton.Type.Custom);
        btn1.setLayout(ca.DLayout.set(ca.DHorizontalLayout_W_C(240, 0.5), ca.DVerticalLayout_H_C(54, 0.16)));
        //设置Button标题的显示文本和字体大小
        btn1.setTitleForState(ca.CAControl.State.Normal, "Noborder");
        btn1.setTitleFontSize(40);
        //设置Button的标题颜色和状态
        btn1.setTitleColorForState(ca.CAControl.State.Normal, ca.CAColor4B.set(51,204,255,255));
        //添加回调事件
        btn1.addTarget(function(){ log("Noborder");}, ca.CAButton.Event.TouchUpInSide );
        this.getView().addSubview(btn1);




        var btn2 = ca.CAButton.create(ca.CAButton.Type.SquareRect);
        btn2.setLayout(ca.DLayout.set(ca.DHorizontalLayout_W_C(240, 0.5), ca.DVerticalLayout_H_C(54, 0.32)));
        //设置Button标题的显示文本
        btn2.setTitleForState(ca.CAControl.State.Normal, "SquareRect");
        //添加回调事件
        btn2.addTarget(function(){ log("Button SquareRect");}, ca.CAButton.Event.TouchUpInSide );
        this.getView().addSubview(btn2);



        var btn3 = ca.CAButton.create(ca.CAButton.Type.RoundedRect);
        btn3.setLayout(ca.DLayout.set(ca.DHorizontalLayout_W_C(240, 0.5), ca.DVerticalLayout_H_C(54, 0.48)));
        //设置Button标题的显示文本
        btn3.setTitleForState(ca.CAControl.State.Normal, "RoundedRect");
        //添加回调事件
        btn3.addTarget(function(){ log("Button RoundedRect");}, ca.CAButton.Event.TouchUpInSide );
        this.getView().addSubview(btn3);


        var btn4 = ca.CAButton.create(ca.CAButton.Type.SquareRect);
        btn4.setLayout(ca.DLayout.set(ca.DHorizontalLayout_W_C(240, 0.5), ca.DVerticalLayout_H_C(54, 0.64)));
        //设置Button的标题颜色和状态
        btn4.setTitleColorForState(ca.CAControl.State.Normal,ca.CAColor4B.WHITE);
        //设置Button的背景View
        btn4.setBackgroundImageForState(ca.CAControl.State.Normal, ca.CAImage.create("image/button_normal.png"), true);
        //设置Button的背景View
        btn4.setBackgroundImageForState(ca.CAControl.State.Highlighted, ca.CAImage.create("image/button_down.png"), true);
        //添加回调事件
        btn4.addTarget(function(){ log("Button Custom");}, ca.CAButton.Event.TouchDown );
        this.getView().addSubview(btn4);

    },


});
