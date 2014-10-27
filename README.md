#### CrossApp 0.3.9 更新内容：  

#####1.CALabel添加文字加粗功能

#####2.CALabel添加文字加粗斜体功能

#####3.CALabel添加文字加下划线功能

#####4.优化DownloadManager
文件下载管理系统优化，并修正上版本头文件引用错误

#####5.重新整理设备相关系统接口
如：GPS获取、摄像头与相册调用、通讯录获取等

#####6.CANavigationController增加二级界面向右划返回上一级界面功能

#####7.优化openGl裁剪功能错位问题

#####8.优化openGl显示图像微差问题

#####9.其他一些细节调整

####升级建议：
重新创建工程，并拷贝Classes与Resources中文件


#### CrossApp 0. 3. 9 Update:

#####1. Added text bold functionality on CALabel

#####2. Added text bold italic functionality on CALabel

#####3. Added text underline functionality on CALabel

#####4. Optimized DownloadManager
Optimized file download management system and fixed header file reference bug from previous release<br/>

#####5. Rearranged device related system interface
For example: GPS info obtaining, camera and photo album calling and address book obtaining etc.<br/>

#####6. Added sliding right on secondary interface to return upper-level interface functionality on CANavigationController

#####7. Optimized OpenGL clipping functionality’s slight error

#####8. Optimized OpenGL imaging coordinates’s slight error

#####9. Other details tweaking

####Suggestion:
rebuild project and copy files in Classes and Resources<br/>


**********************************************************************************


#### CrossApp 0.3.8 更新内容：  

#####1.融合CrossApp-javascript版
基本支持CrossApp 0.3.8的所有功能

#####2.增加CATextView
多行输入控件，用法与CATextField类似

#####3.增加DownloadManager
文件下载管理系统

#####4.增加文字行距接口

#####5.优化文字边缘毛边问题

#####6.优化CAScrollView等滑动容器控件触摸体验

#####7.其他一些细节调整

####升级建议：
重新创建工程，并拷贝Classes与Resources中文件


#### CrossApp 0. 3. 8 Update:

#####1. CrossApp-javascript version integration
Basic support for all functionalities of CrossApp 0. 3. 8<br/>

#####2. Added CATextView
multi-line text input control, its usage is similar with CATextField<br/>

#####3. Added DownloadManager
file download management system<br/>

#####4. Added text line spacing interface

#####5. Text raw edge issue optimization

#####6. Touch experience optimization of CAScrollView and other slide containers control

#####7. Other details tweaking

####Suggestion:
rebuild project and copy files in Classes and Resources<br/>


**********************************************************************************


#### CrossApp 0.3.7 更新内容：  

#####1.优化触摸分发系统

#####2.修复CATabBarController的bug

#####3.修复CATextField的bug

#####4.修改mac平台文字加载机制

#####5.其他一些细节调整

####升级建议：
重新拷贝java文件到android工程中


#### CrossApp 0. 3. 7 Update:

#####1. Touch Distributed System Optimization

#####2. Fixed CATabBarController’s Bug

#####3. Fixed CATextField’s Bug

#####4. Fixed text loading mechanism in Mac platform

#####5. Other details tweaking

####Suggestion:
Recopy java files into android project


**********************************************************************************


#### CrossApp 0.3.6 更新内容：  

#####1.添加CAPullToRefreshView
CAListView、CATableView、CACollectionView等上拉、下拉刷新插件，用法见demo中CollectionViewTest。
CATableView老版上下拉刷新监听代理已移除

#####2.CATabBarController添加左右滑动手势换页效果
需手动设置相关属性开启

#####3.优化触摸事件分发系统
增加支持同方向滑动容器嵌套时子容器滑到头自动切换监听父容器的功能

#####4.优化CAProgress

#####5.优化CAScale9ImageView

#####6.删减CAImageView接口

#####7.修正CACollectionViewDataSource 的代理方法命名

##virtual unsigned int numberOfSectionsInCollectioView()
##改为
##virtual unsigned int numberOfSections(CACollectionView *collectionView)

##virtual unsigned int numberOfRowsInSectionCollectionView(CACollectionView *collectionView, unsigned int section)
##改为
##virtual unsigned int numberOfRowsInSection(CACollectionView *collectionView, unsigned int section)


####升级建议：
注意第7条的修改，如果用到该代理方法需改名与其一致


#### CrossApp.0 3. 6 Update:

##### 1. Added CAPullToRefreshView
Added pull up and pull down refresh plug-ins of CAListView, CATableView and CACollectionView, please view their usage in CollectionViewTest of demo. Removed pull up and pull down refresh monitoring agent of older version CATableView.<br/>

##### 2. Added page turning effect of left and right swipe gestures
You are required to manually set relative properties to launch this effect.<br/>

##### 3. Touch Event Distributed System Optimization
Added functionality that supports system auto switching to monitor parent container after child container slides to end when same direction sliding container is nested.<br/>

##### 4. CAProgress Optimization

##### 5. CAScale9ImageView Optimization

##### 6. Deleted CAImageView Interface

##### 7. Amended Agent Method Naming of CACollectionViewDataSource

##virtual unsigned int numberOfSectionsInCollectioView()<br/>
##is changed as<br/>
##virtual unsigned int numberOfSections(CACollectionView *collectionView)<br/>

##virtual unsigned int numberOfRowsInSectionCollectionView(CACollectionView *collectionView, unsigned int section)<br/>
##is changed as<br/>
##virtual unsigned int numberOfRowsInSection(CACollectionView *collectionView, unsigned int section)<br/>

#### Suggestion:
Note seventh item’s amendment, if you use this agent method, you should follow the same name of it.<br/>



**********************************************************************************


#### CrossApp 0.3.5 更新内容：  

#####1.添加CAStepper
一个数字输入控件,它可以递进式输入数量。

#####2.修改CAAlertView接口
接口重写

#####3.优化CATextField
修改bug，并增加代理监听方法

#####4.优化CAProgress

#####5.优化CASegmentedControl

#####6.优化文字显示

#####7.删除rapidjson

####升级建议：
#1.重新拷贝java文件到android工程中，
#2.xcode需删除extensions目录中rapidjson文件夹
#3.从demo中拷贝新控件默认素材


#### CrossApp 0. 3. 5 Update:

##### 1. Added CAStepper
A number input control which is able to progressively input number.<br/>

##### 2. Modified CAAlertView Interface
Rewrote the interface.<br/>

##### 3. CATextField Optimization
Fixed bug and added agent monitoring method.<br/>

##### 4. CAProgress Optimization

##### 5. CASegmentedControl Optimization

##### 6. Text display Optimization

##### 7. Deleted rapidjson

#### Suggestion:
#1. Recopy java files into android project
#2. In xcode, you need to delete rapidjson folder of extensions directory
#3. Copy default material of new control from demo


**********************************************************************************


#### CrossApp 0.3.4 更新内容：  

#####1.添加CADrawerController
抽屉视图管理器

#####3.添加CAListView
CATableView的简化版，并添加横向排列功能

#####2.添加CAPageControl
pageView页面进度指示器

#####3.优化CAActivityIndicatorView
增加自定义图片接口
增加自定义背景图片接口

#####4.优化CAImageView
添加图片适配方式接口，可支持(铺满、铺满不变形并裁减多余部分、显示全部不变形并居中、显示全部不变形并保持高度不变、显示全部不变形并保持宽度不变)

#####5.优化CATextField
修改设置背景接口

#####6.优化CASegmentedControl
添加接口并修复一些bug

#####7.优化CAPickerView
添加接口并修复一些bug

#####8.其他bug修复与优化



#### CrossApp 0. 3. 4 Update:
##### 1. Added CADrawerController
Drawer view controller<br/>

##### 2. Added CAListView
CATableView’s simplified version, and added horizontal arrangement functionality<br/>

##### 3. Added CAPageControl
pageView page progress indicator<br/>

##### 4. CAActivityIndicatorView Optimization
Added customized image interface, and added customized background image interface<br/>

##### 5. CAImageView Optimization
Added image adaptation mode interface which supports: fully-stretched, fully-stretched with no distortion and cut excess part, fully-displayed with no distortion and centered, fully-displayed with no distortion and stay same height, fully-displayed with no distortion and stay same width.<br/>

##### 6. CATextField Optimization
Modified background setting interface<br/>

##### 7. CASegmentedControl Optimization
Added interfaces and fixed some bugs<br/>

##### 8. CAPickerView Optimization
Added interfaces and fixed some bugs<br/>

##### 9. Fixed and Optimized Other Bugs


**********************************************************************************


#### CrossApp 0.3.3 更新内容：  

#####1.添加CAActivityIndicatorView
加载状态视图

#####2.优化文字性能
增加缓存机制，文字性能再次提升，

#####3.优化CATabBar
增加一些方法与接口

#####4.优化CATableView与CACollectionView
增加section的Header与Footer悬停效果

#####5.优化CATextField
优化并修复一些bug

#####6.其他CAVector，CADeque，CAList
优化一些接口。增加三个容器之间的相互转化

#####7.其他bug修复与优化


#### CrossApp 0. 3. 3 Update:
#####1. Added CAActivityIndicatorView
Load status view.<br/>

#####2. Text Performance Optimization
Added cache mechanism, text performance had another improvement.<br/>

#####3. CATabBar Optimization
Added some methods and interfaces.<br/>

#####4. CATableView and CACollectionView Optimization
Added section’s Header and Footer’s hovering effect.<br/>

#####5. CATextField Optimization
Optimized and fixed some bugs.<br/>

#####6. Other CAVector, CADeque, CAList
Optimized some interfaces. Added the mutual transformation among three containers.<br/>

#####7. Fixed and Optimized other Bugs


**********************************************************************************


#### CrossApp 0.3.2 更新内容：  

#####1.添加md5加密类

#####2.添加CACalendar
时间日期类

#####3.重写CATextField
重写后的textField效率有明显提升

#####4.优化CAPageView
CAPageView增加可监听选中当前页的回调方法

#####5.优化CALabel
增加sizetoFit方法，以便于修改label自己的大小为文字实际大小

#####6.其他bug修复


#### CrossApp 0. 3. 2 Update:

#####1. Added md5 Encryption Class

#####2. Added CACalendar
Date & Time classes<br/>

#####3. Rewrite CATextField
Rewrite-version textField has a better efficiency<br/>

#####4. CAPageView Optimization
Added callmonitoring current selected page of callback method for CAPageView<br/>

#####5. CALabel Optimization
Added sizetoFit method for modifying label’s own size into text actual size<br/>

#####6. Fixed Other Bugs


**********************************************************************************


####【正式版】CrossApp 0.3.1 更新内容：  

#####1.添加CAPickerView
列表选择器，该控件是智能手机中比较常用到的一个控件，它通过转轮界面提供一系列多值选项，它向用户显示信息，也收集用户输入。

#####2.添加CAPageView
翻页滑动容器，该控件是智能手机中比较常用到的一个控件，它通过监听手指滑动手势来体现翻页的效果。

#####3.添加CAIndexPath2E、CAIndexPath3E
二维、三维索引类

#####4.CATableView，CACollectionView增加cell复用机制
复用机制使得CATableView，CACollectionView不再担心超大数据量的显示，且提升用户体验

#####5.优化触摸分发系统
优化触摸分发系统，可更加人性化的分发触摸事件

#####6.httpClient优化
httpClient由原来的一个单独异步线程改为可实现16个异步线程

#####7.修复CAList、CADeque的bug

#####8.修复CATextField在部分机型崩溃的bug



#### CrossApp 0. 3. 1 Update

##### 1. Added CAPickerView
List selector, a commonly used control in smartphone, is able to provide a series of multi-value options via rotative sliding menu, display information to users as well as collecting users’ input.<br/>

##### 2. Added CAPageView
Page turning sliding container, a commonly used control in smartphone, is able to implement page turning effect by monitoring fingers’ swipe gestures.<br/>

##### 3. Added CAIndexPath2E and CAIndexPath3E
Two and three dimensional index class<br/>

##### 4. Added Cell Reuse Mechanism of CATableView and CACollectionView
Reuse mechanism enables CATableView and CACollectionView to bear mass data volume display pressure, and improve UE.<br/>

##### 5. Touch Distributed System Optimization
We optimized touch distributed system, and now it can distribute touch event in a more humanized style.<br/>

##### 6. httpClient Optimization
The previous single asynchronous threaded httpClient is changed into a max 16 asynchronous threaded one<br/>

##### 7. Fixed CAList and CADeque’s Bug

##### 8. Fixed bug that CATextField had crash in some phone models



**********************************************************************************

####【正式版】CrossApp 0.3.0 更新内容：  

#####1.CrossApp-Design发布
CrossApp UI界面编辑器正式推出，这将会大大的加快开发者们开发移动应用的速度。

#####2.大幅优化文字性能
文字机制重写，android平台文字加载速度提升约80%，iOS平台文字加载速度提升约50%。

#####3.优化渲染性能
渲染帧率大幅提升，几乎与原生应用性能近似，部分情况比原生更优（官网中有详细的测试指标的数值文档下载，请查阅）

#####4.优化CANavigationController
优化CANavigationController，大幅度的提升界面管理的效率。

#####5.优化CANavigationBar
添加设置标题与按钮文字颜色的接口。
修复更新背景Image的bug。

#####6.优化CASwitch
针对CASwitch的用户体验进行了优化。

#####7.修复CATableView与CACollectionView
修复CATableView与CACollectionView设置选中项的bug。

#####8.添加对CrossApp-Design导出文件的支持。

#####9.添加新的demo（开发者们反应强烈，所以增加）
新的demo对CrossApp的每一个控件进行了详细的展示，方便大家参考。

#####本版本是CrossApp首次推出正式版本，性能与原生应用近似，已到达可制作商业化产品的开发标准，官方也正式对外准许开发者们使用CrossApp在任何商业移动产品的开发中。
开源的跨平台移动应用开发引擎CrossApp基于MIT开源协议，开发者可以毫无顾虑、完全免费的使用CrossApp开发任何商业项目。



#### CrossApp 0.3.0 Update:  

#####1. CrossApp-Design Release
CorssApp UI interface editor officially releases, which will significantly improve the mobile application development efficiency of developers.

#####2. Great Text Performance Optimization
We rewrote the text mechanism, and the text loading rate on Android was increased by 80% as well as 50% on iOS.

#####3. Rendering Performance Optimization
The rendering frame rate was greatly improved which is almost on the same level with native application performance, and it’s even better in some circumstances. (our official website has the detailed test index value documents, please download and view)

#####4. CANavigationController Optimization
we optimized CANavigationController, and its interface management efficiency was significantly improved.

#####5. CANavigationBar Optimization
Added port for setting title and button text color.
Fixed bug in background Image updating.

#####6. CASwitch Optimization
This CASwitch optimization is specific to UE.

#####7. Fixed CATableView and CACollectionView
Fixed bug in CATableView and CACollectionView selected item setting.

#####8. Added support for CrossApp-Design file exporting.

####9. Added New Demo (a response to our developers’ strong demand)
For your convenience, our new demo contains detailed demonstration for every CrossApp control.

#####Statement: this version is CrossApp’s first official version which is almost on the same level with native application’s performance, and has reached the development standard for making commercial products. Here we officially give permission to developers to produce any commercial mobile products with CrossApp.

Mobile application development engine CrossApp, with open source, cross-platform features, is based on MIT open source protocols, and developers could freely use it to produce any business projects without any misgivings whatsoever.


**********************************************************************************


####CrossApp beta 0.2.1 更新内容：  

#####1.添加容器类
添加CAVector、CADeque、CAList、CAMap、CAMultimap。

#####2.添加控件
添加控件CACollectionView,比CATalbeView 更加复杂的控件诞生，强大的功能可满足很多界面需求。

#####3.添加适配修正参数
添加适配修正参数，以iphone4屏幕为基准，可使其他所有移动设备所表现的视图物理尺寸与iphone4完全一致。

#####4.修复bug
修复触摸事件内存泄露的bug

#####5.修复bug
修复CAAlertView特定情况崩溃的bug

#####6.代码优化
优化CATableView代理方法

#####7.文件补全
添加2.0版本丢失的文件


####CrossApp beta 0.2.1 Update:

#####1.Added container class
Add CAVector, CADeque, CAList, CAMap and CAMultimap.

#####2.Added controls
Add control CACollectionView which is more complicated than CATableView, and its great function could meet many interfaces’ demands.

#####3.Added adaptive corrected parameter
Added adaptive corrected parameter and take iPhone4 as standard, and this parameter enables all other mobile devices’ displayed view physical size are same with iPhone4.

#####4.Fixed bug
Fixed bug that touch event’s memory leak.

#####5.Fixed bug
Fixed bug that CAAlertView had breakdown in particular circumstance.

#####6.Codes optimization
Optimized CATableView agent method.

#####7.Files completion
Added lost files of 2.0 versions.


**********************************************************************************


####CrossApp beta 0.2.0（重要更新） 更新内容：  

#####1.整理引擎目录结构
整理目录结构，改名部分类名。

#####2.触摸事件分发优化
触摸事件分发 现在针对滑动容器优化

#####3.添加控件CAAlertView
提示框，按钮个数三个以内横排，超过三个纵向排列，并放入滑动容器中。

#####4.优化CALabel

#####5.补全CATextField功能

#####6.修复部分bug

#####7.添加一键创建工程

#####8.添加一件部署安卓环境, 下载地址：http://pan.baidu.com/s/1qW6ql32


####CrossApp beta 0.2.0 Update: (Important changes)

#####1.Arranged engine directory structure
Arranged directory structure and changed some class name

#####2.Touch event dispatch optimization
We optimized sliding container of touch event dispatch

#####3.Added CAAlertView control
In prompt box, if button number is less than three, they are arranged in horizontal mode, if the number is greater than three, arranged in vertical mode and put into sliding container.

#####4.CALabel optimization

#####5.Complemented CATextField function

#####6.Fixed some bugs

#####7.Added one-key project building

#####8.Added one-key Android environment deployment, download: http://pan.baidu.com/s/1qW6ql32



####CrossApp alpha 0.1.11（重要更新） 更新内容：  

#####1.引擎默认主题风格变更
新的风格将更加漂亮，切适配性更强。

#####2.CAScale9ImageView重写
由于之前的9宫格有功能缺陷，现重写，新的9宫格可以支持单向缩小。

#####3.CALabel优化
CALabel结构优化，性能优化。将来还会优化。



####CrossApp alpha 0.1.11 Update: (Important changes)

#####1.Default engine theme style change
The new style is more attractive and has a greater suitability.

#####2.CAScale9ImageView rewriting
We rewrote 9 rectangle gird because of the previous version had function defects, and the new version supports unidirectional zoom-out.

#####3.CALabel optimization
We optimized CALabel’s structure and performance, and will do it in the future.



####CrossApp alpha 0.1.10 更新内容：  

#####1.CAImageView优化
优化CAImageView，并修改之前在CAImageView添加子视图显示不正确的bug，现在可随意添加子视图给CAImageView。

#####2.删除ccColor3B，并添加CAColor4B
删除之前的ccColor3B，以前所有用到ccColor3B的地方统统被CAColor4B替换，增加透明度属性。

#####3.删除Opacity属性，并添加新的alpha属性
alpha属性为float型，数值范围为0-1.0f，设置alpha属性可直接影响所有子视图的透明度。

#####4.彻底删除position属性
同样 CCMoveTo、CCMoveBy也被引擎所抛弃

#####5.修复Android平台GPS的bug
修复android平台手机因没有开启gps导致应用程序无法运行的bug

#####6.修复CAScrollView的bug
修复之前代理回调方法virtual void scrollViewWillBeginDragging(CAScrollView* view){};
触发时机错误的bug



####CrossApp alpha 0.1.10 Update:

#####1.CAImageView optimization
Optimized CAImageView, and fixed bug that wrong display problem when adding sub view in CAImageView, now you are free to add sub view in CAImageView.

#####2.Delete ccColor3B and add CAColor4B
Deleted ccColor3B and replace all ccColor3B with CAColor4B, added transparency attribute.

#####3.Deleted Opacity attribute and added new alpha attribute
Alpha attribute is float type with 0 – 1.0f value range, alpha attribute setting can directly influence all sub views’ transparency.

#####4.Completely delete position attribute
CCMoveTo and CCMoveBy are also deleted from our engine

#####5.Fixed the GPS bug of Android platform
Fixed bug that application cannot be started because of Android cellphone did not open GPS

#####6.Fixed CAScrollView bug
Fixed bug that agent callback method ‘void scrollViewWillBeginDragging(CAScrollView* view){};’ has a wrong trigger timing.



####CrossApp alpha 0.1.9 更新内容：  

#####1.触摸事件bug修改
在滑动层上点击按钮瞬间抬起不触发的bug

#####2.CATabBar优化
对CATabBar进行了体验的优化

#####3.CATabBar与CANavigationBar优化
可通过ViewController动态更新其显示数据

#####4.CAButton与CASegmentedControl优化
修改文字显示大小错误的bug

#####6.安卓系统，从后台返回黑屏的bug
之前，在安卓系统中，如果从后台返回，有几率黑屏

#####5.添加动画相关 CCFrameTo 与 CCCenterTo
可以动态的修改 frame与center

#####6.添加viewController悬浮抽屉功能
现在可以调用任何一个viewController的presentModalViewController与dismissModalViewController来显示与撤销。



####CrossApp alpha 0.1.9 Update:

#####1.Touch event bug modification
Fixed bug that Instant release after clicking button on sliding layer is not triggered

#####2.CATabBar optimization
Experience optimization on CATabBar

#####3.CATabBar and CANavigationBar optimization
It’s able to dynamic update CATabBar and CANavigationBar display data via ViewController

#####4.CAButton and CASegmentedControl optimization
Fixed bug that wrong text display size

#####5.Fixed bug that blank screen appears when returning from background in Android system
Previously, if we return from background in Android system, there is certain of probability that blank screen appears

#####6.Added animation related CCFrameTo and CCCenterTo
You can dynamically modify frame and center

#####7.Added suspension drawer function of viewController
You can call any viewController’s presentModalViewController and dismissModalViewController to display and dismiss



####CrossApp alpha 0.1.8 更新内容：  

#####1.
添加CANavigationController左右button的定制

#####2.
CATextField添加win32输入功能

#####3.
添加CANavigationController 替换当前CAViewController的功能

#####4.
修复触摸分发系统在可滑动容器上快速点击按钮无法触发按钮事件的bug

#####6.
修复在win32由于触摸事件造成的偶尔崩溃的bug

#####5.
优化CASegmentedControl

#####6.
优化CANavigationBar在竖屏的高度及布局。



####CrossApp alpha 0.1.8 Update:

#####1.
Added CANavigationController left and right button customization
#####2.
Added win32 input function in CATextField
#####3.
Added CANavigationController, and replace current CAViewController’s function
#####4.
Fixed bug that quickly clicking button on slidable container of touch dispatch system is unable to trigger button event
#####5.
Fixed bug that occasional crashes caused by touch event in win32
#####6.
Optimize CASegmentedControl
#####7.
Optimize CANavigationBar height and layout in portrait mode


####CrossApp alpha 0.1.7 更新内容：  

#####1.
修复CATextField在 win32 与mac平台编译报错的问题

#####2.
修复TabBarController当前viewController不是第一项时 显示与隐藏 tabBar时画面出错的问题


####CrossApp alpha 0.1.7 Update:

#####1.
Fixed CATextField compiling error report problem on win32 and Mac platform.

#####2.
Fixed picture error report problem in displaying and hiding tabBar when current viewController of TabBArController is not the first item.


####CrossApp alpha 0.1.6 更新内容：  
#####1. 添加CASlider：  
滑动条
在此特别鸣谢 9秒ID为juguanhui的 美女程序员，为我们提供了优质的控件源码。

#####2. 添加CASegmentedControl：  
分段选项器
在此特别鸣谢 9秒ID为juguanhui的 美女程序员，为我们提供了优质的控件源码。

#####3. 触摸事件分发规则优化，添加CAResponder类  
经过此次优化，触摸事件分发更加智能，使开发过程变得简单容易。

#####4. CATextField功能补全：  
1.添加输入框选择键盘类型
2.添加遮蔽显示字符为※
3.添加光标移动以及插入删除
4.添加获取键盘高度等功能。
5.目前只支持iOS与Android。

#####5. 添加CATabBarController 、CANavigationController新功能
支持tabBarControll切换viewController动画效果功能
增加 CATabBarController 、CANavigationController 隐藏 其Bar的功能

#####6. 修改部分bug 
1.修正CAView在addSubview之前设置ZOder不起作用的bug
2.修正调用CCDirector中setNotificationNode(CAView* view)不起作用的bug
3.scrollView指示条显示隐藏时机错误的bug

#####7. win32平台向下兼容至VS2012



####CrossApp alpha 0.1.6 Update:
#####1. Added CASlider:
Slider<br/>
Special thanks to a beautiful female programmer (9miao ID: juguanhui) for providing superior controls source codes for us.

#####2. Added CASegmentedControl:
Segmented Controller<br/>
Special thanks to a beautiful female programmer (9miao ID: juguanhui) for providing superior controls source codes for us.

#####3. Touch event dispatch rules optimization, added CAResponder class.
This optimization makes touch event dispatch become more intelligent and development process simple and easy.

#####4. CATextField functions completion:
1.Added keyboard type selecting on text field<br/>
2.Added hiding display character ※<br/>
3.Added cursor movement, insert and delete<br/>
4.Added functions such as obtaining keyboard height and others<br/>
5.Only support iOS and Android for now<br/>

#####5. Added new functions of CATabBarController and CANavigationController
Support tabBarController switching to viewController animation effect<br/>
Added TabBar and NavigationBar hiding function<br/>

#####6. Fixed some bugs
1.Fixed bug: setting ZOder before addSubview in CAView is invalid<br/>
2.Fixed bug: calling setNotificationNode(CAView* view) in CCDirector is invalid<br/>
3.Fixed bug: scrollView indicator's display and hiding timing are wrong<br/>

#####7. Backward compatible to VS2012 on win32 platform



####CrossApp alpha 0.1.5 更新内容：  
#####1. 添加CASwitch：  
在此特别鸣谢 9秒ID为juguanhui的 美女程序员，为我们提供了优质的控件源码。
由于一些原因，juguanhui无法通过git上传代码，所以此次控件源码提交是通过9秒内部上传的。

#####2. 触摸事件分发规则修改：  
新的触摸事件无需注册，只要继承CAView的所有子类，均可通过实现cctouchBegin等函数监听.而且在touchbegin时的视图矩形区域外是无法获得事件监听的。
新的触摸事件是由父视图分发给子视图的，所以当在父视图矩形区域外的touchBegin事件，即使其子视图矩形区域包含这个点，也是无法获取到触摸事件的。

#####3. 增加GPS定位功能：  
目前支持iOS与android平台，wp8无功能

#####4. 增加相册选择照片并返回照片图片数据功能：  
目前支持iOS与android平台，wp8无功能

#####5. 增加从摄像头拍照并返回照片图片数据功能：  
目前支持iOS与android平台，wp8无功能

#####6. 增加获取手机通讯录联系人信息的功能：  
目前支持iOS与android平台，wp8无功能



####CrossApp alpha 0.1.5 Update:
#####1. Added CASwitch:
Special thanks to a beautiful female programmer (9miao ID: juguanhui) for providing superior controls source codes for us. Juguanhui cannot upload codes via git for some reasons, so this time the controls sources code was submitted and uploaded by 9miao worker.

#####2. Touch event dispatch rules modification:
The new touch event does not have to be registered, and you can implement cctouchBegin and other functions monitoring by only inheriting all child classes of CAView. In addition, it’s unable to obtain event monitoring outside of view rectangular region when you touch screen.<br/>

The new touch event is dispatched from parent view to child view, so even if your touchBegin event spot is in child view rectangular region, you cannot obtain touch event on condition that the touchBegin event is outside of parent view rectangular region.<br/>

#####3. Added GPS location function:
Support iOS and Android for now, no WP8.

#####4. Added photo select from album and photo picture data return function:
Support iOS and Android for now, no WP8.

#####5. Added photo taking from webcam and photo picture data return function:
Support iOS and Android for now, no WP8.

#####6. Added mobile address book contacts information obtaining function:
Support iOS and Android for now, no WP8.



####CrossApp alpha 0.1.4 更新内容：  
#####1. 耗电优化：  
由于cocos2d-x的渲染用的渲染驱动模式，程序生命周期中，在不断地重绘，帧数一般在60帧。渲染驱动模式的缺点就是耗电。显而易见，这种不断的重绘方式对于应用程序来说太浪费了。因此，我们针对应用程序的特性，将渲染机制改为事件驱动模式。这种模式的渲染要有外界触发才会重绘，在没有外界触发的时候画面静止，渲染停止，以达到节能的效果  
#####2. CAButton修改：  
增加新的属性 AllowsSelected（包含set，get），默认为false。如果为true，则开启选中模式。在这种模式下。按钮会在选中与默认状态之间切换。即，如果当前为默认状态，按下按钮并抬起，按钮切换到选中状态，再次按下并抬起，则按钮又切换回普通状态。同时，我们可以用getSelected（）方法来及时的获取当前按钮属于2种状态中的哪一种
#####3. CASchedule（经原有CCSchedule修改）：  
**a) 简化用法：**启动停止等操作只需调用相应静态方法即可  
**b) 内存管理修改：**之前启动定时器，针对pTarget的引用计数会+1，以防止因pTarget的释放造成程序崩溃。现在启用了新的机制，已经可以完全避免以上问题，因此在启动定时器后不再对pTarget的引用计数+1，在pTarget释放后自动停止相关定时器  
**c) 新特点：**当pTarget为CAViewController或者其派生类型时，CAViewController的view不显示时，定时器会自动暂停，当重新显示时，会自动恢复。除此之外，在pTarget释放前不会有变化  
#####4. Demo位置变动  
demo由原来的projects文件夹移到samples  



####贡献者名单：juguanhui，


####CrossApp alpha 0.1.4 Update:
#####1.Power Consumption Optimization:
A program is continuously redrawing at general 60 frame rates in its life cycle because of the rendering-driven mode of cocos2d-x rendering, but the shortcoming of this mode is the relatively considerable power consumption. Obviously this continuous redraw mode is quite a waste for program, so we change the rendering mechanism into event-driven mode based on application features. In this rendering mode the redraw is only triggered by outside, if not the screen is static and rendering stops, so as to achieve energy saving goal.
#####2.CAButton Modification:
Added new attribute AllowsSelected (including set, get), default value is false. If value is true then selected mode is started. In this mode, the button status will switch between selected and default: if current status is default one, press button and release and button will switch to selected status; press and release again and button will switch back to default status. However, we can promptly judge and know current button status by using getSelected () method.
#####3.CASchedule (modified based on CCSchedule):
**a)	Simplify usage: ** start, stop and other operation can be acted by calling corresponding static methods.<br/>
**b) Memory management modification: ** previously when we start timer, reference counting on pTarget will plus 1 for the purpose of avoiding program crashed caused by pTarget release. Now the new mechanism we deployed could completely solve the above problem: this mechanism enables system to stop plus 1 on pTarget reference counting after timer starts and auto-stop related timer after pTarget release.<br/>
**c) New feature: ** when pTarget is derived type of CAViewController or others and the view of CAViewController is not displayed, timer will auto pause; it will auto recover when the view is displayed. In addition, it will not change before pTarget releases.<br/>
#####4.Demo Location Change
Demo location is moved to samples folder from projects.



####Contributors: juguanhui，


####【9秒实验室自研】

 CrossApp引擎详细介绍请点此：

<http://www.9miao.com/crossapp/list-24-1.html>

#### 9miao Lab Original

 Click to view CrossApp engine detailed description:<br/>
<http://www.9miao.com/crossapp/list-24-1.html>
 
####近期要做的控件：  
提示框：CAAlertView  
视图分页控制器：CAPageControl  
大段文字输入控件：CATextView  
加载状态控制器：CAActivity  
网页加载控件：CAWebView  

####Recent Controls Writing Plan:
Reminder: CAAlertView<br/>
View paging controller: CAPageControl<br/>
Blocks of text input control: CATextView<br/>
Load status controller: CAActivity<br/>
Webpage load control: CAWebView<br/>

####目前已有控件缺失功能：  
#####1.	textField：  	
**a)	**文字内容长度自由限制  
**b)	**光标移动到文字内容任意位置编辑功能  
**c)	**换行输入功能
#####2.	tableView：
**a)	**cell的复用  
**b)	**cell编辑功能（删除、插入等）  
**c)	**cell的一些模板属性的添加  

####Existed Controls Missing Features:
#####1.	textField：
**a) ** free control on text content length<br/>
**b) ** text editing when cursor is at anywhere of content<br/>
**c) **line feed input<br/>
#####2.	tableView：
**a) **cell reuse<br/>
**b) **cell editing (delete, insert and others)<br/>
**c) **added some template attributes of cell<br/>


=======
####【正式版】CrossApp 0.3.1 更新内容：  

#####1.添加CAPickerView
列表选择器，该控件是智能手机中比较常用到的一个控件，它通过转轮界面提供一系列多值选项，它向用户显示信息，也收集用户输入。

#####2.添加CAPageView
翻页滑动容器，该控件是智能手机中比较常用到的一个控件，它通过监听手指滑动手势来体现翻页的效果。

#####3.添加CAIndexPath2E、CAIndexPath3E
二维、三维索引类

#####4.CATableView，CACollectionView增加cell复用机制
复用机制使得CATableView，CACollectionView不再担心超大数据量的显示，且提升用户体验

#####5.优化触摸分发系统
优化触摸分发系统，可更加人性化的分发触摸事件

#####6.httpClient优化
httpClient由原来的一个单独异步线程改为可实现16个异步线程

#####7.修复CAList、CADeque的bug

#####8.修复CATextField在部分机型崩溃的bug


#### CrossApp 0.3.1 Update:

#####1. Added CAPickerView
List selector, a commonly used control in smartphone, is able to provide a series of multi-value options via rotative sliding menu, display information to users as well as collecting users’ input.

#####2. Added CAPageView
Page turning sliding container, a commonly used control in smartphone, is able to implement page turning effect by monitoring fingers’ swipe gestures.

#####3. Added CAIndexPath2E and CAIndexPath3E
Two and three dimensional index class

#####4. Added Cell Reuse Mechanism of CATableView and CACollectionView
Reuse mechanism enables CATableView and CACollectionView to bear mass data volume display pressure, and improve UE.

#####5. Touch Distributed System Optimization
We optimized touch distributed system, and now it can distribute touch event in a more humanized style.

#####6. httpClient Optimization
The previous single asynchronous threaded httpClient is changed into a max 16 asynchronous threaded one

#####7. Fixed CAList and CADeque’s Bug

#####8. Fixed bug that CATextField had crash in some phone models




####【正式版】CrossApp 0.3.0 更新内容：  

#####1.CrossApp-Design发布
CrossApp UI界面编辑器正式推出，这将会大大的加快开发者们开发移动应用的速度。

#####2.大幅优化文字性能
文字机制重写，android平台文字加载速度提升约80%，iOS平台文字加载速度提升约50%。

#####3.优化渲染性能
渲染帧率大幅提升，几乎与原生应用性能近似，部分情况比原生更优（官网中有详细的测试指标的数值文档下载，请查阅）

#####4.优化CANavigationController
优化CANavigationController，大幅度的提升界面管理的效率。

#####5.优化CANavigationBar
添加设置标题与按钮文字颜色的接口。
修复更新背景Image的bug。

#####6.优化CASwitch
针对CASwitch的用户体验进行了优化。

#####7.修复CATableView与CACollectionView
修复CATableView与CACollectionView设置选中项的bug。

#####8.添加对CrossApp-Design导出文件的支持。

#####9.添加新的demo（开发者们反应强烈，所以增加）
新的demo对CrossApp的每一个控件进行了详细的展示，方便大家参考。

#####本版本是CrossApp首次推出正式版本，性能与原生应用近似，已到达可制作商业化产品的开发标准，官方也正式对外准许开发者们使用CrossApp在任何商业移动产品的开发中。
开源的跨平台移动应用开发引擎CrossApp基于MIT开源协议，开发者可以毫无顾虑、完全免费的使用CrossApp开发任何商业项目。



#### CrossApp 0.3.0 Update:  

#####1. CrossApp-Design Release
CorssApp UI interface editor officially releases, which will significantly improve the mobile application development efficiency of developers.

#####2. Great Text Performance Optimization
We rewrote the text mechanism, and the text loading rate on Android was increased by 80% as well as 50% on iOS.

#####3. Rendering Performance Optimization
The rendering frame rate was greatly improved which is almost on the same level with native application performance, and it’s even better in some circumstances. (our official website has the detailed test index value documents, please download and view)

#####4. CANavigationController Optimization
we optimized CANavigationController, and its interface management efficiency was significantly improved.

#####5. CANavigationBar Optimization
Added port for setting title and button text color.
Fixed bug in background Image updating.

#####6. CASwitch Optimization
This CASwitch optimization is specific to UE.

#####7. Fixed CATableView and CACollectionView
Fixed bug in CATableView and CACollectionView selected item setting.

#####8. Added support for CrossApp-Design file exporting.

####9. Added New Demo (a response to our developers’ strong demand)
For your convenience, our new demo contains detailed demonstration for every CrossApp control.

#####Statement: this version is CrossApp’s first official version which is almost on the same level with native application’s performance, and has reached the development standard for making commercial products. Here we officially give permission to developers to produce any commercial mobile products with CrossApp.

Mobile application development engine CrossApp, with open source, cross-platform features, is based on MIT open source protocols, and developers could freely use it to produce any business projects without any misgivings whatsoever.


**********************************************************************************


####CrossApp beta 0.2.1 更新内容：  

#####1.添加容器类
添加CAVector、CADeque、CAList、CAMap、CAMultimap。

#####2.添加控件
添加控件CACollectionView,比CATalbeView 更加复杂的控件诞生，强大的功能可满足很多界面需求。

#####3.添加适配修正参数
添加适配修正参数，以iphone4屏幕为基准，可使其他所有移动设备所表现的视图物理尺寸与iphone4完全一致。

#####4.修复bug
修复触摸事件内存泄露的bug

#####5.修复bug
修复CAAlertView特定情况崩溃的bug

#####6.代码优化
优化CATableView代理方法

#####7.文件补全
添加2.0版本丢失的文件


####CrossApp beta 0.2.1 Update:

#####1.Added container class
Added CAVector, CADeque, CAList, CAMap and CAMultimap.

#####2.Add controls
Added control CACollectionView which is more complicated than CATableView, and its great function could meet many interfaces’ demands.

#####3.Added adaptive corrected parameter
Added adaptive corrected parameter and take iPhone4 as standard, and this parameter enables all other mobile devices’ displayed view physical size are same with iPhone4.

#####4.Fixed bug
Fixed bug that touch event’s memory leak.

#####5.Fixed bug
Fixed bug that CAAlertView had breakdown in particular circumstance.

#####6.Codes optimization
Optimized CATableView agent method.

#####7.Files completion
Added lost files of 2.0 versions.


**********************************************************************************


####CrossApp beta 0.2.0（重要更新） 更新内容：  

#####1.整理引擎目录结构
整理目录结构，改名部分类名。

#####2.触摸事件分发优化
触摸事件分发 现在针对滑动容器优化

#####3.添加控件CAAlertView
提示框，按钮个数三个以内横排，超过三个纵向排列，并放入滑动容器中。

#####4.优化CALabel

#####5.补全CATextField功能

#####6.修复部分bug

#####7.添加一键创建工程

#####8.添加一件部署安卓环境, 下载地址：http://pan.baidu.com/s/1qW6ql32


####CrossApp beta 0.2.0 Update: (Important changes)

#####1.Arranged engine directory structure
Arranged directory structure and changed some class name

#####2.Touch event dispatch optimization
We optimized sliding container of touch event dispatch

#####3.Added CAAlertView control
In prompt box, if button number is less than three, they are arranged in horizontal mode, if the number is greater than three, arranged in vertical mode and put into sliding container.

#####4.CALabel optimization

#####5.Complemented CATextField function

#####6.Fixed some bugs

#####7.Added one-key project building

#####8.Added one-key Android environment deployment, download: http://pan.baidu.com/s/1qW6ql32



####CrossApp alpha 0.1.11（重要更新） 更新内容：  

#####1.引擎默认主题风格变更
新的风格将更加漂亮，切适配性更强。

#####2.CAScale9ImageView重写
由于之前的9宫格有功能缺陷，现重写，新的9宫格可以支持单向缩小。

#####3.CALabel优化
CALabel结构优化，性能优化。将来还会优化。



####CrossApp alpha 0.1.11 Update: (Important changes)

#####1.Default engine theme style change
The new style is more attractive and has a greater suitability.

#####2.CAScale9ImageView rewriting
We rewrote 9 rectangle gird because of the previous version had function defects, and the new version supports unidirectional zoom-out.

#####3.CALabel optimization
We optimized CALabel’s structure and performance, and will do it in the future.



####CrossApp alpha 0.1.10 更新内容：  

#####1.CAImageView优化
优化CAImageView，并修改之前在CAImageView添加子视图显示不正确的bug，现在可随意添加子视图给CAImageView。

#####2.删除ccColor3B，并添加CAColor4B
删除之前的ccColor3B，以前所有用到ccColor3B的地方统统被CAColor4B替换，增加透明度属性。

#####3.删除Opacity属性，并添加新的alpha属性
alpha属性为float型，数值范围为0-1.0f，设置alpha属性可直接影响所有子视图的透明度。

#####4.彻底删除position属性
同样 CCMoveTo、CCMoveBy也被引擎所抛弃

#####5.修复Android平台GPS的bug
修复android平台手机因没有开启gps导致应用程序无法运行的bug

#####6.修复CAScrollView的bug
修复之前代理回调方法virtual void scrollViewWillBeginDragging(CAScrollView* view){};
触发时机错误的bug



####CrossApp alpha 0.1.10 Update:

#####1.CAImageView optimization
Optimized CAImageView, and fixed bug that wrong display problem when adding sub view in CAImageView, now you are free to add sub view in CAImageView.

#####2.Deleted ccColor3B and added CAColor4B
Deletet ccColor3B and replace all ccColor3B with CAColor4B, added transparency attribute.

#####3.Deleted Opacity attribute and added new alpha attribute
Alpha attribute is float type with 0 – 1.0f value range, alpha attribute setting can directly influence all sub views’ transparency.

#####4.Completely delete position attribute
CCMoveTo and CCMoveBy are also deleted from our engine

#####5.Fixed the GPS bug of Android platform
Fixed bug that application cannot be started because of Android cellphone did not open GPS

#####6.Fixed CAScrollView bug
Fixed bug that agent callback method ‘void scrollViewWillBeginDragging(CAScrollView* view){};’ has a wrong trigger timing.



####CrossApp alpha 0.1.9 更新内容：  

#####1.触摸事件bug修改
在滑动层上点击按钮瞬间抬起不触发的bug

#####2.CATabBar优化
对CATabBar进行了体验的优化

#####3.CATabBar与CANavigationBar优化
可通过ViewController动态更新其显示数据

#####4.CAButton与CASegmentedControl优化
修改文字显示大小错误的bug

#####6.安卓系统，从后台返回黑屏的bug
之前，在安卓系统中，如果从后台返回，有几率黑屏

#####5.添加动画相关 CCFrameTo 与 CCCenterTo
可以动态的修改 frame与center

#####6.添加viewController悬浮抽屉功能
现在可以调用任何一个viewController的presentModalViewController与dismissModalViewController来显示与撤销。



####CrossApp alpha 0.1.9 Update:

#####1.Touch event bug modification
Fixed bug that Instant release after clicking button on sliding layer is not triggered

#####2.CATabBar optimization
Experience optimization on CATabBar

#####3.CATabBar and CANavigationBar optimization
It’s able to dynamic update CATabBar and CANavigationBar display data via ViewController

#####4.CAButton and CASegmentedControl optimization
Fixed bug that wrong text display size

#####5.Fixed bug that blank screen appears when returning from background in Android system
Previously, if we return from background in Android system, there is certain of probability that blank screen appears

#####6.Added animation related CCFrameTo and CCCenterTo
You can dynamically modify frame and center

#####7.Added suspension drawer function of viewController
You can call any viewController’s presentModalViewController and dismissModalViewController to display and dismiss



####CrossApp alpha 0.1.8 更新内容：  

#####1.
添加CANavigationController左右button的定制

#####2.
CATextField添加win32输入功能

#####3.
添加CANavigationController 替换当前CAViewController的功能

#####4.
修复触摸分发系统在可滑动容器上快速点击按钮无法触发按钮事件的bug

#####6.
修复在win32由于触摸事件造成的偶尔崩溃的bug

#####5.
优化CASegmentedControl

#####6.
优化CANavigationBar在竖屏的高度及布局。



####CrossApp alpha 0.1.8 Update:

#####1.
Added CANavigationController left and right button customization
#####2.
Added win32 input function in CATextField
#####3.
Added CANavigationController, and replace current CAViewController’s function
#####4.
Fixed bug that quickly clicking button on slidable container of touch dispatch system is unable to trigger button event
#####5.
Fixed bug that occasional crashes caused by touch event in win32
#####6.
Optimize CASegmentedControl
#####7.
Optimize CANavigationBar height and layout in portrait mode


####CrossApp alpha 0.1.7 更新内容：  

#####1.
修复CATextField在 win32 与mac平台编译报错的问题

#####2.
修复TabBarController当前viewController不是第一项时 显示与隐藏 tabBar时画面出错的问题


####CrossApp alpha 0.1.7 Update:

#####1.
Fixed CATextField compiling error report problem on win32 and Mac platform.

#####2.
Fixed picture error report problem in displaying and hiding tabBar when current viewController of TabBArController is not the first item.


####CrossApp alpha 0.1.6 更新内容：  
#####1. 添加CASlider：  
滑动条
在此特别鸣谢 9秒ID为juguanhui的 美女程序员，为我们提供了优质的控件源码。

#####2. 添加CASegmentedControl：  
分段选项器
在此特别鸣谢 9秒ID为juguanhui的 美女程序员，为我们提供了优质的控件源码。

#####3. 触摸事件分发规则优化，添加CAResponder类  
经过此次优化，触摸事件分发更加智能，使开发过程变得简单容易。

#####4. CATextField功能补全：  
1.添加输入框选择键盘类型
2.添加遮蔽显示字符为※
3.添加光标移动以及插入删除
4.添加获取键盘高度等功能。
5.目前只支持iOS与Android。

#####5. 添加CATabBarController 、CANavigationController新功能
支持tabBarControll切换viewController动画效果功能
增加 CATabBarController 、CANavigationController 隐藏 其Bar的功能

#####6. 修改部分bug 
1.修正CAView在addSubview之前设置ZOder不起作用的bug
2.修正调用CCDirector中setNotificationNode(CAView* view)不起作用的bug
3.scrollView指示条显示隐藏时机错误的bug

#####7. win32平台向下兼容至VS2012



####CrossApp alpha 0.1.6 Update:
#####1. Added CASlider:
Slider<br/>
Special thanks to a beautiful female programmer (9miao ID: juguanhui) for providing superior controls source codes for us.

#####2. Added CASegmentedControl:
Segmented Controller<br/>
Special thanks to a beautiful female programmer (9miao ID: juguanhui) for providing superior controls source codes for us.

#####3. Touch event dispatch rules optimization, add CAResponder class.
This optimization makes touch event dispatch become more intelligent and development process simple and easy.

#####4. CATextField functions completion:
1.Added keyboard type selecting on text field<br/>
2.Added hiding display character ※<br/>
3.Added cursor movement, insert and delete<br/>
4.Added functions such as obtaining keyboard height and others<br/>
5.Only support iOS and Android for now<br/>

#####5. Added new functions of CATabBarController and CANavigationController
Support tabBarController switching to viewController animation effect<br/>
Added TabBar and NavigationBar hiding function<br/>

#####6. Fixed some bugs
1.Fixed bug: setting ZOder before addSubview in CAView is invalid<br/>
2.Fixed bug: calling setNotificationNode(CAView* view) in CCDirector is invalid<br/>
3.Fixed bug: scrollView indicator's display and hiding timing are wrong<br/>

#####7. Backward compatible to VS2012 on win32 platform



####CrossApp alpha 0.1.5 更新内容：  
#####1. 添加CASwitch：  
在此特别鸣谢 9秒ID为juguanhui的 美女程序员，为我们提供了优质的控件源码。
由于一些原因，juguanhui无法通过git上传代码，所以此次控件源码提交是通过9秒内部上传的。

#####2. 触摸事件分发规则修改：  
新的触摸事件无需注册，只要继承CAView的所有子类，均可通过实现cctouchBegin等函数监听.而且在touchbegin时的视图矩形区域外是无法获得事件监听的。
新的触摸事件是由父视图分发给子视图的，所以当在父视图矩形区域外的touchBegin事件，即使其子视图矩形区域包含这个点，也是无法获取到触摸事件的。

#####3. 增加GPS定位功能：  
目前支持iOS与android平台，wp8无功能

#####4. 增加相册选择照片并返回照片图片数据功能：  
目前支持iOS与android平台，wp8无功能

#####5. 增加从摄像头拍照并返回照片图片数据功能：  
目前支持iOS与android平台，wp8无功能

#####6. 增加获取手机通讯录联系人信息的功能：  
目前支持iOS与android平台，wp8无功能



####CrossApp alpha 0.1.5 Update:
#####1. Added CASwitch:
Special thanks to a beautiful female programmer (9miao ID: juguanhui) for providing superior controls source codes for us. Juguanhui cannot upload codes via git for some reasons, so this time the controls sources code was submitted and uploaded by 9miao worker.

#####2. Touch event dispatch rules modification:
The new touch event does not have to be registered, and you can implement cctouchBegin and other functions monitoring by only inheriting all child classes of CAView. In addition, it’s unable to obtain event monitoring outside of view rectangular region when you touch screen.<br/>

The new touch event is dispatched from parent view to child view, so even if your touchBegin event spot is in child view rectangular region, you cannot obtain touch event on condition that the touchBegin event is outside of parent view rectangular region.<br/>

#####3. Added GPS location function:
Support iOS and Android for now, no WP8.

#####4. Added photo select from album and photo picture data return function:
Support iOS and Android for now, no WP8.

#####5. Added photo taking from webcam and photo picture data return function:
Support iOS and Android for now, no WP8.

#####6. Added mobile address book contacts information obtaining function:
Support iOS and Android for now, no WP8.



####CrossApp alpha 0.1.4 更新内容：  
#####1. 耗电优化：  
由于cocos2d-x的渲染用的渲染驱动模式，程序生命周期中，在不断地重绘，帧数一般在60帧。渲染驱动模式的缺点就是耗电。显而易见，这种不断的重绘方式对于应用程序来说太浪费了。因此，我们针对应用程序的特性，将渲染机制改为事件驱动模式。这种模式的渲染要有外界触发才会重绘，在没有外界触发的时候画面静止，渲染停止，以达到节能的效果  
#####2. CAButton修改：  
增加新的属性 AllowsSelected（包含set，get），默认为false。如果为true，则开启选中模式。在这种模式下。按钮会在选中与默认状态之间切换。即，如果当前为默认状态，按下按钮并抬起，按钮切换到选中状态，再次按下并抬起，则按钮又切换回普通状态。同时，我们可以用getSelected（）方法来及时的获取当前按钮属于2种状态中的哪一种
#####3. CASchedule（经原有CCSchedule修改）：  
**a) 简化用法：**启动停止等操作只需调用相应静态方法即可  
**b) 内存管理修改：**之前启动定时器，针对pTarget的引用计数会+1，以防止因pTarget的释放造成程序崩溃。现在启用了新的机制，已经可以完全避免以上问题，因此在启动定时器后不再对pTarget的引用计数+1，在pTarget释放后自动停止相关定时器  
**c) 新特点：**当pTarget为CAViewController或者其派生类型时，CAViewController的view不显示时，定时器会自动暂停，当重新显示时，会自动恢复。除此之外，在pTarget释放前不会有变化  
#####4. Demo位置变动  
demo由原来的projects文件夹移到samples  



####贡献者名单：juguanhui，


####CrossApp alpha 0.1.4 Update:
#####1.Power Consumption Optimization:
A program is continuously redrawing at general 60 frame rates in its life cycle because of the rendering-driven mode of cocos2d-x rendering, but the shortcoming of this mode is the relatively considerable power consumption. Obviously this continuous redraw mode is quite a waste for program, so we change the rendering mechanism into event-driven mode based on application features. In this rendering mode the redraw is only triggered by outside, if not the screen is static and rendering stops, so as to achieve energy saving goal.
#####2.CAButton Modification:
Added new attribute AllowsSelected (including set, get), default value is false. If value is true then selected mode is started. In this mode, the button status will switch between selected and default: if current status is default one, press button and release and button will switch to selected status; press and release again and button will switch back to default status. However, we can promptly judge and know current button status by using getSelected () method.
#####3.CASchedule (modified based on CCSchedule):
**a)	Simplify usage: ** start, stop and other operation can be acted by calling corresponding static methods.<br/>
**b) Memory management modification: ** previously when we start timer, reference counting on pTarget will plus 1 for the purpose of avoiding program crashed caused by pTarget release. Now the new mechanism we deployed could completely solve the above problem: this mechanism enables system to stop plus 1 on pTarget reference counting after timer starts and auto-stop related timer after pTarget release.<br/>
**c) New feature: ** when pTarget is derived type of CAViewController or others and the view of CAViewController is not displayed, timer will auto pause; it will auto recover when the view is displayed. In addition, it will not change before pTarget releases.<br/>
#####4.Demo Location Change
Demo location is moved to samples folder from projects.



####Contributors: juguanhui，


####【9秒实验室自研】

 CrossApp引擎详细介绍请点此：

<http://www.9miao.com/crossapp/list-24-1.html>

#### 9miao Lab Original

 Click to view CrossApp engine detailed description:<br/>
<http://www.9miao.com/crossapp/list-24-1.html>
 
####近期要做的控件：  
提示框：CAAlertView  
视图分页控制器：CAPageControl  
大段文字输入控件：CATextView  
加载状态控制器：CAActivity  
网页加载控件：CAWebView  

####Recent Controls Writing Plan:
Reminder: CAAlertView<br/>
View paging controller: CAPageControl<br/>
Blocks of text input control: CATextView<br/>
Load status controller: CAActivity<br/>
Webpage load control: CAWebView<br/>

####目前已有控件缺失功能：  
#####1.	textField：  	
**a)	**文字内容长度自由限制  
**b)	**光标移动到文字内容任意位置编辑功能  
**c)	**换行输入功能
#####2.	tableView：
**a)	**cell的复用  
**b)	**cell编辑功能（删除、插入等）  
**c)	**cell的一些模板属性的添加  

####Existed Controls Missing Features:
#####1.	textField：
**a) ** free control on text content length<br/>
**b) ** text editing when cursor is at anywhere of content<br/>
**c) **line feed input<br/>
#####2.	tableView：
**a) **cell Reuse<br/>
**b) **cell editing (delete, insert and others)<br/>
**c) **added some template attributes of cell<br/>
>>>>>>> a0fca0f78baf5e0a811792d99d995b5c43b26d7a
