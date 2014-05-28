
####CrossApp alpha 0.1.4 更新内容：  
#####1. 耗电优化：  
由于cocos2d-x的渲染用的渲染驱动模式，程序生命周期中，在不断地重绘，帧数一般在60帧。渲染驱动模式的缺点就是耗电。显而易见，这种不断的重绘方式对于应用程序来说太浪费了。因此，我们针对应用程序的特性，将渲染机制改为事件驱动模式。这种模式的渲染要有外界触发才会重绘，在没有外界触发的时候画面静止，渲染停止，以达到节能的效果  
#####2. CAButton修改：  
增加新的属性 AllowsSelected（包含set，get），默认为false。如果为true，则开启选中模式。在这种模式下。按钮会在选中与默认状态之间切换。即，如果当前为默认状态，按下按钮并抬起，按钮切换到选中状态，再次按下并抬起，则按钮又切换回普通状态。同时，我们可以用getSelected（）方法来及时的获取当前按钮属于2种状态中的哪一种
######3. CASchedule（经原有CCSchedule修改）：  
**a) 简化用法：**启动停止等操作只需调用相应静态方法即可  
**b) 内存管理修改：**之前启动定时器，针对pTarget的引用计数会+1，以防止因pTarget的释放造成程序崩溃。现在启用了新的机制，已经可以完全避免以上问题，因此在启动定时器后不再对pTarget的引用计数+1，在pTarget释放后自动停止相关定时器  
**c) 新特点：**当pTarget为CAViewController或者其派生类型时，CAViewController的view不显示时，定时器会自动暂停，当重新显示时，会自动恢复。除此之外，在pTarget释放前不会有变化  
#####4. Demo位置变动  
demo有原来的projects文件夹移到samples  



####【9秒实验室自研】

 CrossApp引擎详细介绍请点此：

 <http://www.9miao.com/crossapp/list-24-1.html>


####近期要做的控件：  
滑动条： CASlider  
状态按钮：CASwitch  
分段选项按钮：CASegmentedControl  
提示框：CAAlertView  
视图分页控制器：CAPageControl  
大段文字输入控件：CATextView  
加载状态控制器：CAActivity  
网页加载控件：CAWebView  

####目前已有控件缺失功能：  
#####1.	textField：  	
**a)	**文字内容长度自由限制  
**b)	**光标移动到文字内容任意位置编辑功能  
**c)	**换行输入功能
#####2.	tableView：
**a)	**cell的复用  
**b)	**cell编辑功能（删除、插入等）  
**c)	**cell的一些模板属性的添加  
