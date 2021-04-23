/**
 * Created by crossApp on 16/8/3.
 */

var TabBarControllerTest = ca.CAViewController.extend({

    ctor: function () {
        this._super();
    },

    viewDidLoad: function() {

        var item1 = ca.CATabBarItem.create("item1", ca.CAImage.create(""), ca.CAImage.create(""));
        var item2 = ca.CATabBarItem.create("item2", ca.CAImage.create(""), ca.CAImage.create(""));
        var item3 = ca.CATabBarItem.create("item3", ca.CAImage.create(""), ca.CAImage.create(""));

        var item = new Array;
        item.push(item1);
        item.push(item2);
        item.push(item3);

        var tabBar = ca.CATabBar.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout.fill(), ca.DVerticalLayout_H_C(98,0.33)));
        tabBar.setItems(item);
        tabBar.showSelectedIndicator();
        tabBar.setSelectedAtIndex(1);
        this.getView().addSubview(tabBar);
    },
});
