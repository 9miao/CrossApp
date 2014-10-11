require('js/header.js');
var d = new thirdViewController();
d.setTitle("FourthViewController");
d.init();

var e = new thirdViewController();
e.setTitle("FifthViewController");
e.init();

var b= new thirdViewController();
b.setTitle("SecondViewController");
b.init();

var c= new thirdViewController();
c.setTitle("ThridViewController");
c.init();

var a= new thirdViewController();
a.setTitle("FirstViewController");
a.init();

var arr = Array();

var nav = new ca.NavigationController();
nav.initWithRootViewController(a);
nav.setTabBarItem(ca.TabBarItem.create("Third",ca.Image.create("tabbar_function/first_1.png"),ca.Image.create("tabbar_function/first_2.png")));
var navb = new ca.NavigationController();
navb.initWithRootViewController(b);
navb.setTabBarItem(ca.TabBarItem.create("Second",ca.Image.create("tabbar_function/second_1.png"),ca.Image.create("tabbar_function/second_2.png")));

var navc = new ca.NavigationController();
navc.initWithRootViewController(c);
navc.setTabBarItem(ca.TabBarItem.create("First",ca.Image.create("tabbar_function/third_1.png"),ca.Image.create("tabbar_function/third_2.png")));

var navd = new ca.NavigationController();
navd.initWithRootViewController(d);
navd.setTabBarItem(ca.TabBarItem.create("Fourth",ca.Image.create("tabbar_function/fourth_1.png"),ca.Image.create("tabbar_function/fourth_2.png")));

var nave = new ca.NavigationController();
nave.initWithRootViewController(e);
nave.setTabBarItem(ca.TabBarItem.create("Fifth",ca.Image.create("tabbar_function/fifth_1.png"),ca.Image.create("tabbar_function/fifth_2.png")));

arr.push(navc);
arr.push(navb);
arr.push(nav);
arr.push(navd);
arr.push(nave);

var dir = ca.Application.getApplication();
var window = ca.Window.create();
var tabbar = new ca.TabBarController();
tabbar.initWithViewControllers(arr);

var drawer = new DrawerController();
drawer.init();

var drawerController = new ca.DrawerController();
drawerController.initWithController(drawer,tabbar,300);
window.setRootViewController(drawerController);
dir.runWindow(window);