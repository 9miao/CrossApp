/**
 * Created by crossApp on 16/8/3.
 */
var TextFieldTest = ca.CAViewController.extend({

    ctor: function () {
        this._super();
    },

    viewDidLoad: function() {

        var textField1 = ca.CATextField.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout_L_R(100, 100), ca.DVerticalLayout_T_H(200, 100)));
        //PlaceHolder文本内容
        textField1.setPlaceHolderText("Input");
        //键盘类型
        textField1.setKeyboardType(ca.CATextField.KeyboardType.Default);
        //TextField的对齐方式
        textField1.setAlign(ca.CATextField.Align.Left);
        this.getView().addSubview(textField1);

        var textField2 = ca.CATextField.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout_L_R(100, 100), ca.DVerticalLayout_T_H(450, 100)));
        textField2.setTag(101);
        textField2.setPlaceHolderText("Input");
        textField2.setKeyboardType(ca.CATextField.KeyboardType.Default);
        textField2.setMarginImageLeft(ca.DSize.set(60,60),"source_material/search.png");
        textField2.setClearButtonMode(ca.CATextField.ClearButtonMode.WhileEditing);
        textField2.setAlign(ca.CATextField.Align.Left);
        textField2.setSecureTextEntry(true);
        this.getView().addSubview(textField2);
    },

    textFieldShouldBeginEditing: function(){
        log("textFieldShouldBeginEditing-->");
        return true;
    },
    textFieldShouldEndEditing: function(){
        log("textFieldShouldEndEditing-->");
        return true;
    },
    textFieldShouldReturn: function(){
        log("textFieldShouldReturn-->");
    },
    keyBoardHeight: function(height){
        log("keyBoardHeight-->");
    },
    textFieldShouldChangeCharacters: function(location, lenght, changedText){
        log("textFieldShouldChangeCharacters-->");
        return true;
    }
});
