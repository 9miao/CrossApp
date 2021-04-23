/**
 * Created by crossApp on 16/8/8.
 */


var MenuViewController = ca.CAViewController.extend({

    ctor: function () {
        this._super();
    },

    viewDidLoad: function() {

        this.getView().setColor(ca.CAColor4B.CLEAR);

        this.menuList = [];
        this.menuList.push("CrossApp");
        this.menuList.push("9秒");

        var tableViewLayout = ca.DLayout.set(ca.DHorizontalLayout.fill(), ca.DVerticalLayout_T_B(450, 0));
        this.tableView = ca.CATableView.createWithLayout(tableViewLayout);
        this.tableView.setAllowsSelection(true);
        this.tableView.setBackgroundImage(null);
        this.tableView.setSeparatorColor(ca.CAColor4B.set(166, 166, 166, 100));
        this.tableView.setShowsScrollIndicators(false);
        this.tableView.setScrollEnabled(false);
        this.getView().addSubview(this.tableView);

        this.tableView.onCellAtIndexPath(this.tableCellAtIndex.bind(this)) ;
        this.tableView.onCellHeightAtIndexPath(this.tableViewHeightForRowAtIndexPath.bind(this));
        this.tableView.onNumberOfRowsAtIndexPath(this.numberOfRowsInSection.bind(this));
        this.tableView.onDidSelectCellAtIndexPath(this.tableViewDidSelectRowAtIndexPath.bind(this)) ;

        var logoLayout = ca.DLayout.set(ca.DHorizontalLayout_W_C(261, 0.5), ca.DVerticalLayout_T_H(120, 258));
        var logo = ca.CAImageView.createWithImage(ca.CAImage.create("image/logo.png"));
        logo.setLayout(logoLayout);
        this.getView().addSubview(logo);
    },
    tableViewDidSelectRowAtIndexPath: function(section ,row)
    {
        if(row == 0)
        {
            var webViewController = new MyWebViewController("http://crossapp.9miao.com");
            webViewController.setTitle("CrossApp");
            webViewController.init();
            ca.rootWindow.getRootNavigationController().pushViewController(webViewController, true);
        }
        else if(row == 1)
        {
            var webViewController = new MyWebViewController("http://www.9miao.com/");
            webViewController.setTitle("9秒");
            webViewController.init();
            ca.rootWindow.getRootNavigationController().pushViewController(webViewController, true);
        }
        ca.rootWindow.getDrawerController().hideLeftViewController(false);

    },
    tableCellAtIndex: function( cellSize, section ,row)
    {
        var cell = this.tableView.dequeueReusableCellWithIdentifier("CrossApp");
        if (cell == null)
        {
            cell = ca.CATableViewCell.create("CrossApp");
            cell.setBackgroundImage(null, false);
            var test_layout = ca.DLayout.set(ca.DHorizontalLayout_L_R(50, 0),ca.DVerticalLayout.fill());
            var test = ca.CALabel.createWithLayout(test_layout);
            test.setTextAlignment(ca.CATextAlignment.Left);
            test.setVerticalTextAlignmet(ca.CAVerticalTextAlignment.Center);
            test.setFontSize(32);
            test.setColor(ca.CAColor4B.WHITE);
            test.setTag(100);
            cell.addSubview(test);

            var arrow_layout = ca.DLayout.set(ca.DHorizontalLayout_R_W(0, 64),ca.DVerticalLayout_T_H(20, 64));
            var arrow = ca.CAImageView.createWithLayout(arrow_layout);
            arrow.setTag(101);
            cell.addSubview(arrow);
        }
        var test = cell.getSubviewByTag(100);
        test.setText(this.menuList[row]);

        var arrow = cell.getSubviewByTag(101);
        arrow.setImage(ca.CAImage.create("source_material/cell_btn_right.png"));

        return cell;

    },
    numberOfRowsInSection: function(section)
    {
        return 2;
    },
    tableViewHeightForRowAtIndexPath: function(section, row)
    {
        return 100;
    }
});
