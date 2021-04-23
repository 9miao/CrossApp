

var RootWindow = ca.CAWindow.extend(
    {
        m_pRootNavigationController:null,
        ctor: function () {
            this._super();

            var viewController = new FirstViewController();

            var controllers = [];
            for(var i=0; i<3; i++)
            {
                controllers.push(new FirstViewController());
            }

            var firstItem = ca.CANavigationBarItem.create("控件展示" + "<" + ca.winSize.width + "x" + ca.winSize.height + ">");
            var barButtonItem = ca.CABarButtonItem.create("", ca.CAImage.create("image/ic_category_list.png"), null);
            barButtonItem.setCallbackFunction(function (){
                this.drawer.showLeftViewController(true);
            }.bind(this));
            firstItem.addLeftButtonItem(barButtonItem);

            this.tabBarConttoller = new ca.CATabBarController();
            this.tabBarConttoller.initWithViewControllers(controllers);
            this.tabBarConttoller.setNavigationBarItem(firstItem);

            this.m_pRootNavigationController = new ca.CANavigationController();
            this.m_pRootNavigationController.initWithRootViewController(this.tabBarConttoller);


            this.drawer = new ca.CADrawerController();
            this.drawer.initWithController(new MenuViewController(), this.m_pRootNavigationController);
            this.drawer.autorelease();
            this.drawer.setBackgroundImage(ca.CAImage.create("image/bg.jpg"));
            this.drawer.setEffect3D(true);

            this.setRootViewController(this.drawer);

        },
        getRootNavigationController:function(){
            return this.m_pRootNavigationController;
        },
        getDrawerController:function () {
            return this.drawer;
        },
    }
);
