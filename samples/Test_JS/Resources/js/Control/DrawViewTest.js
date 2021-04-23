/**
 * Created by crossApp on 16/8/8.
 */

var DrawViewTest = ca.CAViewController.extend({

    ctor: function () {
        this._super();
    },

    viewDidLoad: function() {
        var drawView = ca.CADrawView.create() ;
        drawView.setLayout(ca.DLayout.set(ca.DHorizontalLayout_L_R(0,0) , ca.DVerticalLayout_T_B(0,40)));
        this.getView().addSubview(drawView) ;

        var win_wid = ca.winSize.width ;

        /** 线 */
        drawView.drawLine(ca.DPoint.set(win_wid * 0.2, 50), ca.DPoint.set(win_wid * 0.8 ,50),ca.CAColor4B.BLUE) ;

        //point (left-bottom , right-bottom , right-top , left-top)

        /** 矩形 */
        drawView.drawRect(ca.DPoint.set(win_wid * 0.25,100) , ca.DPoint.set(win_wid * 0.75, 100) , ca.DPoint.set(win_wid * 0.75 , 220) , ca.DPoint.set(win_wid * 0.25 , 220)  , ca.CAColor4B.RED) ;

        /** 实心多边形 */
        drawView.drawSolidCircle(ca.DPoint.set(win_wid * 0.5 , 400) , 100 , 270 ,5  , 1 , 1 , ca.CAColor4B.YELLOW) ;

        /** 斜线 */
        drawView.drawSegment(ca.DPoint.set(win_wid * 0.3 , 600) , ca.DPoint.set(win_wid * 0.7 ,600) , 5.0, ca.CAColor4B.BLUE) ;

        /** 带中线的圆 */
        drawView.drawCircle(ca.DPoint.set(win_wid * 0.3 , 800), 30 , 270 , 360 , true , 3 , 3 , ca.CAColor4B.MAGENTA) ;

        /** 不带中线的圆 */
        drawView.drawCircle(ca.DPoint.set(win_wid * 0.7 , 800), 30 , 270 , 360 , false , 3 , 3 , ca.CAColor4B.MAGENTA) ;

        /* 菱形 */
        drawView.drawCircle(ca.DPoint.set(win_wid * 0.5 , 990), 30 , 270 , 4 , false , 3 , 3 , ca.CAColor4B.ORANGE) ;



    },


});
