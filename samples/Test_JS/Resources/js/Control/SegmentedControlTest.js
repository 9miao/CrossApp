/**
 * Created by zhanglei on 16/8/3.
 */
var SegmentedControlTest = ca.CAViewController.extend({
    ctor: function () {
        this._super();
    },
    viewDidLoad: function() {


        var segment1 = ca.CASegmentedControl.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout_W_C(300, 0.5), ca.DVerticalLayout_H_C(54, 0.25)), 3);
        for (var i = 0; i < 3; i++)
        {
            var  temstr = "Page" + (i + 1);
            segment1.setTitleForSegmentAtIndex(temstr, i);
        }
        segment1.setTitleColor(ca.CAColor4B.BLACK);
        segment1.setTitleSelectedColor(ca.CAColor4B.WHITE);
        segment1.setTarget(this.segmentCallback);
        segment1.setSelectedAtIndex(0);
        this.getView().addSubview(segment1);


        var segment2 = ca.CASegmentedControl.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout_W_C(300, 0.5), ca.DVerticalLayout_H_C(54, 0.55)), 3);
        for (var i = 0; i < 3; i++)
        {
            var  temstr2 = "Page" + (i + 1);
            segment2.setTitleForSegmentAtIndex(temstr2, i);
        }
        segment2.setTitleColor(ca.CAColor4B.BLACK);
        segment2.setTitleSelectedColor(ca.CAColor4B.WHITE);
        segment2.setTarget(this.segmentCallback);
        segment2.setBackgroundImage(ca.CAImage.create("image/seg1.png"));
        segment2.setSegmentItemBackgroundImage(ca.CAImage.create("image/seg2.png"));
        segment2.setTintColor(ca.CAColor4B.set(44, 178, 93, 255));
        segment2.setSelectedAtIndex(0);
        this.getView().addSubview(segment2);
    },

    segmentCallback: function ( number) {
        log("segmentCallback-->"+ number);
    }

});
