/**
 * Created by crossApp on 2017/3/3.
 */

var PageControlTest = ca.CAViewController.extend({
    ctor: function ()
    {
        this._super();
    },


    viewDidLoad: function() {

        var pageControl = ca.CAPageControl.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout_W_C(150, 0.5), ca.DVerticalLayout_H_C(54, 0.25)));
        pageControl.setNumberOfPages(5);
        pageControl.setStyle(ca.CAPageControl.Style.Dot);
        pageControl.setPageIndicatorTintColor(ca.CAColor4B.WHITE);
        pageControl.setCurrentPageIndicatorTintColor(ca.CAColor4B.GREEN);
        pageControl.setTarget(function (index)
        {
            ca.log("index = "+index);
        });
        this.getView().addSubview(pageControl);

        var pageControl2 = ca.CAPageControl.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout_W_C(150, 0.5), ca.DVerticalLayout_H_C(54, 0.4)));
        pageControl2.setNumberOfPages(5);
        pageControl2.setStyle(ca.CAPageControl.Style.Round);
        pageControl2.setPageIndicatorTintColor(ca.CAColor4B.WHITE);
        pageControl2.setCurrentPageIndicatorTintColor(ca.CAColor4B.GREEN);
        pageControl2.setTarget(function (index)
        {
            ca.log("index = "+index);
        });
        this.getView().addSubview(pageControl2);

        var pageControl3 = ca.CAPageControl.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout_W_C(150, 0.5), ca.DVerticalLayout_H_C(54, 0.55)));
        pageControl3.setNumberOfPages(5);
        pageControl3.setStyle(ca.CAPageControl.Style.Rectangle);
        pageControl3.setPageIndicatorTintColor(ca.CAColor4B.WHITE);
        pageControl3.setCurrentPageIndicatorTintColor(ca.CAColor4B.GREEN);
        pageControl3.setTarget(function (index)
        {
            ca.log("index = "+index);
        });
        this.getView().addSubview(pageControl3);
    },
});