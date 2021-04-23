/**
 * Created by zhanglei on 16/8/1.
 */
var AlertViewTest = ca.CAViewController.extend({
    ctor: function () {
        this._super();
    },
    viewDidLoad: function() {

        var btn1 = ca.CAButton.create(ca.CAButton.Type.RoundedRect);
        btn1.setLayout(ca.DLayout.set(ca.DHorizontalLayout_W_C(240, 0.5), ca.DVerticalLayout_H_C(54, 0.25)));
        btn1.setTitleForState(ca.CAControl.State.Normal, "Click-1");
        btn1.addTarget(function () {
            var alertView = ca.CAAlertView.create("Alert","this is a alert!");
            alertView.show(function (btnIndex)
            {
                ca.log("btnIndex=="+btnIndex);
            });
        }, ca.CAButton.Event.TouchUpInSide);
        this.getView().addSubview(btn1);

        var btn2 = ca.CAButton.create(ca.CAButton.Type.RoundedRect);
        btn2.setLayout(ca.DLayout.set(ca.DHorizontalLayout_W_C(240, 0.5), ca.DVerticalLayout_H_C(54, 0.5)));
        btn2.setTitleForState(ca.CAControl.State.Normal, "Click-2");
        btn2.addTarget(function () {
            var alertView = ca.CAAlertView.create("Alert", "this is a alert!");
            alertView.setButtonTitles(["ok","close"]);
            alertView.show(function (btnIndex)
            {
                ca.log("btnIndex=="+btnIndex);
            });
        },ca.CAButton.Event.TouchUpInSide);
        this.getView().addSubview(btn2);

        var btn3 = ca.CAButton.create(ca.CAButton.Type.RoundedRect);
        btn3.setLayout(ca.DLayout.set(ca.DHorizontalLayout_W_C(240, 0.5), ca.DVerticalLayout_H_C(54, 0.75)));
        btn3.setTitleForState(ca.CAControl.State.Normal, "Click-3");
        btn3.addTarget(function () {
            var text = "        When a lion was asleep, a little mouse began running up and down beside him. This soon wakened the lion. He was very angry, and caught the mouse in his paws."
                + "Forgive me, please.\" cried the little mouse.\"I may be able to help you someday.\" The lion was tickled at these words."
                + "He thought, \"How could this little mouse help me?\" However he lifted up his paws and let him go."
                + "A few days later, the lion was caught in a trap."
                + "The hunters wanted to take him alive to the king, so they tied him to a tree, and went away to look for a wagon."
                + "Just then the little mouse passed by, and saw the sad lion."
                + "He went up to him, and soon gnawed away the ropes. \"Was I not right?\" asked the little mouse.";

            var alertView = ca.CAAlertView.create("Alert", text);
            alertView.setButtonTitles(["ok","close","button3","button4","button5","button6","button7"]);
            alertView.show(function (btnIndex)
            {
                ca.log("btnIndex=="+btnIndex);
            });
        }, ca.CAButton.Event.TouchUpInSide);
        this.getView().addSubview(btn3);
    },
});
