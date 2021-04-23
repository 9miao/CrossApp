/**
 * Created by zhanglei on 16/8/4.
 */
var ImagePickerControllerTest = ca.CAViewController.extend({
    ctor: function () {
        this._super();
    },
    viewDidLoad: function() {

         var _this_ = this;
         var takePicture = ca.CAButton.create(ca.CAButton.Type.SquareRect);
         takePicture.setLayout(ca.DLayout.set(ca.DHorizontalLayout_W_C(240, 0.5), ca.DVerticalLayout_H_C(54, 0.07)));
         //设置Button标题的显示文本和字体大小
         takePicture.setTitleForState(ca.CAControl.State.Normal, "拍照");
         //添加回调事件
         takePicture.addTarget(function(){
            var imgPicker = ca.CAImagePickerController.create(ca.CAImagePickerController.SourceType.CameraDeviceRear);
            imgPicker.open(function(img){
                           var img2 = ca.CAImageView.createWithImage(img);
                           img2.setLayout(ca.DLayout.set(ca.DHorizontalLayout_W_C(448, 0.5), ca.DVerticalLayout_H_C(700, 0.6)));
                            _this_.getView().addSubview(img2);
                           });
            }, ca.CAButton.Event.TouchUpInSide );
         this.getView().addSubview(takePicture);

        var photoAlbum = ca.CAButton.create(ca.CAButton.Type.SquareRect);
        photoAlbum.setLayout(ca.DLayout.set(ca.DHorizontalLayout_W_C(240, 0.5), ca.DVerticalLayout_H_C(54, 0.15)));
        //设置Button标题的显示文本和字体大小
        photoAlbum.setTitleForState(ca.CAControl.State.Normal, "相册");
        //添加回调事件
        photoAlbum.addTarget(function(){

            var imgPicker = ca.CAImagePickerController.create(ca.CAImagePickerController.SourceType.PhotoLibrary);
            imgPicker.open(function(img){
                var img2 = ca.CAImageView.createWithImage(img);
                img2.setLayout(ca.DLayout.set(ca.DHorizontalLayout_W_C(448, 0.5), ca.DVerticalLayout_H_C(700, 0.6)));
                _this_.getView().addSubview(img2);
            });
        }, ca.CAButton.Event.TouchUpInSide );
        this.getView().addSubview(photoAlbum);
    },
});
