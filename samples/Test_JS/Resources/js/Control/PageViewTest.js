/**
 * Created by crossApp on 16/8/3.
 */
var PageViewTest = ca.CAViewController.extend({

    ctor: function () {
        this._super();
    },

    viewDidLoad: function() {

        var view1 = ca.CAImageView.createWithImage(ca.CAImage.create("image/1.jpg"));
        var view2 = ca.CAImageView.createWithImage(ca.CAImage.create("image/2.jpg"));
        var view3 = ca.CAImageView.createWithImage(ca.CAImage.create("image/3.jpg"));
        var views = new Array();
        views.push(view1);
        views.push(view2);
        views.push(view3);

        this._pageView = ca.CAPageView.createWithLayout(ca.DLayout.fill(), 0);
        this._pageView.setViews(views);
        this.getView().addSubview(this._pageView);

        this._pageView.onBeginTurning(this.pageViewDidBeginTurning.bind(this)) ;
        this._pageView.onEndTurning(this.pageViewDidEndTurning.bind(this)) ;
        this._pageView.onDidSelectedPageAtIndex(this.pageViewDidSelectPageAtIndex.bind(this)) ;


        this.pageControl = ca.CAPageControl.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout_W_C(100, 0.5), ca.DVerticalLayout_T_H(30, 50)));
        this.pageControl.setNumberOfPages(views.length);
        this.pageControl.setPageIndicatorTintColor(ca.CAColor4B.GRAY);
        this.pageControl.setCurrentPageIndicatorTintColor(ca.CAColor4B.GREEN);
        this.getView().addSubview(this.pageControl);

        var pageContro = this.pageControl;
        var pageView = this._pageView;
        this.pageControl.setTarget(function(index)
        {
            ca.log("Button RoundedRect");
            ca.log("btn_tag==="+pageContro.getCurrentPage());
            pageView.setCurrPage(pageContro.getCurrentPage(), true);
        });
    },

    pageViewDidBeginTurning: function () {

        ca.log("pageViewDidBeginTurning()" );

    },
    pageViewDidEndTurning: function () {
        this.pageControl.setCurrentPage(this._pageView.getCurrPage());
        this.pageControl.updateCurrentPageDisplay();
        ca.log("pageViewDidEndTurning() ---  this._pageView.getCurrPage() = " + this._pageView.getCurrPage() );
    },
    pageViewDidSelectPageAtIndex: function (index, point) {
        ca.log("pageViewDidSelectPageAtIndex("+index+"),("+point+")");
    }


});
