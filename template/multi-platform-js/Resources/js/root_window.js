require('js/first_view_controller.js');

var RootWindow = ca.CAWindow.extend(
    {
        ctor: function () {
            this._super();

            var firstViewController = new FirstViewController();
            firstViewController.init();
            this.setRootViewController(firstViewController);
            firstViewController.release();
        }
    }
);
