/**
 * Created by crossApp on 16/8/8.
 */


var TableViewTest = ca.CAViewController.extend({

    ctor: function () {
        this._super();
    },

    viewDidLoad: function() {
        this.sectionTitle = ["A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"];

        this.tableView = ca.CATableView.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout_L_R(0, 0), ca.DVerticalLayout.fill()));
        this.tableView.setAllowsSelection(true);
        this.tableView.setAllowsMultipleSelection(false);
        this.tableView.setShowsScrollIndicators(false);
        this.tableView.setSelectRowAtIndexPath(2, 1);
        this.getView().addSubview(this.tableView);
                                               
        this.tableView.onDidSelectCellAtIndexPath(this.tableViewDidSelectRowAtIndexPath.bind(this)) ;
        this.tableView.onDidDeselectCellAtIndexPath(this.tableViewDidDeselectRowAtIndexPath.bind(this)) ;
        this.tableView.onCellAtIndexPath(this.tableCellAtIndex.bind(this)) ;
        this.tableView.onSectionViewForHeaderInSection(this.tableViewSectionViewForHeaderInSection.bind(this)) ;
        this.tableView.onSectionViewForFooterInSection(this.tableViewSectionViewForFooterInSection.bind(this)) ;
        this.tableView.onNumberOfRowsAtIndexPath(this.numberOfRowsInSection.bind(this));
        this.tableView.onNumberOfSections(this.numberOfSections.bind(this)) ;
        this.tableView.onCellHeightAtIndexPath(this.tableViewHeightForRowAtIndexPath.bind(this));
        this.tableView.onHeightForHeaderInSection(this.tableViewHeightForHeaderInSection.bind(this));
        this.tableView.onHeightForFooterInSection(this.tableViewHeightForFooterInSection.bind(this)) ;

        

    },
    tableViewDidSelectRowAtIndexPath: function(section ,row)
    {

    },
    tableViewDidDeselectRowAtIndexPath: function( section ,row)
    {

    },
    tableCellAtIndex: function( cellSize, section ,row)
    {
        var cell = this.tableView.dequeueReusableCellWithIdentifier("CrossApp");
        if (cell == null)
        {
            cell = ca.CATableViewCell.create("CrossApp");
            var image = ca.CAImageView.createWithLayout(ca.DLayout.fill());
            image.setImage(ca.CAImage.create("source_material/second_2.png"));
            cell.getContentView().addSubview(image);

            var cellText = ca.CALabel.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout_L_R(150, 10), ca.DVerticalLayout_T_B(10, 10)));
            cellText.setTag(100);
            cellText.setFontSize(30);
            cellText.setTextAlignment(ca.CATextAlignment.Left);
            cellText.setVerticalTextAlignmet(ca.CAVerticalTextAlignment.Center);
            cell.getContentView().addSubview(cellText);

            var deleteBtn = ca.CAButton.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout_R_W(0, 180), ca.DVerticalLayout.fill()), ca.CAButton.Type.Custom);
            deleteBtn.setBackgroundImageForState(ca.CAControl.State.Normal, ca.CAImage.createWithColor4B(ca.CAColor4B.RED), false);
            deleteBtn.setBackgroundImageForState(ca.CAControl.State.Highlighted, ca.CAImage.createWithColor4B(ca.CAColor4B.ORANGE), false);
            deleteBtn.setTitleForState(ca.CAControl.State.Normal, "Delete");
            deleteBtn.setTitleColorForState(ca.CAControl.State.Normal, ca.CAColor4B.WHITE);
            deleteBtn.setTitleColorForState(ca.CAControl.State.Highlighted, ca.CAColor4B.WHITE);
            deleteBtn.setTitleFontSize(32);
            deleteBtn.setTag(101);
            cell.insertSubview(deleteBtn, -1);
        }
        cell.setDraggingLength(180);
                                               
        var cellText = cell.getContentView().getSubviewByTag(100);
        cellText.setText(this.sectionTitle[section]+"-"+row);

        var deleteBtn = cell.getSubviewByTag(101);
        deleteBtn.addTarget(function ()
        {
            ca.log("button at (section: %d, row: %d)", section, row);

        }, ca.CAButton.Event.TouchUpInSide);

        return cell;

    }
    ,tableViewSectionViewForHeaderInSection: function(viewSize ,section)
    {
        var head = this.sectionTitle[section];
        var view = ca.CAView.createWithColor(ca.CAColor4B.GRAY);

        var header = ca.CALabel.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout_L_R(50, 0), ca.DVerticalLayout.fill()));
        header.setText(head);
        header.setFontSize(30);
        header.setColor(ca.CAColor4B.WHITE);
        header.setTextAlignment(ca.CATextAlignment.Left);
        header.setVerticalTextAlignmet(ca.CAVerticalTextAlignment.Center);
        view.addSubview(header);

        return view;
    }
    ,tableViewSectionViewForFooterInSection: function(viewSize ,section)
    {
        var view = ca.CAView.createWithColor(ca.CAColor4B.GRAY);
        return view;
    }
    ,numberOfRowsInSection: function(section)
    {
        return 2;
    }
    ,numberOfSections: function()
    {
        return this.sectionTitle.length;
    }
    ,tableViewHeightForRowAtIndexPath: function(section, row)
    {
        return 130;
    }
    ,tableViewHeightForHeaderInSection: function(section)
    {
        return 50;
    }
    ,tableViewHeightForFooterInSection: function(section)
    {
        return 1;
    }

});
