/**
 * Created by crossApp on 16/8/3.
 */
var LabelTest = ca.CAViewController.extend({

    ctor: function (num) {
        this._super();

        var item  = ca.CANavigationBarItem.create("文本展示") ;
        var buttonItem = ca.CABarButtonItem.create("", ca.CAImage.create("source_material/btn_right_white.png"), null) ;
        buttonItem.setCallbackFunction(this.btncallback.bind(this)) ;
        item.addRightButtonItem(buttonItem) ;
        this.setNavigationBarItem(item);

    },

    viewDidLoad: function() {

        this._labelNum = 0 ;
        this._viewContainer = ca.CAView.createWithLayout(ca.DLayout.fill());
        this.getView().addSubview(this._viewContainer);

        this._initLabels() ;

    },

    _initLabels:function(){
        var str = "Hello World";
        if (this._labelNum == 0)
        {
            var Font = ca.CAFont.create();
            Font.fontSize = 36;
            Font.color = ca.RED;

            var label = ca.CALabel.create();
            label.setText(str);
            label.setFont(Font);
            label.setTextAlignment(ca.CATextAlignment.Center);
            label.setVerticalTextAlignmet(ca.CAVerticalTextAlignment.Center);
            label.setLayout(ca.DLayout.set(ca.DHorizontalLayout_L_R(100, 100), ca.DVerticalLayout_H_C(40, 0.16)));

            var Font1 = ca.CAFont.create() ;
            Font1.fontSize = 36;
            Font1.color = ca.RED;
            Font1.underLine = true;

            var label1 = ca.CALabel.create();
            label1.setText(str);
            label1.setFont(Font1);
            label1.setTextAlignment(ca.CATextAlignment.Center);
            label1.setVerticalTextAlignmet(ca.CAVerticalTextAlignment.Center);
            label1.setLayout(ca.DLayout.set(ca.DHorizontalLayout_L_R(100, 100), ca.DVerticalLayout_H_C(40, 0.32)));

            var label2 = ca.CALabel.create();
            var Font2 = ca.CAFont.create() ;
            Font2.fontSize = 36;
            Font2.color = ca.RED;
            Font2.bold = true;
            label2.setText(str);
            label2.setFont(Font2);
            label2.setTextAlignment(ca.CATextAlignment.Center);
            label2.setVerticalTextAlignmet(ca.CAVerticalTextAlignment.Center);
            label2.setLayout(ca.DLayout.set(ca.DHorizontalLayout_L_R(100, 100), ca.DVerticalLayout_H_C(40, 0.48)));

            var label3 = ca.CALabel.create();
            var Font3 = ca.CAFont.create() ;
            Font3.fontSize = 36;
            Font3.color = ca.RED;
            Font3.italics = true;
            label3.setText(str);
            label3.setFont(Font3);
            label3.setTextAlignment(ca.CATextAlignment.Center);
            label3.setVerticalTextAlignmet(ca.CAVerticalTextAlignment.Center);
            label3.setLayout(ca.DLayout.set(ca.DHorizontalLayout_L_R(100, 100), ca.DVerticalLayout_H_C(40, 0.64)));

            var label4 = ca.CALabel.create();
            var Font4 = ca.CAFont.create() ;
            Font4.fontSize = 36;
            Font4.color = ca.RED;
            label4.setText("Enable Copy");
            label4.setFont(Font4);
            label4.setTextAlignment(ca.CATextAlignment.Center);
            label4.setVerticalTextAlignmet(ca.CAVerticalTextAlignment.Center);
            label4.setLayout(ca.DLayout.set(ca.DHorizontalLayout_L_R(100, 100), ca.DVerticalLayout_H_C(40, 0.8)));

            this._viewContainer.addSubview(label);
            this._viewContainer.addSubview(label1);
            this._viewContainer.addSubview(label2);
            this._viewContainer.addSubview(label3);
            this._viewContainer.addSubview(label4);

        }
        else if (this._labelNum == 1)
        {
            var label5 = ca.CALabel.create();
            var Font5 = ca.CAFont.create() ;
            Font5.fontSize = 36;
            Font5.color = ca.RED;
            Font5.bold = true;
            Font5.italics = true;
            Font5.underLine = true;
            label5.setText("Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World");
            label5.setFont(Font5);
            label5.setLineSpacing(20);
            label5.setTextAlignment(ca.CATextAlignment.Center);
            label5.setVerticalTextAlignmet(ca.CAVerticalTextAlignment.Center);
            label5.setLayout(ca.DLayout.set(ca.DHorizontalLayout_L_R(50, 50), ca.DVerticalLayout_T_H(300, 300)));

            this._viewContainer.addSubview(label5);

        }
        else
        {

            var RichLabel = ca.CARichLabel.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout_L_R(100, 100), ca.DVerticalLayout_T_B(200, 200)));
            var RichLabelFont = ca.CAFont.create() ;
            RichLabelFont.bold = true;
            RichLabelFont.fontSize = 42;
            RichLabelFont.color = ca.RED;
            RichLabel.appendText("Hello World Hello World Hello World Hello World Hello World Hello World ", RichLabelFont);

            var RichLabelFont1 = ca.CAFont.create() ;
            RichLabelFont1.italics = true;
            RichLabelFont1.fontSize = 42;
            RichLabelFont1.color = ca.RED;
            RichLabel.appendText("Hello World Hello World Hello World Hello World Hello World Hello World ", RichLabelFont1);

            var RichLabelFont2 = ca.CAFont.create() ;
            RichLabelFont2.underLine = true;
            RichLabelFont2.fontSize = 42;
            RichLabelFont2.color = ca.RED;
            RichLabel.appendText("Hello World Hello World Hello World Hello World Hello World Hello World ", RichLabelFont2);

            this._viewContainer.addSubview(RichLabel);

        }
    },

    btncallback: function () {
        this._labelNum = this._labelNum == 2 ? 0 : this._labelNum +1 ;
        this._viewContainer.removeAllSubviews() ;
        this._initLabels() ;

    }
});
