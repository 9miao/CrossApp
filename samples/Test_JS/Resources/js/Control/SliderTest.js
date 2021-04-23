/**
 * Created by crossApp on 16/8/3.
 */
var SliderTest = ca.CAViewController.extend({

    ctor: function (num) {
        this._super();
    },

    viewDidLoad: function() {


        this.sliderValue =  ca.CALabel.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout_L_R(100, 100), ca.DVerticalLayout_T_H(250, 50)));
        this.sliderValue.setColor(ca.CAColor4B.set(51,204,255,255));
        this.sliderValue.setText("0");
        this.sliderValue.setFontSize(30);
        this.sliderValue.setTextAlignment(ca.CATextAlignment.Center);
        this.sliderValue.setVerticalTextAlignmet(ca.CAVerticalTextAlignment.Center);


        this.getView().addSubview(this.sliderValue);
        var slider1 = ca.CASlider.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout_L_R(120, 120), ca.DVerticalLayout_T_H(300, 56)));
        var callbackSlider1 = this.sliderValue;
        slider1.setTarget(function (value) {
            value =  value * 100;
            callbackSlider1.setText(Math.round(value));
        });
        slider1.setTag(100);
        this.getView().addSubview(slider1);

        /******************************************************************************************/

        this.sliderValue2 =  ca.CALabel.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout_W_C(100 , 0.5), ca.DVerticalLayout_T_H(550, 50)));
        this.sliderValue2.setColor(ca.CAColor4B.set(51,204,255,255));
        this.sliderValue2.setText("0");
        this.sliderValue2.setFontSize(30);
        this.sliderValue2.setTextAlignment(ca.CATextAlignment.Center);
        this.sliderValue2.setVerticalTextAlignmet(ca.CAVerticalTextAlignment.Center);
        this.getView().addSubview(this.sliderValue2);



        var slider2 =  ca.CASlider.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout_L_R(120, 120), ca.DVerticalLayout_T_H(600, 56)));
        slider2.setTag(101);
        slider2.setThumbTintImage( ca.CAImage.create("source_material/btn_square_highlighted.png"));
        var callbackSlider2 = this.sliderValue2;
        slider2.setTarget(function (value) {
            value =  value * 100;
            callbackSlider2.setText(Math.round(value));
        });
        this.getView().addSubview(slider2);
    },
});
