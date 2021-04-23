/**
 * Created by crossApp on 16/8/3.
 */

var ProgressTest = ca.CAViewController.extend({

    ctor: function () {
        this._super();
    },

    viewDidLoad: function() {
        this.progress_value = ca.CALabel.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout_L_R(100, 100), ca.DVerticalLayout_T_H(300, 50)));
        this.progress_value.setTag(100);
        this.progress_value.setFontSize(30);
        this.progress_value.setColor(ca.CAColor4B.set(51, 204, 255, 255));
        this.progress_value.setTextAlignment(ca.CATextAlignment.Center);
        this.progress_value.setVerticalTextAlignmet(ca.CAVerticalTextAlignment.Center);
        this.getView().addSubview(this.progress_value);

        this.progress = ca.CAProgress.create();
        this.progress.setLayout(ca.DLayout.set(ca.DHorizontalLayout_L_R(100, 100), ca.DVerticalLayout_T_H(550, 0)));
        //定时函数

        ca.CAScheduler.getScheduler().schedule(this.progressValueChange.bind(this),"A" ,this ,0);
        this.getView().addSubview(this.progress);
    },


    progressValueChange: function (dt) {
        var currentValue = this.progress.getProgress() + 0.001;
        this.progress.setProgress(currentValue);
        var valueText = currentValue*100-0.1;
        this.progress_value.setText(valueText.toFixed(1));
        if(null == this.progress){
            ca.CAScheduler.getScheduler().unschedule(this,this.progressValueChange);
        }
    }
});
