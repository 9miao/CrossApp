/**
 * Created by crossApp on 16/8/8.
 */
var WaterfallViewTest = ca.CAViewController.extend({

    ctor: function () {
        this._super();

        this.colorArr = new Array();
        this.heightArr = new Array();
        this._randomColor() ;

    },
    viewDidLoad: function() {

        var headerRefreshView = ca.CAPullToRefreshView.create(ca.CAPullToRefreshView.Type.Header);
        var footerRefreshView = ca.CAPullToRefreshView.create(ca.CAPullToRefreshView.Type.Footer);

        this.Waterfall = ca.CAWaterfallView.createWithLayout(ca.DLayout.fill());
        this.Waterfall.setHeaderRefreshView(headerRefreshView) ;
        this.Waterfall.setFooterRefreshView(footerRefreshView) ;
        this.Waterfall.setItemMargin(10);
        this.Waterfall.setColumnMargin(10);
        this.Waterfall.setColumnCount(2);
        this.Waterfall.setAllowsSelection(true);
        this.getView().addSubview(this.Waterfall);

        this.Waterfall.onCellAtIndexPath(this.waterfallCellAtIndex.bind(this)) ;
        this.Waterfall.onCellHeightAtIndexPath(this.waterfallViewHeightForItemAtIndex.bind(this));
        this.Waterfall.onNumberOfItemsAtIndexPath(this.numberOfItems.bind(this)) ;
        this.Waterfall.onWillDisplayCellAtIndexPath(this.waterfallViewWillDisplayCellAtIndex.bind(this)) ;
        this.Waterfall.onDidSelectCellAtIndexPath(this.waterfallViewDidSelectCellAtIndexPath.bind(this)) ;
        this.Waterfall.onDidDeselectCellAtIndexPath(this.waterfallViewDidDeselectCellAtIndexPath.bind(this)) ;
        this.Waterfall.onHeaderBeginRefreshing(this.scrollViewHeaderBeginRefreshing.bind(this)) ;
        this.Waterfall.onFooterBeginRefreshing(this.scrollViewFooterBeginRefreshing.bind(this)) ;


    },

    _randomColor:function(){
        for (var i = 0; i < 12; i++)
        {
            var r = Math.floor(Math.random()*255);
            var g = Math.floor(Math.random()*255);
            var b = Math.floor(Math.random()*255);

            this.colorArr.push(ca.CAColor4B.set(r, g, b, 255));
            this.heightArr.push(Math.random() * 300 + 300);
        }
    },
                                                   
    refreshData1: function(interval){
        this.colorArr = [];
        this.heightArr = [];
        this._randomColor() ;
        this.Waterfall.reloadData();
    },
                                                   
    refreshData2: function(interval){
        this._randomColor() ;
        this.Waterfall.reloadData();
    },
                                                   
    scrollViewHeaderBeginRefreshing: function ()
    {
        ca.CAScheduler.getScheduler().scheduleOnce(this.refreshData1.bind(this), "scrollViewHeaderBeginRefreshing",this, 0.5);
    },
                                                   
    scrollViewFooterBeginRefreshing: function ()
    {
        ca.CAScheduler.getScheduler().scheduleOnce(this.refreshData2.bind(this), "scrollViewFooterBeginRefreshing",this, 0.5);
    },
                                                   
    waterfallViewDidSelectCellAtIndexPath: function (itemIndex)
    {
        //选中
        cell = this.Waterfall.cellForRowAtIndexPath(itemIndex);
        cell.getContentView().setRotation(-360);
        cell.getContentView().setScale(0.5);
        ca.CAViewAnimation.beginAnimations("");
        cell.getContentView().setRotation(0);
        cell.getContentView().setScale(1.0);
        ca.CAViewAnimation.commitAnimations();
        ca.log("选中");
    },
                                                   
    waterfallViewDidDeselectCellAtIndexPath: function (itemIndex)
    {
        ca.log("取消选中");
    },

    waterfallCellAtIndex: function (cellSize, itemIndex)
    {
        cell = this.Waterfall.dequeueReusableCellWithIdentifier("CrossApp");
        if (cell == null)
        {
            cell = ca.CAWaterfallViewCell.create("CrossApp");

            var itemImage = ca.CAView.createWithLayout(ca.DLayout.fill());
            itemImage.setTag(99);
            cell.getContentView().addSubview(itemImage);

            var itemText = ca.CALabel.createWithLayout(ca.DLayout.fill());
            itemText.setTag(100);
            itemText.setFontSize(24);
            itemText.setTextAlignment(ca.CATextAlignment.Center);
            itemText.setVerticalTextAlignmet(ca.CAVerticalTextAlignment.Center);
            cell.getContentView().addSubview(itemText);
        }

        //设置Item背景颜色
        var itemImageView = cell.getContentView().getSubviewByTag(99);
        itemImageView.setColor(this.colorArr[itemIndex]);

        var itemText = cell.getContentView().getSubviewByTag(100);
        itemText.setText("("+itemIndex+")");

        return  cell;
    },
    //Necessary
    waterfallViewHeightForItemAtIndex: function( itemIndex)
    {
        return this.heightArr[itemIndex];
    },
    //Necessary
    numberOfItems: function (waterfallView)
    {
        return this.colorArr.length;
    },

    waterfallViewWillDisplayCellAtIndex: function ( cell, itemIndex)
    {

    }
});
