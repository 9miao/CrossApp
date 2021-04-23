/**
 * Created by zhanglei on 16/8/8.
 */
var ScrollViewTest = ca.CAViewController.extend({

    ctor: function () {
        this._super();
    },

    viewDidLoad: function() {

        this.colorArr = new Array();

        this.scrollView = ca.CAScrollView.createWithLayout(ca.DLayout.fill());
        this.scrollView.setMinimumZoomScale(0.2);
        this.scrollView.setMaximumZoomScale(5.0);
        this.scrollView.setMultitouchGesture(ca.CAScrollView.MultitouchGesture.ZoomAndRotate);
        this.scrollView.setViewSize(ca.DSize.set(2160,3840));
        this.scrollView.setReachBoundaryHandOverToSuperview(false);
        this.getView().addSubview(this.scrollView);

        this.p_imageView = ca.CAImageView.createWithImage(ca.CAImage.create("image/h1.png"));
        this.p_imageView.setLayout(ca.DLayout.fill());
        this.scrollView.addSubview(this.p_imageView);

        this.scrollView.onDidMoved(this.scrollViewDidMoved.bind(this));
        this.scrollView.onStopMoved(this.scrollViewStopMoved.bind(this));
        this.scrollView.onWillBeginDragging(this.scrollViewWillBeginDragging.bind(this));
        this.scrollView.onDragging(this.scrollViewDragging.bind(this));
        this.scrollView.onDidEndDragging(this.scrollViewDidEndDragging.bind(this));
        this.scrollView.onDidZoom(this.scrollViewDidZoom.bind(this));
        this.scrollView.onTouchUpWithoutMoved(this.scrollViewTouchUpWithoutMoved.bind(this));
    },

    scrollViewDidMoved: function (view)
    {
        log("DidMoved-->");
    },

    scrollViewStopMoved: function ( view)
    {
        log("StopMoved-->");
    },

    scrollViewWillBeginDragging: function ( view)
    {
        log("BeginDragging-->");
    },

    scrollViewDragging: function ( view)
    {
        log("BeginDragging-->");
    },

    scrollViewDidEndDragging: function ( view)
    {
        log("DidEndDragging-->");
    },

    scrollViewDidZoom: function ( view)
    {
        log("DidZoom-->");
    },

    scrollViewTouchUpWithoutMoved: function ( view)
    {
        log("DidEndDragging-->");
    }
});
