/**
 * Created by zhanglei on 16/8/2.
 */
var ImageViewTest = ca.CAViewController.extend({

    ctor: function () {
        this._super();
        this._imageViewNum = 0;
    },

    viewDidLoad: function() {
        this.initImageView();
    },

    initImageView:function(){
        log("ImageViewText-->imageViewNum:"+this._imageViewNum);

        this.getView().removeAllSubviews();

        if (this._imageViewNum == 0)
        {
            var image1 = ca.CAImageView.createWithImage(ca.CAImage.create("image/h1.png"));
            image1.setLayout(ca.DLayout.fill());

            var view1 = ca.CAView.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout_L_R(50, 50), ca.DVerticalLayout_T_B(50, 50)));
            view1.addSubview(image1);
            this.getView().addSubview(view1);
        }
        else if (this._imageViewNum == 1)
        {
            var image2 = ca.CAImageView.createWithLayout(ca.DLayout.fill());

            image2.setImage(ca.CAImage.create("image/h1.png"));
            image2.setScaleType(ca.CAImageView.ScaleType.FitImageXY);
            var view2 = ca.CAView.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout_L_R(50, 50), ca.DVerticalLayout_T_B(50, 50)));
            view2.addSubview(image2);
            this.getView().addSubview(view2);
        }
        else if (this._imageViewNum == 2)
        {
            var image3 = ca.CAImageView.createWithLayout(ca.DLayout.fill());
            image3.setImage(ca.CAImage.create("image/h1.png"));
            image3.setScaleType(ca.CAImageView.ScaleType.FitImageCrop);

            var view3 = ca.CAView.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout_L_R(50, 50), ca.DVerticalLayout_T_B(50, 50)));
            view3.addSubview(image3);
            this.getView().addSubview(view3);
        }
        else if (this._imageViewNum == 3)
        {
            var image4 = ca.CAImageView.createWithLayout(ca.DLayout.fill());
            image4.setImage(ca.CAImage.create("image/h1.png"));
            image4.setScaleType(ca.CAImageView.ScaleType.FitImageInside);


            var view4 = ca.CAView.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout_L_R(50, 50), ca.DVerticalLayout_T_B(50, 50)));
            view4.addSubview(image4);
            this.getView().addSubview(view4);
        }
        else if (this._imageViewNum == 4)
        {
            var img = new Array(3);
            for (var i=0; i<8; i++)
            {

                var str = "animation/npc_382-"+ (i+1) +".png";
                img[i] = ca.CAImage.create(str);
            }
            var image7 = ca.CAImageView.createWithImage(ca.CAImage.create("animation/npc_382-1.png"));
            image7.setLayout(ca.DLayout.fill());
            image7.setAnimationImages(img);
            image7.setAnimationDuration(0.1);
            image7.startAnimating();

            var view7 = ca.CAView.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout_L_R(100, 100), ca.DVerticalLayout_T_B(200, 200)));
            view7.addSubview(image7);
            this.getView().addSubview(view7);
        }

        var btn = ca.CAButton.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout_NW_C(0.3,0.5) , ca.DVerticalLayout_B_H(20 ,50)),ca.CAButton.Type.RoundedRect);
        btn.setTitleForState(ca.CAControl.State.Normal, "Switch Next");
        btn.addTarget(this.btncallback.bind(this), ca.CAButton.Event.TouchUpInSide);
        this.getView().addSubview(btn);
    },

    btncallback: function ()
    {
        this._imageViewNum += 1;

        if(this._imageViewNum > 4)
        {
            this._imageViewNum = 0;
        }
        this.initImageView();
    }
});
