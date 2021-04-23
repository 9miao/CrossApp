/**
 * Created by crossApp on 16/8/3.
 */
var StepperTest = ca.CAViewController.extend({

    ctor: function () {
        this._super();
    },

    viewDidLoad: function() {

        this.step_value = ca.CALabel.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout_W_C(250, 0.5), ca.DVerticalLayout_H_C(100 , 0.15)));
        this.step_value.setText("value = 0");
        this.step_value.setFontSize(28);
        this.step_value.setTextAlignment(ca.CATextAlignment.Center);
        this.step_value.setVerticalTextAlignmet(ca.CAVerticalTextAlignment.Center);
        this.getView().addSubview(this.step_value);

        var step = ca.CAStepper.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout_W_C(240, 0.5), ca.DVerticalLayout_H_C(100. , 0.3)), ca.CAStepper.Orientation.Horizontal);
        step.setMinValue(0);
        step.setMaxValue(50);
        step.setStepValue(1);
        //step.setAutoRepeat(false);
        this.getView().addSubview(step);
        var valueLabel = this.step_value;
        step.setTarget(function (value) {
            log("stepperCallBack-->");
            var str = "value = " + value;
            valueLabel.setText(str);
        });


        /********************************************************************************************/


        this.step_value2 = ca.CALabel.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout_W_C(250, 0.5), ca.DVerticalLayout_H_C(100 , 0.65)));
        this.step_value2.setText("value = 0");
        this.step_value2.setFontSize(28);
        this.step_value2.setTextAlignment(ca.CATextAlignment.Center);
        this.step_value2.setVerticalTextAlignmet(ca.CAVerticalTextAlignment.Center);
        this.getView().addSubview(this.step_value2);


        var step2 = ca.CAStepper.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout_W_C(60, 0.5), ca.DVerticalLayout_H_C(200 , 0.8)), ca.CAStepper.Orientation.Vertical);
        step2.setMinValue(0);
        step2.setMaxValue(50);
        step2.setStepValue(1);
        this.getView().addSubview(step2);
        var valueLabel2 = this.step_value2;
        step2.setTarget(function (value) {
            log("stepperCallBack-->");
            var str = "value = " + value;
            valueLabel2.setText(str);
        });

    },

});
