/**
 * Created by zhanglei on 16/8/3.
 */
var Scale9ImageViewTest = ca.CAViewController.extend({
    ctor: function () {
        this._super();
    },
    viewDidLoad: function() {
        var image = ca.CAImageView.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout_L_W(200, 60), ca.DVerticalLayout_T_H(100, 60)));
        image.setImage(ca.CAImage.create("source_material/btn_rounded_normal.png"));
        this.getView().addSubview(image);

        var s9image = ca.CAScale9ImageView.createWithImage(ca.CAImage.create("source_material/btn_rounded_normal.png"));
        s9image.setLayout(ca.DLayout.set(ca.DHorizontalLayout_L_R(200, 200), ca.DVerticalLayout_T_H(400, 200)));
        this.getView().addSubview(s9image);
    },

});