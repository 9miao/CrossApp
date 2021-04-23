/**
 * Created by crossApp on 16/8/8.
 */

var ListViewTest = ca.CAViewController.extend({
    ctor: function () {
        this._super();
    },
    viewDidLoad: function() {

        this.listview1 = ca.CAListView.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout.fill(), ca.DVerticalLayout_T_H(0, 120)));

        list1DataSourceDelegate.init(this.listview1) ;

        this.getView().addSubview(this.listview1);


        this.listview2 = ca.CAListView.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout.fill(), ca.DVerticalLayout_T_B(122,0)));

        list2DataSourceDelegate.init(this.listview2) ;

        this.getView().addSubview(this.listview2);

    },

});

var list1DataSourceDelegate = {

    init:function(listview){
        this.listview = listview ;

        this.listview.setAllowsSelection(true);
        this.listview.setAllowsMultipleSelection(false);
        this.listview.setOrientation(ca.CAListView.Orientation.Horizontal); //(ca.CAListView.orientation.horizontal);
        this.listview.setShowsHorizontalScrollIndicator(false);
        this.listview.setSeparatorColor(ca.CAColor4F.CLEAR);
        this.listview.setReachBoundaryHandOverToSuperview(false);

        this.listview.onDidSelectCellAtIndexPath(this.listViewDidSelectCellAtIndex.bind(this));
        this.listview.onDidDeselectCellAtIndexPath(this.listViewDidDeselectCellAtIndex.bind(this));
        this.listview.onNumberOfRowsAtIndexPath(this.numberOfIndex.bind(this));
        this.listview.onCellHeightAtIndexPath(this.listViewHeightForIndex.bind(this));
        this.listview.onWillDisplayCellAtIndexPath(this.listViewWillDisplayCellAtIndex.bind(this)) ;
        this.listview.onCellAtIndexPath(this.listViewCellAtIndex.bind(this)) ;
    },

    numberOfIndex: function () {
        return 30;
    },
    listViewHeightForIndex: function ( index) {
        return 120;
    },
    listViewCellAtIndex: function (cellSize, index) {
        var cell = this.listview.dequeueReusableCellWithIdentifier("ListViewCell");
        if (cell == null) {
            cell = ca.CAListViewCell.create("ListViewCell");

            var test = ca.CALabel.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout_L_W(0, 200), ca.DVerticalLayout.fill()));
            test.setColor(ca.CAColor4B.set(51, 204, 255, 255));
            test.setTextAlignment(ca.CATextAlignment.Center);
            test.setVerticalTextAlignmet(ca.CAVerticalTextAlignment.Center);
            test.setFontSize(28);
            test.setTag(100);
            cell.addSubview(test);
        }

        var test = cell.getSubviewByTag(100);
        test.setText(""+index);
        return cell;
    },
    listViewWillDisplayCellAtIndex: function ( cell, index) {
        return 0;
    },

    listViewDidSelectCellAtIndex: function ( index) {
    },
    listViewDidDeselectCellAtIndex: function ( index) {
    }
}

var list2DataSourceDelegate = {


    init:function(listview){
        this.listview = listview ;

        this.listview.setAllowsSelection(true);
        this.listview.setAllowsMultipleSelection(false);
        this.listview.setOrientation(ca.CAListView.Orientation.Vertical); //(ca.CAListView.orientation.horizontal);
        this.listview.setShowsHorizontalScrollIndicator(false);
        this.listview.setSeparatorColor(ca.CAColor4F.CLEAR);


        this.listview.onDidSelectCellAtIndexPath(this.listViewDidSelectCellAtIndex.bind(this));
        this.listview.onDidDeselectCellAtIndexPath(this.listViewDidDeselectCellAtIndex.bind(this));
        this.listview.onNumberOfRowsAtIndexPath(this.numberOfIndex.bind(this));
        this.listview.onCellHeightAtIndexPath(this.listViewHeightForIndex.bind(this));
        this.listview.onWillDisplayCellAtIndexPath(this.listViewWillDisplayCellAtIndex.bind(this)) ;
        this.listview.onCellAtIndexPath(this.listViewCellAtIndex.bind(this)) ;
    },

    numberOfIndex: function () {
        return 30;
    },
    listViewHeightForIndex: function ( index) {
        return 100;
    },
    listViewCellAtIndex: function ( cellSize, index) {
        var cell = this.listview.dequeueReusableCellWithIdentifier("ListViewCell");
        if (cell == null) {
            cell = ca.CAListViewCell.create("ListViewCell");

            var test = ca.CALabel.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout_L_W(0, 200), ca.DVerticalLayout.fill()));
            test.setColor(ca.CAColor4B.set(51, 204, 255, 255));
            test.setTextAlignment(ca.CATextAlignment.Center);
            test.setVerticalTextAlignmet(ca.CAVerticalTextAlignment.Center);
            test.setFontSize(28);
            test.setTag(100);
            cell.addSubview(test);

            var btn = ca.CAButton.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout_W_C(100, 0.85), ca.DVerticalLayout_H_C(50, 0.5)), ca.CAButton.Type.SquareRect);
            btn.setTitleForState(ca.CAControl.State.Normal, "btn");
            btn.setTag(200);
            cell.addSubview(btn);
            btn.setTouchEventScrollHandOverToSuperview(false);
        }

        var test = cell.getSubviewByTag(100);
        test.setText("cell-"+index);
        return cell;
    },
    listViewWillDisplayCellAtIndex: function ( cell, index) {
    },

    listViewDidSelectCellAtIndex: function ( index) {
    },
    listViewDidDeselectCellAtIndex: function ( index) {
    }
}





