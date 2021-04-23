/**
 * Created by zhanglei on 16/8/3.
 */
var GifViewTest = ca.CAViewController.extend({
    ctor: function () {
        this._super();

    },
    viewDidLoad: function() {
        var GifView = ca.CAGifView.createWithGif(ca.CAGif.create("image/gifview1.gif"));
        GifView.setLayout(ca.DLayout.set(ca.DHorizontalLayout_W_C(400, 0.5), ca.DVerticalLayout_H_C(400, 0.5)));
        //设置Gif永远重复
        GifView.setRepeatForever(true);
        //查看Gif是否永远重复
        log("isRepeatForever =" + GifView.isRepeatForever());

        this.getView().addSubview(GifView);
    },
});