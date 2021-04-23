/**
 * Created by zhanglei on 16/8/4.
 */
var WebViewTest = ca.CAViewController.extend({
    ctor: function () {
        this._super();
    },
    viewDidLoad: function() {
        var web = ca.CAWebView.createWithLayout(ca.DLayout.fill());
        web.loadURL("http://www.baidu.com");
        this.getView().addSubview(web);
    },
});
