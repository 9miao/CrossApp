/**
 * Created by crossApp on 16/8/8.
 */


var MyWebViewController = ca.CAViewController.extend({

    ctor: function (url) {
        this._super();
        this.url = url;
    },

    viewDidLoad: function() {

        this.webView = ca.CAWebView.createWithLayout(ca.DLayout.fill());
        this.webView.loadURL(this.url);
        this.getView().addSubview(this.webView);
    },

});
