/**
 * Created by crossApp on 16/8/8.
 */

var ClipViewTest = ca.CAViewController.extend({

    ctor: function () {
        this._super();
    },

    viewDidLoad: function() {

        var clipView = ca.CAClippingView.create() ;
        clipView.setLayout(ca.DLayout.set(ca.DHorizontalLayout_L_R(0,0) , ca.DVerticalLayout_T_B(0,0)));
        clipView.setClippingEnabled(true) ;
        //clipView.setInverted(true);
        this.getView().addSubview(clipView) ;


        var win_wid = ca.winSize.width ;

        var drawView = ca.CADrawView.create() ;
        drawView.setLayout(ca.DLayout.fill()) ;
        drawView.drawSolidCircle(ca.DPoint.set(win_wid * 0.5 , 700) , win_wid * 0.4 , 270 , 5  , 1 , 1 , ca.CAColor4B.YELLOW) ;
        clipView.setStencil(drawView) ;

        var scrllVeiw = ca.CAScrollView.createWithLayout(ca.DLayout.fill()) ;
        scrllVeiw.setViewSize(ca.DSize.set(1080, 1920)) ;
        var imgView = ca.CAImageView.createWithImage(ca.CAImage.create("r/HelloWorld.png")) ;
        imgView.setLayout(ca.DLayout.fill()) ;
        scrllVeiw.addSubview(imgView) ;
        clipView.addSubview(scrllVeiw) ;

    },

});
