/**
 * Created by crossApp on 16/8/4.
 */
var CollectionViewTest = ca.CAViewController.extend({

    ctor: function () {
        this._super();

        this.colorArr = new Array();
        this._randomColor() ;

    },
    viewDidLoad: function() {

        var headerRefreshView = ca.CAPullToRefreshView.create(ca.CAPullToRefreshView.Type.Header);
        var footerRefreshView = ca.CAPullToRefreshView.create(ca.CAPullToRefreshView.Type.Footer);

        this.conllection = ca.CACollectionView.createWithLayout(ca.DLayout.fill());
        this.conllection.setAllowsSelection(true);
        this.conllection.setHeaderRefreshView(headerRefreshView);
        this.conllection.setFooterRefreshView(footerRefreshView);
        this.conllection.setHoriInterval(20);
        this.conllection.setVertInterval(20);
        this.getView().addSubview(this.conllection);
                                                    
        this.conllection.onCellAtIndexPath(this.collectionCellAtIndex.bind(this)) ;
        this.conllection.onCellHeightAtIndexPath(this.collectionViewHeightForRowAtIndexPath.bind(this));
        this.conllection.onNumberOfItemsAtIndexPath(this.numberOfItemsInRowsInSection.bind(this)) ;
        this.conllection.onNumberOfRowsAtIndexPath(this.numberOfRowsInSection.bind(this));
        this.conllection.onNumberOfSections(this.numberOfSections.bind(this)) ;
        this.conllection.onDidSelectCellAtIndexPath(this.collectionViewDidSelectCellAtIndexPath.bind(this)) ;
        this.conllection.onDidDeselectCellAtIndexPath(this.collectionViewDidDeselectCellAtIndexPath.bind(this)) ;
        this.conllection.onHeaderBeginRefreshing(this.scrollViewHeaderBeginRefreshing.bind(this)) ;
        this.conllection.onFooterBeginRefreshing(this.scrollViewFooterBeginRefreshing.bind(this)) ;

        this.conllection.reloadData();
        this.conllection.startPullToHeaderRefreshView();
    },

    refreshData1: function(interval){
        this.colorArr = [];
        this._randomColor() ;
        this.conllection.reloadData();
    },

    refreshData2: function(interval){
        this._randomColor() ;
        this.conllection.reloadData();
    },

    _randomColor:function(){
        for (var i = 0; i < 40; i++)
        {
            var r = Math.floor(Math.random()*255);
            var g = Math.floor(Math.random()*255);
            var b = Math.floor(Math.random()*255);

            this.colorArr.push(ca.CAColor4B.set(r, g, b, 255));
        }
    },

    scrollViewHeaderBeginRefreshing: function()
    {
        ca.CAScheduler.getScheduler().scheduleOnce(this.refreshData1.bind(this), "A", this, 0.5);
    },
    scrollViewFooterBeginRefreshing: function()
    {
        ca.CAScheduler.getScheduler().scheduleOnce(this.refreshData2.bind(this), "A", this, 0.5);
    },
    collectionViewDidSelectCellAtIndexPath: function(section, row, item)
    {
        var cell = this.conllection.cellForRowAtIndexPath(section, row, item);
        cell.getContentView().setRotation(-360);
        cell.getContentView().setScale(0.5);
        ca.CAViewAnimation.beginAnimations("");
        cell.getContentView().setRotation(0);
        cell.getContentView().setScale(1.0);
        ca.CAViewAnimation.commitAnimations();
    },
    collectionViewDidDeselectCellAtIndexPath: function(section, row, item)
    {

    },
    collectionCellAtIndex: function(cellSize, section, row, item)
    {
        if (row * 3 + item >= this.colorArr.length)
        {
            return null;
        }

        var _size = ca.DSizeZero;
        var cell = this.conllection.dequeueReusableCellWithIdentifier("cell");
        if (cell == null)
        {
            cell = ca.CACollectionViewCell.create("cell");

            var itemImage = ca.CAView.createWithLayout(ca.DLayout.fill());
            itemImage.setTag(99);
            cell.getContentView().addSubview(itemImage);

            var itemText = ca.CALabel.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout.fill(), ca.DVerticalLayout_T_B(50, 50)));
            itemText.setTag(100);
            itemText.setFontSize(29);
            itemText.setTextAlignment(ca.CATextAlignment.Center);
            itemText.setVerticalTextAlignmet(ca.CAVerticalTextAlignment.Center);
            cell.getContentView().addSubview(itemText);
        }

        var itemImageView = cell.getContentView().getSubviewByTag(99);
        itemImageView.setColor(this.colorArr[row * 3 + item]);

        var pos= section+""+row+""+item;
        var itemText = cell.getContentView().getSubviewByTag(100);
        itemText.setText(pos);

        return cell;
    },
    numberOfSections: function()
    {
        return 1;
    },
    numberOfRowsInSection: function(section)
    {
        return (this.colorArr.length % 3 == 0 ? this.colorArr.length / 3 : this.colorArr.length / 3 + 1);
    },
    numberOfItemsInRowsInSection: function( section, row)
    {
        return 3;
    },
    collectionViewHeightForRowAtIndexPath: function(section, row)
    {
        return (ca.winSize.width - 20 * 4) /3;
    },




});
