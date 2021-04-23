var FirstViewController = ca.CAViewController.extend({
    ctor: function () {
        this._super();
    },
    viewDidLoad: function () {

        var img = ca.CAImageView.createWithImage(ca.CAImage.create("r/HelloWorld.png"));
        img.setLayout(ca.DLayout.fill());
        img.setScaleType(1);
        this.getView().addSubview(img);

        var font = ca.CAFont.create();
        font.color = ca.CAColor4B.WHITE;
        font.fontSize = 72;

        var labelLayout = ca.DLayout.set(ca.DHorizontalLayout_L_R(20, 20),ca.DVerticalLayout_T_H(200, 100));
        var label = ca.CALabel.createWithLayout(labelLayout);
        label.setTextAlignment(ca.CATextAlignment.Center);
        label.setVerticalTextAlignmet(ca.CAVerticalTextAlignment.Center);
        label.setFont(font);
        this.getView().addSubview(label);
        label.setText("CrossApp");
    },
    viewDidUnload: function () {
        ca.log("viewDidUnload");
    }
});
