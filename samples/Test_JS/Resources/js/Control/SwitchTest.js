/**
 * Created by crossApp on 16/8/3.
 */
var SwitchTest = ca.CAViewController.extend({

    ctor: function () {
        this._super();
    },

    viewDidLoad: function() {

        var customSwitch1 = ca.CASwitch.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout_L_W(150, 100), ca.DVerticalLayout_T_H(350, 20)),ca.CASwitch.Type.RoundedRect);
        customSwitch1.setTag(100);
        customSwitch1.setIsOn(true, false);
        customSwitch1.setTarget(this.switchStateChange);
        this.getView().addSubview(customSwitch1);

        var customSwitch2 = ca.CASwitch.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout_R_W(150, 100), ca.DVerticalLayout_T_H(350, 20)),ca.CASwitch.Type.SquareRect);
        customSwitch2.setTag(101);
        customSwitch2.setIsOn(true, false);
        customSwitch2.setTarget(this.switchStateChange);
        this.getView().addSubview(customSwitch2);

    },

    switchStateChange: function (bIsOn) {
        if (!bIsOn)
        {
            log("switchStateChange false");
        }
        else
        {
            log("switchStateChange true");
        }
    }
});
