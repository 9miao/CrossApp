/**
 * Created by crossApp on 16/8/3.
 */



var PickerViewTest = ca.CAViewController.extend({

    ctor: function () {
        this._super();

        this.cityArr = [
            "北京市",
            "天津市",
            "上海市",
            "重庆市",
            "河北省",
            "山西省",
            "辽宁省",
            "吉林省",
            "黑龙江省",
            "江苏省",
            "浙江省",
            "安徽省",
            "福建省",
            "江西省",
            "山东省",
            "河南省",
            "湖北省",
            "湖南省",
            "广东省",
            "海南省",
            "四川省",
            "贵州省",
            "云南省",
            "陕西省",
            "甘肃省",
            "青海省",
            "台湾省",
            "广西壮族自治区",
            "内蒙古自治区",
            "西藏自治区",
            "宁夏回族自治区",
            "新疆维吾尔自治区",
            "香港特别行政区",
            "澳门特别行政区"
        ];
    },

    viewDidLoad: function() {

        // this.city_value = ca.CALabel.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout.fill(), ca.DVerticalLayout_T_H(100, 40)));
        // this.city_value.setText("北京市");
        // this.city_value.setColor(ca.CAColor4B.BLACK);
        // this.city_value.setFontSize(28);
        // this.city_value.setTextAlignment(ca.CATextAlignment.Center);
        // this.city_value.setVerticalTextAlignmet(ca.CAVerticalTextAlignment.Center);
        // this.getView().addSubview(this.city_value);
        //
        //
        // this.pickerView = ca.CAPickerView.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout_W_C(480, 0.5), ca.DVerticalLayout_T_H(200, 400)));
        // this.pickerView.setFontSizeNormal(40);
        // this.pickerView.setFontSizeSelected(40);
        // this.pickerView.setFontColorNormal(ca.CAColor4B.BLACK);
        // this.pickerView.setFontColorSelected(ca.CAColor4B.BLACK);
        // this.pickerView.setBackgroundImage(ca.CAImage.createWithColor4B(ca.CAColor4B.YELLOW));
        // this.getView().addSubview(this.pickerView);
        //
        // this.pickerView.onDidSelectRow(this.didSelectRow.bind(this)) ;
        // this.pickerView.onNumberOfComponents(this.numberOfComponentsInPickerView.bind(this)) ;
        // this.pickerView.onNumberOfRowsInComponent(this.numberOfRowsInComponent.bind(this)) ;
        // this.pickerView.onWidthForComponent(this.widthForComponent.bind(this)) ;
        // this.pickerView.onHeightForComponent(this.rowHeightForComponent.bind(this)) ;
        // this.pickerView.onTitleForRow(this.titleForRow.bind(this)) ;
        // this.pickerView.onViewForSelected(this.viewForSelected.bind(this));
        //
        // this.pickerView.reloadAllComponents();

        var dpv3 = ca.CADatePickerView.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout_NW_C(0.8, 0.5), ca.DVerticalLayout_T_H(200, 400)),ca.CADatePickerView.Mode.Date);
        dpv3.setBackgroundImage(ca.CAImage.createWithColor4B(ca.CAColor4B.YELLOW));
        dpv3.setBackgroundImageForSelected(ca.CAImage.createWithColor4B(ca.CAColor4B.GREEN));
        this.getView().addSubview(dpv3);

        dpv3.onSelectRow(function (data) {

        });
    },
    didSelectRow: function ( row, component) {
        this.city_value.setText(this.cityArr[row]);
    },
    numberOfComponentsInPickerView: function () {
        return 1;
    },

    numberOfRowsInComponent: function ( component) {
        return this.cityArr.length;
    },
    widthForComponent: function ( component) {
        return 480;
    },

    rowHeightForComponent: function ( component) {
        return 80;
    },
    titleForRow: function ( row, component) {
        return this.cityArr[row];
    },
    viewForSelected: function (component, size) {
        return ca.CAView.createWithColor(ca.CAColor4B.GREEN);
    }
});
