/**
 * Created by crossApp on 16/8/8.
 */

var AutoCollectionViewHorizontalTest = ca.CAViewController.extend({

    ctor: function () {
        this._super();
    },

    viewDidLoad: function() {

        this.HorizontalcolorArr = new Array();
        for (var i = 0; i < 40; i++)
        {
            var r = Math.floor(Math.random()*255);
            var g = Math.floor(Math.random()*255);
            var b = Math.floor(Math.random()*255);

            this.HorizontalcolorArr.push(ca.CAColor4B.set(r, g, b, 255));
        }
                                                                  

        this.autoCollection = ca.CAAutoCollectionView.createWithLayout(ca.DLayout.fill());
        this.autoCollection.setAllowsSelection(true);
        this.autoCollection.setAllowsMultipleSelection(true);
        this.autoCollection.setOrientation(ca.CAAutoCollectionView.Orientation.Horizontal);
        this.autoCollection.setHoriCellInterval(20);
        this.autoCollection.setVertCellInterval(20);
        this.autoCollection.setReachBoundaryHandOverToSuperview(false);
        this.getView().addSubview(this.autoCollection);
                                                                  
                                                                  
        this.autoCollection.onDidSelectCellAtIndexPath(this.collectionViewDidSelectCellAtIndexPath.bind(this));
        this.autoCollection.onCellAtIndexPath(this.collectionCellAtIndex.bind(this));
        this.autoCollection.onCellSizeAtIndexPath(this.collectionViewSizeForItemAtIndexPath.bind(this));
        this.autoCollection.onNumberOfItemsAtIndexPath(this.numberOfItemsInSection.bind(this));
        this.autoCollection.onNumberOfSections(this.numberOfSections.bind(this));
    },

    collectionViewDidSelectCellAtIndexPath: function(section, item)
    {
        var cell = this.autoCollection.cellForRowAtIndexPath(section, item);
        cell.getContentView().setRotation(-360);
        cell.getContentView().setScale(0.5);
        ca.CAViewAnimation.beginAnimations("");
        cell.getContentView().setRotation(0);
        cell.getContentView().setScale(1.0);
        ca.CAViewAnimation.commitAnimations();
    },
    collectionCellAtIndex: function(cellSize, section, item)
    {
        var cell = this.autoCollection.dequeueReusableCellWithIdentifier("cell");

        if (cell == null)
        {
            cell = ca.CACollectionViewCell.create("cell");

            var itemImage = ca.CAView.createWithLayout(ca.DLayout.fill());
            itemImage.setTag(99);
            cell.getContentView().addSubview(itemImage);

            var itemText = ca.CALabel.createWithLayout(ca.DLayout.fill());
            itemText.setTag(100);
            itemText.setFontSize(29);
            itemText.setTextAlignment(ca.CATextAlignment.Center);
            itemText.setVerticalTextAlignmet(ca.CAVerticalTextAlignment.Center);
            cell.getContentView().addSubview(itemText);
        }

        var itemImageView = cell.getContentView().getSubviewByTag(99);
        itemImageView.setColor(this.HorizontalcolorArr[item]);
        ca.log("row = "+ item);


        var itemText = cell.getContentView().getSubviewByTag(100);
        itemText.setText("("+section+","+item+")");
        return  cell;

    },

    collectionViewSizeForItemAtIndexPath: function( collectionView, section, item)
    {
        return ca.DSize.set(230, 230);
    },

    numberOfItemsInSection: function(collectionView, section)
    {
        return this.HorizontalcolorArr.length;
    },
    numberOfSections: function(collectionView)
    {
        return 1;
    }

});
