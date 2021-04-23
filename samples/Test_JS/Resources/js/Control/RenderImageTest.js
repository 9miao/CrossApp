/**
 * Created by crossApp on 16/8/8.
 */

var RenderImageTest = ca.CAViewController.extend({

    ctor: function () {
        this._super();
    },

    viewDidLoad: function() {

        var image = ca.CAImage.create("r/HelloWorld.png");
        this.imageView = ca.CAImageView.createWithImage(image) ;
        this.imageView.setLayout(ca.DLayout.set(ca.DHorizontalLayout_W_C(400, 0.5) , ca.DVerticalLayout_H_C(500, 0.5))) ;
        this.imageView.setScaleType(ca.CAImageView.ScaleType.FitImageInside) ;
        this.getView().addSubview(this.imageView) ;

        this.button = ca.CAButton.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout_W_C(200, 0.5) , ca.DVerticalLayout_B_H(20, 60)) , ca.CAButton.Type.RoundedRect) ;
        this.button.setTitleForState(ca.CAControl.State.Normal,"截取") ;
        this.getView().addSubview(this.button) ;
        this.button.addTarget(this._onButtonEvent.bind(this) , ca.CAButton.Event.TouchUpInSide) ;

    },

    _onButtonEvent:function(){

        var layer = ca.CAView.createWithLayout(ca.DLayout.fill()) ;
        layer.setColor(ca.CAColor4B.WHITE) ;
        layer.setAlpha(0) ;
        ca.rootWindow.insertSubview(layer , 0xffff) ;

        ca.CAViewAnimation.beginAnimations("");
        ca.CAViewAnimation.setAnimationDuration(0.1);
        var imgview = this.imageView ;
        ca.CAViewAnimation.setAnimationDidStopSelector(function()
        {
            var winSize = ca.winSize;
            var render =  ca.CARenderImage.create(winSize.width , winSize.height , ca.CAImage.PixelFormat.RGBA8888);
            render.printscreenWithView(ca.rootWindow.getRootViewController().getView()) ;
            var img = render.getImageView().getImage()  ;
            imgview.setImage(img) ;

            layer.removeFromSuperview();

        });
        layer.setAlpha(255) ;
        ca.CAViewAnimation.commitAnimations();
    },

    getStencil: function(size, index)
    {
        if (index == 0)
        {
            var ver = new Array();
            ver[0] = ca.DPoint.set(0,0) ;
            ver[1] = ca.DPoint.set(0, size.height);
            ver[2] = ca.DPoint.set(size.width, size.height);
            ver[3] = ca.DPoint.set(size.width, 0);
            var stencil = ca.CADrawView.create();
            stencil.drawPolygon(ver, 4, ca.CAColor4B.set(255, 0, 0, 0), 2, ca.CAColor4B.set(255, 0, 0, 0));
            stencil.setFrameOrigin(ca.DPoint.set(0, size.height));
            return stencil;
        }
        else if (index == 1)
        {
            var cir = new Array(720);
            for (var i=0; i<720; i++)
            {
                var x = Math.cos(i * Math.PI/180) * size.width/2;
                var y = Math.sin(i * Math.PI/180) * size.width/2;
                cir[i] = ca.p(x, y);
            }
            var stencil = ca.CADrawView.create();
            stencil.drawPolygon(cir, 720, ca.CAColor4B.set(1, 1, 1, 0.5), 0, ca.CAColor4B.set(1, 1, 1, 0));
            stencil.setCenterOrigin(ca.DPoint.set(size.width/2, size.height/2));
            return stencil;
        }
        return null;
    }
});
