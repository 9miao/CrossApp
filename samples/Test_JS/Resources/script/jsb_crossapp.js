ca.ENGINE_VERSION = "CrossApp-JS v2.1.4";

ca.LANGUAGE_ENGLISH    = 0;
ca.LANGUAGE_CHINESE    = 1;
ca.LANGUAGE_FRENCH     = 2;
ca.LANGUAGE_ITALIAN    = 3;
ca.LANGUAGE_GERMAN     = 4;
ca.LANGUAGE_SPANISH    = 5;
ca.LANGUAGE_RUSSIAN    = 6;
ca.LANGUAGE_KOREAN     = 7;
ca.LANGUAGE_JAPANESE   = 8;
ca.LANGUAGE_HUNGARIAN  = 9;
ca.LANGUAGE_PORTUGUESE = 10;
ca.LANGUAGE_ARABIC     = 11;

ca.CAHttpRequest.Type = {};
ca.CAHttpRequest.Type.Get = 0;
ca.CAHttpRequest.Type.Post = 1;
ca.CAHttpRequest.Type.PostFile = 2;
ca.CAHttpRequest.Type.Put = 3;
ca.CAHttpRequest.Type.Delete = 4;
ca.CAHttpRequest.Type.Unkown = 5;


ca.CAActivityIndicatorView.Style = {};
ca.CAActivityIndicatorView.Style.WhiteLarge = 0;
ca.CAActivityIndicatorView.Style.GrayLarge = 1;
ca.CAActivityIndicatorView.Style.White = 2;
ca.CAActivityIndicatorView.Style.Gray = 3;
ca.CAActivityIndicatorView.Style.Image = 4;

ca.CAScrollView.MultitouchGesture = {};
ca.CAScrollView.MultitouchGesture.Zoom = 0;
ca.CAScrollView.MultitouchGesture.Rotate = 1;
ca.CAScrollView.MultitouchGesture.ZoomAndRotate = 2;
ca.CAScrollView.MultitouchGesture.None = 3;

ca.CAListView.Orientation = {};
ca.CAListView.Orientation.Vertical = 0;
ca.CAListView.Orientation.Horizontal = 1;


//CAAutoCollectionView
ca.CAAutoCollectionView.Orientation = {};
ca.CAAutoCollectionView.Orientation.Vertical = 0;
ca.CAAutoCollectionView.Orientation.Horizontal = 1;

ca.CAAutoCollectionView.CellHoriAlign = {};
ca.CAAutoCollectionView.CellHoriAlign.HoriAlignLeft = 0;
ca.CAAutoCollectionView.CellHoriAlign.HoriAlignCenter = 1;
ca.CAAutoCollectionView.CellHoriAlign.HoriAlignRight = 2;

ca.CAAutoCollectionView.CellVertAlign = {};
ca.CAAutoCollectionView.CellVertAlign.VertAlignTop = 0;
ca.CAAutoCollectionView.CellVertAlign.VertAlignCenter = 1;
ca.CAAutoCollectionView.CellVertAlign.VertAlignBottom = 2;

ca.CABarVerticalAlignment = {};
ca.CABarVerticalAlignment.Top = 0,
ca.CABarVerticalAlignment.Bottom = 1;

ca.CAButton.Type = {};
ca.CAButton.Type.Custom = 0;
ca.CAButton.Type.SquareRect = 1;
ca.CAButton.Type.RoundedRect = 2;

ca.CAButton.Event = {};
ca.CAButton.Event.TouchDown = 0;
ca.CAButton.Event.TouchDownRepeat = 1;
ca.CAButton.Event.TouchMoved = 2;
ca.CAButton.Event.TouchMovedOutSide = 3;
ca.CAButton.Event.TouchUpInSide = 4;
ca.CAButton.Event.TouchUpOutSide = 5;
ca.CAButton.Event.TouchLongPress = 6;
ca.CAButton.Event.TouchCancelled = 7;

ca.CAControl.State = {};
ca.CAControl.State.Normal = 0;
ca.CAControl.State.Highlighted = 1;
ca.CAControl.State.Selected = 2;
ca.CAControl.State.Disabled = 3;

ca.CADatePickerView.Mode = {};
ca.CADatePickerView.Mode.Time = 0;
ca.CADatePickerView.Mode.Date = 1;
ca.CADatePickerView.Mode.DateAndTime = 2;
ca.CADatePickerView.Mode.CountDownTimer = 3;

ca.CADeviceIdiom = {};
ca.CADeviceIdiom.Unknown = -1;
ca.CADeviceIdiom.iPad = 0;
ca.CADeviceIdiom.iPhone = 1;

ca.CABlueToothState = {};
ca.CABlueToothState.CABLUETOOTHOEPNSUCCESS = 0;
ca.CABlueToothState.CABLUETOOTHISOEPN = 1;
ca.CABlueToothState.CABLUETOOTHOPENING = 2;
ca.CABlueToothState.CABLUETOOTHCLOSESUCCESS = 3;
ca.CABlueToothState.CABLUETOOTHCLOSED = 4;
ca.CABlueToothState.CABLUETOOTHCLOSEFAILD = 5;

ca.CABlueToothType = {};
ca.CABlueToothType.CABLUETOOTHOPEN = 0;
ca.CABlueToothType.CABLUETOOTHCLOSE = 1;
ca.CABlueToothType.CABLUETOOTHDISCOVERY = 3 ;
ca.CABlueToothType.CABLUETOOTHCANCELDISCOVERY = 4;

ca.CADevice.NetWorkData = {};
ca.CADevice.NetWorkData.Wifi= 0;
ca.CADevice.NetWorkData.ReachableViaWWAN = 1;
ca.CADevice.NetWorkData.None =2;

ca.CADevice.VolumeData = {};
ca.CADevice.VolumeData.Music= 0;
ca.CADevice.VolumeData.System = 1;
ca.CADevice.VolumeData.Ring =2;
ca.CADevice.VolumeData.VoicCall= 0;
ca.CADevice.VolumeData.Alarm = 1;
ca.CADevice.VolumeData.Notification =2;

ca.ETextAlign = {};
ca.ETextAlign.kAlignCenter        = 0x33, ///< Horizontal center and vertical center.
ca.ETextAlign.kAlignTop           = 0x13, ///< Horizontal center and vertical top.
ca.ETextAlign.kAlignTopRight      = 0x12, ///< Horizontal right and vertical top.
ca.ETextAlign.kAlignRight         = 0x32, ///< Horizontal right and vertical center.
ca.ETextAlign.kAlignBottomRight   = 0x22, ///< Horizontal right and vertical bottom.
ca.ETextAlign.kAlignBottom        = 0x23, ///< Horizontal center and vertical bottom.
ca.ETextAlign.kAlignBottomLeft    = 0x21, ///< Horizontal left and vertical bottom.
ca.ETextAlign.kAlignLeft          = 0x31, ///< Horizontal left and vertical center.
ca.ETextAlign.kAlignTopLeft       = 0x11, ///< Horizontal left and vertical top.

ca.CAImage.PixelFormat = {};
ca.CAImage.PixelFormat.RGBA8888 = 0;
ca.CAImage.PixelFormat.RGB888 =1;
ca.CAImage.PixelFormat.RGB565 =2;
ca.CAImage.PixelFormat.A8 =3;
ca.CAImage.PixelFormat.I8 =4;
ca.CAImage.PixelFormat.AI88 =5;
ca.CAImage.PixelFormat.RGBA4444 =6;
ca.CAImage.PixelFormat.RGB5A1 =7;
ca.CAImage.PixelFormat.DEFAULT = 0,

ca.CAImage.Format = {};
ca.CAImage.Format.JPG = 0;//! JPEG
ca.CAImage.Format.PNG = 1;//! PNG
ca.CAImage.Format.GIF = 2;//! GIF
ca.CAImage.Format.TIFF = 3;//! TIFF
ca.CAImage.Format.WEBP = 4;//! WebP
ca.CAImage.Format.ETC = 5;//! ETC
ca.CAImage.Format.TGA = 6;//! TGA
ca.CAImage.Format.RAW_DATA = 7;//! Raw Data
ca.CAImage.Format.UNKOWN = 8;//! Unknown format

ca.AsyncType = {};
ca.AsyncType.AsyncImageType = 0;
ca.AsyncType.AsyncStringType = 1;

ca.CAImageView.ScaleType = {};
ca.CAImageView.ScaleType.FitImageXY = 0;         //塞满
ca.CAImageView.ScaleType.FitImageCrop = 1;           //塞满裁剪，不变形, 居中
ca.CAImageView.ScaleType.FitImageInside = 2;         //显示全部，不变形, 居中
ca.CAImageView.ScaleType.FitViewByHorizontal = 3;    //显示全部，不变形，横向适应图片
ca.CAImageView.ScaleType.FitViewByVertical = 4;       //显示全部，不变形，纵横适应图片


ca.CAKeypadDispatcher.KeypadMSGType = {};
ca.CAKeypadDispatcher.KeypadMSGType.BackClicked = 1;
ca.CAKeypadDispatcher.KeypadMSGType.MenuClicked = 2;

ca.CAPageView.Orientation = {};
ca.CAPageView.Orientation.Horizontal = 0;
ca.CAPageView.Orientation.Vertical = 1;

ca.CAPageControl.Style = {};
ca.CAPageControl.Style.Dot = 0;
ca.CAPageControl.Style.Round = 1;
ca.CAPageControl.Style.Rectangle = 2;

ca.CAProgressStyle = {};
ca.CAProgressStyle.Default = 0;
ca.CAProgressStyle.Bar = 1;

ca.CAPullToRefreshView.Type = {};
ca.CAPullToRefreshView.Type.Header = 0;
ca.CAPullToRefreshView.Type.Footer = 1;
ca.CAPullToRefreshView.Type.Custom = 2;



ca.CAIndicatorType = {};
ca.CAIndicatorType.Orientation = {};
ca.CAIndicatorType.Orientation.Horizontal = 0;
ca.CAIndicatorType.Orientation.Vertical = 1;

ca.CAStepper.Orientation = {};
ca.CAStepper.Orientation.Horizontal = 0;
ca.CAStepper.Orientation.Vertical = 1;

ca.CATextField.KeyboardType = {};
ca.CATextField.KeyboardType.Default = 0;
ca.CATextField.KeyboardType.NumbersAndPunctuation =1;
ca.CATextField.KeyboardType.URL = 2;
ca.CATextField.KeyboardType.NumberPad = 3;
ca.CATextField.KeyboardType.PhonePad = 4;
ca.CATextField.KeyboardType.NamePhonePad = 5;
ca.CATextField.KeyboardType.EmailAddress = 6;

ca.CATextField.ReturnType = {};
ca.CATextField.ReturnType.Done=0,
ca.CATextField.ReturnType.Go = 1;
ca.CATextField.ReturnType.Next = 2;
ca.CATextField.ReturnType.Search = 3;
ca.CATextField.ReturnType.Send = 4;

ca.CATextField.ClearButtonMode = {};
ca.CATextField.ClearButtonMode.None = 0,
ca.CATextField.ClearButtonMode.WhileEditing = 1;

ca.CATextField.Align = {};
ca.CATextField.Align.Left = 0;
ca.CATextField.Align.Center = 1;
ca.CATextField.Align.Right = 2;

ca.CATextView.Align = {};
ca.CATextView.Align.Left = 0;
ca.CATextView.Align.Center = 1;
ca.CATextView.Align.Right = 2;

ca.CATextView.ReturnType = {};
ca.CATextView.ReturnType.Default = 0,
ca.CATextView.ReturnType.Done =1 ;
ca.CATextView.ReturnType.Send =2 ;
ca.CATextView.ReturnType.Next = 3 ;

ca.VPError = {};
ca.VPError.kErrorNone = 0;
ca.VPError.kErrorOpenFile = 1;
ca.VPError.kErrorStreamInfoNotFound = 2;
ca.VPError.kErrorStreamNotFound = 3;
ca.VPError.kErrorCodecNotFound = 4;
ca.VPError.kErrorOpenCodec = 5;
ca.VPError.kErrorAllocateFrame = 6;
ca.VPError.kErrorSetupScaler = 7;
ca.VPError.kErrorReSampler = 8;
ca.VPError.kErrorUnsupported = 9;
ca.VPError.kErrorUnknown = 10;

ca.VPFrameType = {};
ca.VPFrameType.kFrameTypeAudio = 0;
ca.VPFrameType.kFrameTypeVideo = 1;

ca.VPVideoFrameFormat = {};
ca.VPVideoFrameFormat.kVideoFrameFormatRGB = 0;
ca.VPVideoFrameFormat.kVideoFrameFormatYUV = 1;

ca.CALayoutLinearType = {};
ca.CALayoutLinearType.CALayoutLinearHorizontal = 0;
ca.CALayoutLinearType.CALayoutLinearVertical = 1;

ca.CAViewAnimation.Curve = {};
ca.CAViewAnimation.Curve.Linear = 0;
ca.CAViewAnimation.Curve.EaseOut = 1;           // slow at end
ca.CAViewAnimation.Curve.EaseIn = 2;            // slow at beginning
ca.CAViewAnimation.Curve.EaseInOut = 3;         // slow at beginning and end
ca.CAViewAnimation.Curve.EaseSineOut = 1;
ca.CAViewAnimation.Curve.EaseSineIn = 2;
ca.CAViewAnimation.Curve.EaseSineInOut = 3;
ca.CAViewAnimation.Curve.EaseBackOut = 4;
ca.CAViewAnimation.Curve.EaseBackIn = 5;
ca.CAViewAnimation.Curve.EaseBackInOut = 6;
ca.CAViewAnimation.Curve.EaseQuadOut = 7;
ca.CAViewAnimation.Curve.EaseQuadIn = 8;
ca.CAViewAnimation.Curve.EaseQuadInOut = 9;
ca.CAViewAnimation.Curve.EaseCubicOut = 10;
ca.CAViewAnimation.Curve.EaseCubicIn = 11;
ca.CAViewAnimation.Curve.EaseCubicInOut = 12;
ca.CAViewAnimation.Curve.EaseQuartOut = 13;
ca.CAViewAnimation.Curve.EaseQuartIn = 14;
ca.CAViewAnimation.Curve.EaseQuartInOut = 15;
ca.CAViewAnimation.Curve.EaseExpoOut = 16;
ca.CAViewAnimation.Curve.EaseExpoIn = 17;
ca.CAViewAnimation.Curve.EaseExpoInOut = 18;

ca.CAVerticalTextAlignment = {};
ca.CAVerticalTextAlignment.Top = 0;
ca.CAVerticalTextAlignment.Center = 1;
ca.CAVerticalTextAlignment.Bottom = 2;

ca.CATextAlignment = {};
ca.CATextAlignment.Left = 0;
ca.CATextAlignment.Center = 1;
ca.CATextAlignment.Right = 2;

ca.CAStatusBarStyle = {};
ca.CAStatusBarStyle.Default = 0; // Dark content, for use on light backgrounds
ca.CAStatusBarStyle.LightContent = 1; // Light content, for use on dark backgrounds

ca.CAInterfaceOrientation = {};
ca.CAInterfaceOrientation.Unknown  = 0;
ca.CAInterfaceOrientation.Portrait = 1;
ca.CAInterfaceOrientation.Landscape = 2;

ca.ConversionResult = {};
ca.ConversionResult.conversionOK = 0;           /* conversion successful */
ca.ConversionResult.sourceExhausted = 1;        /* partial character in source, but hit end */
ca.ConversionResult.targetExhausted = 2;        /* insuff. room in target for conversion */
ca.ConversionResult.sourceIllegal = 3;           /* source sequence is illegal/malformed */

ca.ConversionFlags = {};
ca.ConversionFlags.strictConversion = 0;
ca.ConversionFlags.lenientConversion = 1;

ca.DicItemType = {};
ca.DicItemType.EDIC_TYPENULL = 0;
ca.DicItemType.EDIC_TYPEINT = 1;
ca.DicItemType.EDIC_TYPEUINT = 2;
ca.DicItemType.EDIC_TYPEFLOAT = 3;
ca.DicItemType.EDIC_TYPESTRING = 4;
ca.DicItemType.EDIC_TYPEBOOLEN = 5;
ca.DicItemType.EDIC_TYPEARRAY = 6;
ca.DicItemType.EDIC_TYPEOBJECT = 7;

ca.GifRecordType = {};
ca.GifRecordType.UNDEFINED_RECORD_TYPE = 0;
ca.GifRecordType.SCREEN_DESC_RECORD_TYPE = 1;
ca.GifRecordType.IMAGE_DESC_RECORD_TYPE = 2; /* Begin with ',' */
ca.GifRecordType.EXTENSION_RECORD_TYPE = 3;  /* Begin with '!' */
ca.GifRecordType.TERMINATE_RECORD_TYPE = 4;   /* Begin with ';' */

ca.CASwitch.Type = {};
ca.CASwitch.Type.SquareRect = 0;
ca.CASwitch.Type.RoundedRect = 1;

ca.CACheckbox.Type = {};
ca.CACheckbox.Type.Custom = 0;
ca.CACheckbox.Type.SquareRect = 1;
ca.CACheckbox.Type.RoundedRect = 2;

ca.CAAVPlayer.PlaybackBufferEmpty = "PlaybackBufferEmpty";
ca.CAAVPlayer.PlaybackLikelyToKeepUp = "PlaybackLikelyToKeepUp";
ca.CAAVPlayer.PlayStatePause = "PlayStatePause";
ca.CAAVPlayer.PlayStatePlaying = "PlayStatePlaying";
ca.CAAVPlayer.PlayStatePlayback = "PlayStatePlayback";

ca.CAImagePickerController.SourceType = {};
ca.CAImagePickerController.SourceType.PhotoLibrary = 0;
ca.CAImagePickerController.SourceType.CameraDeviceRear = 1;
ca.CAImagePickerController.SourceType.CameraDeviceFront = 2;
ca.CAImagePickerController.SourceType.SavedPhotosAlbum = 2;

ca.NetworkStatus = {};
ca.NetworkStatus.NotReachable     = 0,
ca.NetworkStatus.ReachableViaWiFi = 2,
ca.NetworkStatus.ReachableViaWWAN = 1

ca.tAudioManagerMode = {};
ca.tAudioManagerMode.kAMM_FxOnly = 0;                    //!Other apps will be able to play audio
ca.tAudioManagerMode.kAMM_FxPlusMusic = 1;                //!Only this app will play audio
ca.tAudioManagerMode.kAMM_FxPlusMusicIfNoOtherAudio = 2;    //!If another app is playing audio at start up then allow it to continue and don't play music
ca.tAudioManagerMode.kAMM_MediaPlayback = 3;                //!This app takes over audio e.g music player app
ca.tAudioManagerMode.kAMM_PlayAndRecord = 4;                //!App takes over audio and has input and output

ca.tAudioManagerState = {};
ca.tAudioManagerState.kAMStateUninitialised = 0; //!Audio manager has not been initialised - do not use
ca.tAudioManagerState.kAMStateInitialising = 1;  //!Audio manager is in the process of initialising - do not use
ca.tAudioManagerState.kAMStateInitialised = 2;       //!Audio manager is initialised - safe to use

ca.tAudioManagerResignBehavior = {};
ca.tAudioManagerResignBehavior.kAMRBDoNothing = 0;                //Audio manager will not do anything on resign or becoming active
ca.tAudioManagerResignBehavior.kAMRBStopPlay = 1;                //Background music is stopped on resign and resumed on become active
ca.tAudioManagerResignBehavior.kAMRBStop = 2;                    //Background music is stopped on resign but not resumed - maybe because you want to do this from within your game

ca.tAudioSourceChannel = {};
ca.tAudioSourceChannel.kASC_Left = 0;
ca.tAudioSourceChannel.kASC_Right = 1;

ca.tLongAudioSourceState = {};
ca.tLongAudioSourceState.kLAS_Init = 0;
ca.tLongAudioSourceState.kLAS_Loaded = 1;
ca.tLongAudioSourceState.kLAS_Playing = 2;
ca.tLongAudioSourceState.kLAS_Paused = 3;
ca.tLongAudioSourceState.kLAS_Stopped = 4;


ca.FLT_MAX = 0xFFFFFFFF;

ca.DHorizontalLayout = {

    L_R:0,
    L_W:1,
    R_W:2,
    W_C:3,
    NW_C:4,

    set: function(var1, var2, type)
    {
        var dhorizontal_layout = {};
        dhorizontal_layout.left = ca.FLT_MAX;
        dhorizontal_layout.right = ca.FLT_MAX;
        dhorizontal_layout.width = ca.FLT_MAX;
        dhorizontal_layout.center = ca.FLT_MAX;
        dhorizontal_layout.normalizedWidth = ca.FLT_MAX;
        dhorizontal_layout.type = type;

        switch (type)
        {
            case ca.DHorizontalLayout.L_R:
            {
                dhorizontal_layout.left = var1;
                dhorizontal_layout.right = var2;
            }
                break;
            case ca.DHorizontalLayout.L_W:
            {
                dhorizontal_layout.left = var1;
                dhorizontal_layout.width = var2;
            }
                break;
            case ca.DHorizontalLayout.R_W:
            {
                dhorizontal_layout.right = var1;
                dhorizontal_layout.width = var2;
            }
                break;
            case ca.DHorizontalLayout.W_C:
            {
                dhorizontal_layout.width = var1;
                dhorizontal_layout.center = var2;
            }
                break;
            case ca.DHorizontalLayout.NW_C:
            {
                dhorizontal_layout.normalizedWidth = var1;
                dhorizontal_layout.center = var2;
            }
                break;
            default:
                break;
        }

        return dhorizontal_layout;
    },

    fill: function () {

        return this.set(0, 0, ca.DHorizontalLayout.L_R);
    },

    zero: function () {

        return this.set(ca.FLT_MAX, ca.FLT_MAX, ca.FLT_MAX);
    },
};

ca.DHorizontalLayout_L_R = function(left, right)
{
    return ca.DHorizontalLayout.set(left, right, ca.DHorizontalLayout.L_R);
};

ca.DHorizontalLayout_L_W = function(left, width)
{
    return ca.DHorizontalLayout.set(left, width, ca.DHorizontalLayout.L_W);
};

ca.DHorizontalLayout_R_W = function(right, width)
{
    return ca.DHorizontalLayout.set(right, width, ca.DHorizontalLayout.R_W);
};

ca.DHorizontalLayout_W_C = function(width, center)
{
    return ca.DHorizontalLayout.set(width, center, ca.DHorizontalLayout.W_C);
};
ca.DHorizontalLayout_NW_C = function(width, center)
{
    return ca.DHorizontalLayout.set(width, center, ca.DHorizontalLayout.NW_C);
};

ca.DVerticalLayout = {

    T_B:0,
    T_H:1,
    B_H:2,
    H_C:3,
    NH_C:4,

    set: function(var1, var2, type)
    {
        var dvertical_layout = {};
        dvertical_layout.top = ca.FLT_MAX;
        dvertical_layout.bottom = ca.FLT_MAX;
        dvertical_layout.height = ca.FLT_MAX;
        dvertical_layout.center = ca.FLT_MAX;
        dvertical_layout.normalizedHeight = ca.FLT_MAX;
        dvertical_layout.type = type;

        switch (type)
        {
            case ca.DVerticalLayout.T_B:
            {
                dvertical_layout.top = var1;
                dvertical_layout.bottom = var2;
            }
                break;
            case ca.DVerticalLayout.T_H:
            {
                dvertical_layout.top = var1;
                dvertical_layout.height = var2;
            }
                break;
            case ca.DVerticalLayout.B_H:
            {
                dvertical_layout.bottom = var1;
                dvertical_layout.height = var2;
            }
                break;
            case ca.DVerticalLayout.H_C:
            {
                dvertical_layout.height = var1;
                dvertical_layout.center = var2;
            }
                break;
            case ca.DVerticalLayout.NH_C:
            {
                dvertical_layout.normalizedHeight = var1;
                dvertical_layout.center = var2;
            }
                break;
            default:
                break;
        }

        return dvertical_layout;
    },

    fill: function () {

        return this.set(0, 0, ca.DVerticalLayout.T_B);
    },

    zero: function () {

        return this.set(ca.FLT_MAX, ca.FLT_MAX, ca.FLT_MAX);
    },
};

ca.DVerticalLayout_T_B = function(top, bottom)
{
    return ca.DVerticalLayout.set(top, bottom, ca.DVerticalLayout.T_B);
};

ca.DVerticalLayout_T_H = function(top, height)
{
    return ca.DVerticalLayout.set(top, height, ca.DVerticalLayout.T_H);
};

ca.DVerticalLayout_B_H = function(bottom, height)
{
    return ca.DVerticalLayout.set(bottom, height, ca.DVerticalLayout.B_H);
};

ca.DVerticalLayout_H_C = function(height, center)
{
    return ca.DVerticalLayout.set(height, center, ca.DVerticalLayout.H_C);
};

ca.DVerticalLayout_NH_C = function(height, center)
{
    return ca.DVerticalLayout.set(height, center, ca.DVerticalLayout.NH_C);
};

ca.DLayout = {

    set: function (hor, ver)
    {
        var dlayout = {};
        dlayout.horizontal = hor;
        dlayout.vertical = ver;
        return dlayout;
    },

    fill: function () {

        return this.set(ca.DHorizontalLayout.fill(), ca.DVerticalLayout.fill());
    },

    zero: function () {

        return this.set(ca.DHorizontalLayout.zero(), ca.DVerticalLayout.zero());
    },
};

ca.CAFont = {

    create: function()
    {
        var shadow = {};
        shadow.shadowEnabled = false;
        shadow.shadowOffset = ca.DSize.zero();
        shadow.shadowBlur = 0;
        shadow.shadowColor = ca.CAColor4B.set(0, 0, 0, 88);

        var stroke = {};
        stroke.strokeEnabled = false;
        stroke.strokeColor = ca.CAColor4B.BLACK;
        stroke.strokeSize = 0;

        var cafont = {};
        cafont.bold = false;
        cafont.underLine = false;
        cafont.deleteLine = false;
        cafont.italics = false;
        cafont.italicsValue = 0.5;
        cafont.wordWrap = false;
        cafont.fontName = "";
        cafont.fontSize = 24;
        cafont.lineSpacing = 0;
        cafont.color = ca.CAColor4B.BLACK;
        cafont.shadow = shadow;
        cafont.stroke = stroke;

        return cafont;
    },
};

ca.CAColor4B = {

    WHITE: {r:255, g:255, b:255, a:255},
    YELLOW: {r:255, g:255, b:0, a:255},
    GREEN: {r:0, g:255, b:0, a:255},
    BLUE: {r:0, g:0, b:255, a:255},
    RED: {r:255, g:0, b:0,a: 255},
    MAGENTA: {r:255, g:0, b:255, a:255},
    BLACK: {r:0, g:0, b:0, a:255},
    ORANGE: {r:255, g:127, b:0, a:255},
    GRAY: {r:166, g:166, b:166, a:255},
    CLEAR: {r:255, g:255, b:255, a:0},

    set: function (r, g, b, a)
    {
        var c4b = {};
        c4b.r = r;
        c4b.g = g;
        c4b.b = b;
        c4b.a = a;
        return c4b;
    },

    copy: function (color4b) {

        var c4b = {};
        c4b.r = color4b.r;
        c4b.g = color4b.g;
        c4b.b = color4b.b;
        c4b.a = color4b.a;
        return c4b;
    },

    setUInt32: function (rgba)
    {
        var b = rgba % 0x100;
        rgba /= 0x100;
        var g = rgba % 0x100;
        rgba /= 0x100;
        var r = rgba % 0x100;
        rgba /= 0x100;
        var a = rgba % 0x100;

        return this.set(r, g, b, a);
    },

    getUInt32: function (c4b)
    {
        return (c4b.b + c4b.g * 0x100 + c4b.r * 0x10000 + c4b.a * 0x1000000);
    },

    equals: function (c1, c2)
    {
        return (c1.r == c2.r && c1.g == c2.g && c1.b == c2.b && c1.a == c2.a);
    },
};

ca.CAColor4F = {

    WHITE: {r:1, g:1, b:1,a:1},
    YELLOW: {r:1, g:1, b:0,a:1},
    GREEN: {r:0, g:1, b:0,a:1},
    BLUE: {r:0, g:0, b:1,a:1},
    RED: {r:1, g:0, b:0,a:1},
    MAGENTA: {r:1, g:0, b:1,a:1},
    BLACK: {r:0, g:0, b:0,a:1},
    ORANGE: {r:1, g:0.5, b:0,a:1},
    GRAY: {r:0.65, g:0.65, b:0.65,a:1},
    CLEAR: {r:1, g:1, b:1,a:0},

    set: function (r, g, b, a)
    {
        var c4f = {};
        c4f.r = r;
        c4f.g = g;
        c4f.b = b;
        c4f.a = a;
        return c4f;
    },

    copy: function (c4f) {

        var c4f = {};
        c4f.r = color4f.r;
        c4f.g = color4f.g;
        c4f.b = color4f.b;
        c4f.a = color4f.a;
        return c4f;
    },

    equals: function (c1, c2)
    {
        return (c1.r == c2.r && c1.g == c2.g && c1.b == c2.b && c1.a == c2.a);
    },
};

ca.DPoint = {

    set: function(x, y)
    {
        var dpoint = {};
        dpoint.x = x;
        dpoint.y = y;
        return dpoint;
    },

    zero: function () {

        return this.set(0, 0);
    },

    copy: function (p) {

        var dpoint = {};
        dpoint.x = p.x;
        dpoint.y = p.y;
        return dpoint;
    },

    equals: function (p1, p2) {

        return ((p1.x == p2.x) && (p1.y == p2.y));
    },

    getDistance: function (p1, p2)
    {
        var xx = p1.x - p2.x;
        var yy = p1.y - p2.y;
        return Math.sqrt(xx*xx + yy*yy)
    },
};

ca.DSize = {

    set: function(width, height)
    {
        var dsize = {};
        dsize.width = width;
        dsize.height = height;
        return dsize;
    },

    zero: function () {

        return this.set(0, 0);
    },

    copy: function (s) {

        var dsize = {};
        dsize.width = s.width;
        dsize.height = s.height;
        return dsize;
    },

    equals: function (s1, s2) {

        return ((s1.width == s2.width) && (s1.height == s2.height));
    },

};

ca.DRect = {

    set: function(x, y, width, height)
    {
        var drect = {};
        drect.x = x;
        drect.y = y;
        drect.width = width;
        drect.height = height;
        return drect;
    },

    set2: function (point, size) {

        var drect = {};
        drect.x = point.x;
        drect.y = point.y;
        drect.width = size.width;
        drect.height = size.height;
        return drect;
    },

    zero: function () {

        return this.set(0, 0);
    },

    copy: function (r) {

        var drect = {};
        drect.x = r.x;
        drect.y = r.y;
        drect.width = r.width;
        drect.height = r.height;
        return drect;
    },

    equals: function (r1, r2)
    {
        return ( r1.x == r2.x && r1.y == r2.y && r1.width == r2.width && r1.height == r2.height);
    },

    intersectsRect: function (r1, r2)
    {
        if (r1.x + r1.width < r2.x)
        {
            return false;
        }

        if (r2.x + r2.width < r1.x)
        {
            return false;
        }

        if (r1.y + r1.height < r2.y)
        {
            return false;
        }

        if (r2.y + r2.height < r1.y)
        {
            return false;
        }

        return true;
    },

    containsPoint: function (r, p)
    {
        if (r.x + r.width < p.x)
        {
            return false;
        }

        if (r.x > p.x)
        {
            return false;
        }

        if (r.y + r.height < p.y)
        {
            return false;
        }

        if (r.y > p.y)
        {
            return false;
        }

        return true;
    }
};


ca.CAApplicationDidChangeStatusBarOrientationNotification = "CAApplicationDidChangeStatusBarOrientationNotification";
ca.CROSSAPP_CCLOG_NOTIFICATION = "CROSSAPP_CCLOG_NOTIFICATION";

