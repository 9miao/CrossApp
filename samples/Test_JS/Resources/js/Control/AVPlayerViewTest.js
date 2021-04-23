/**
 * Created by crossApp on 16/8/4.
 */
var AVPlayerViewTest = ca.CAViewController.extend({
    ctor: function () {
        this._super();
    },
    viewDidLoad: function() {

      this.previewBtn = ca.CAButton.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout.fill(), ca.DVerticalLayout_H_C(640, 0.3)), ca.CAButton.Type.Custom);
      this.previewBtn.setImageForState(ca.CAControl.State.Normal, ca.CAImage.create("image/start.png"));
      this.previewBtn.setImageForState(ca.CAControl.State.Highlighted, ca.CAImage.create("image/start.png"));
      this.previewBtn.setImageSize(ca.DSize.set(46, 46));
      this.setTextTagAndBackgroundImage(this.previewBtn, "", "image/preview.png");
      this.previewBtn.setZOrder(200);
      var _this_ = this;
      this.previewBtn.addTarget(function()
        {
            _this_.previewBtn.setVisible(false);
            _this_.playVideo();

        }, ca.CAButton.Event.TouchUpInSide);
      this.getView().addSubview(this.previewBtn);
    },

    playVideo: function () {

        var _this_ = this;
        if(this.avplayer)
        {
            this.previewBtn.setVisible(false);
            this.avplayer.play();
            return;
        }

        this.avplayer = ca.CAAVPlayer.createWithUrl("https://vd3.bdstatic.com/mda-kmrjft83jp9e60y3/v1-cae/sc/mda-kmrjft83jp9e60y3.mp4?v_from_s=nj_haokan_4469&auth_key=1615190283-0-0-f8f717c7a86cf702ddc7cbb516b75619&bcevod_channel=searchbox_feed&pd=1&pt=3&abtest=");
        //this.avplayer = ca.CAAVPlayer.createWithUrl("http://download.3g.joy.cn/video/236/60236937/1451280942752_hd.mp4");

        this.avplayer.onDidPlayToEndTime(function ()
        {
            _this_.previewBtn.setVisible(true);
            _this_.avplayer.stop();
        });

        this.avplayer.onTimeJumped(function ()
        {
            ca.log("进度调整");
        });

        var playerView = ca.CAAVPlayerView.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout.fill(), ca.DVerticalLayout_H_C(640, 0.3)));
        playerView.setPlayer(this.avplayer);
        playerView.insertSubview(ca.CAView.createWithLayout(ca.DLayout.fill(), ca.CAColor4B.BLACK), -1);
        this.getView().addSubview(playerView);


        var slider = ca.CASlider.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout_L_R(100, 100), ca.DVerticalLayout_H_C(100, 0.52)));
        slider.setValue(0);
        slider.setZOrder(100);
        this.getView().addSubview(slider);
        slider.setMaxTrackTintImage(ca.CAImage.CLEAR_IMAGE());
        slider.setTargetForTouchUpSide(function (value)
        {
            var current = value * _this_.avplayer.getDuration();
            _this_.avplayer.setCurrentTime(current); //??
        });
        this.avplayer.onPeriodicTime(function (current, duration)
        {
            var vl = parseFloat(current / duration);
            if(!isNaN(vl))
            {
                slider.setValue(vl);
            }
        });

        var progress = ca.CAProgress.createWithLayout(ca.DLayout.set(ca.DHorizontalLayout_L_R(3, 3), ca.DVerticalLayout.fill()));
        progress.setZOrder(-1);
        slider.addSubview(progress);
        progress.setProgressTintImage(progress.getProgressTrackImage());
        progress.setProgressTintColor(ca.CAColor4B.RED);
        progress.setProgressTrackColor(ca.CAColor4B.YELLOW);
        progress.setProgress(0);
        this.avplayer.onLoadedTime(function( current, duration)
        {
            progress.setProgress(current / duration);
        });

        var btn0 = ca.CAButton.create(ca.CAButton.Type.Custom);
        btn0.setLayout(ca.DLayout.set(ca.DHorizontalLayout_L_W(30, 46), ca.DVerticalLayout_H_C(46, 0.52)));
        this.setTextTagAndBackgroundImage(btn0, "暂停", "image/play.png");
        this.getView().addSubview(btn0);
        btn0.setZOrder(100);
        btn0.addTarget(function()
        {
            var textTag = btn0.getTextTag();
            if (textTag == "播放")
            {
                _this_.setTextTagAndBackgroundImage(btn0, "暂停", "image/play.png");
                _this_.avplayer.play();
            }
            else if (textTag == "暂停")
            {
                _this_.setTextTagAndBackgroundImage(btn0, "播放", "image/start.png");
                _this_.avplayer.pause();
            }

        }, ca.CAButton.Event.TouchUpInSide);



        var btn2 = ca.CAButton.create(ca.CAButton.Type.Custom);
        btn2.setLayout(ca.DLayout.set(ca.DHorizontalLayout_R_W(30, 46), ca.DVerticalLayout_H_C(46, 0.52)));
        btn2.setBackgroundImageForState(ca.CAControl.State.Normal, ca.CAImage.create("image/stop.png"), true);
        btn2.setBackgroundImageForState(ca.CAControl.State.Highlighted, ca.CAImage.create("image/stop.png"), true);
        this.getView().addSubview(btn2);
        btn2.setZOrder(100);
        btn2.addTarget(function()
        {
            _this_.setTextTagAndBackgroundImage(btn0, "暂停", "image/play.png");
            _this_.previewBtn.setVisible(true);
            _this_.avplayer.stop();
        }, ca.CAButton.Event.TouchUpInSide);

        var activity = ca.CAActivityIndicatorView.createWithLayout(ca.DLayout.fill());
        playerView.addSubview(activity);
        activity.stopAnimating();

        this.avplayer.play();

        this.avplayer.onPlayState(function(state)
        {
            if (state == ca.CAAVPlayer.PlayStatePlaying)
            {
                activity.startAnimating();
            }
            else if (state == ca.CAAVPlayer.PlayStatePause)
            {
                activity.stopAnimating();
            }
        });

        this.avplayer.onPlayBufferLoadingState(function(state)
        {
            if (state == ca.CAAVPlayer.PlaybackBufferEmpty)
            {
                activity.startAnimating();
            }
            else if (state == ca.CAAVPlayer.PlaybackLikelyToKeepUp)
            {
                activity.stopAnimating();
            }
        });
    },

    setTextTagAndBackgroundImage: function (btn, textTag, filePath) {
        btn.setTextTag(textTag);
        btn.setBackgroundImageForState(ca.CAControl.State.Normal, ca.CAImage.create(filePath), false);
        btn.setBackgroundImageForState(ca.CAControl.State.Highlighted, ca.CAImage.create(filePath), false);
    },

});
