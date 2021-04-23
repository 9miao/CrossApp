
var ActivityIndicatorViewTest = ca.CAViewController.extend({
    ctor: function (num) {
        this._super();
    },
    viewDidLoad: function() {

        var idc1 = ca.CAActivityIndicatorView.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout.fill(), ca.DVerticalLayout_NH_C(0.2, 0.167)));
        idc1.setStyle(ca.CAActivityIndicatorView.Style.WhiteLarge);
        idc1.startAnimating();
        this.getView().addSubview(idc1);


        var idc2 = ca.CAActivityIndicatorView.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout.fill(), ca.DVerticalLayout_NH_C(0.2, 0.334)));
        idc2.setStyle(ca.CAActivityIndicatorView.Style.GrayLarge);
        idc2.startAnimating();
        this.getView().addSubview(idc2);


        var idc3 = ca.CAActivityIndicatorView.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout.fill(), ca.DVerticalLayout_NH_C(0.2, 0.5)));
        idc3.setStyle(ca.CAActivityIndicatorView.Style.White);
        idc3.setCycleTime(1.0);
        idc3.setTimesOneCycle(12);
        idc3.startAnimating();
        this.getView().addSubview(idc3);


        var idc4 = ca.CAActivityIndicatorView.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout.fill(), ca.DVerticalLayout_NH_C(0.2, 0.667)));
        idc4.setStyle(ca.CAActivityIndicatorView.Style.Gray);
        idc4.setCycleTime(1.0);
        idc4.setTimesOneCycle(12);
        idc4.startAnimating();
        this.getView().addSubview(idc4);


        var imageView = ca.CAImageView.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout_L_R(100, 100), ca.DVerticalLayout_T_B(100,100)));
        imageView.setFrame(ca.DRect.set(0, 0, 100, 100));
        imageView.setImage(ca.CAImage.create("image/indicator2.png"));

        var idc5 = ca.CAActivityIndicatorView.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout.fill(), ca.DVerticalLayout_NH_C(0.2, 0.833)));
        idc5.setStyle(ca.CAActivityIndicatorView.Style.Image);
        idc5.setActivityIndicatorView(imageView);
        idc5.startAnimating();
        this.getView().addSubview(idc5);
    },
});
