package org.CrossApp.lib;

import android.Manifest;
import android.content.DialogInterface;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.util.Log;
import android.widget.Toast;

import androidx.appcompat.app.AlertDialog;

import com.tbruyelle.rxpermissions3.RxPermissions;

import java.util.ArrayList;
import java.util.List;

import io.reactivex.rxjava3.functions.Consumer;

import static org.CrossApp.lib.CrossAppDevice.list;

public class CAPermissionsManager {

    public static void request(List<String> types, CAPermissionsManagerCallBack capermissionsmanagercallback) {
        final CrossAppActivity context = CrossAppActivity.getContext();
        final RxPermissions rxPermissions = new RxPermissions(context);
        String[] array = new String[types.size()];

        for (int i = 0; i < types.size(); i++) {
            String type = types.get(i);
            switch (type) {
                case "读取日历": {
                    array[i] = Manifest.permission.READ_CALENDAR;
                }
                break;
                case "修改日历": {
                    array[i] = Manifest.permission.WRITE_CALENDAR;
                }
                break;
                case "相机": {
                    array[i] = Manifest.permission.CAMERA;
                }
                break;
                case "读取联系人": {
                    array[i] = Manifest.permission.READ_CONTACTS;
                }
                break;
                case "修改联系人": {
                    array[i] = Manifest.permission.WRITE_CONTACTS;
                }
                break;
                case "GPS定位": {
                    array[i] = Manifest.permission.ACCESS_FINE_LOCATION;
                }
                break;
                case "WIFI定位": {
                    array[i] = Manifest.permission.ACCESS_COARSE_LOCATION;
                }
                break;
                case "麦克风": {
                    array[i] = Manifest.permission.RECORD_AUDIO;
                }
                break;
                case "拨号": {
                    array[i] = Manifest.permission.CALL_PHONE;
                }
                break;
                case "传感器": {
                    array[i] = Manifest.permission.BODY_SENSORS;
                }
                break;
                case "发短信": {
                    array[i] = Manifest.permission.SEND_SMS;
                }
                break;
                case "读取存储卡": {
                    array[i] = Manifest.permission.READ_EXTERNAL_STORAGE;
                }
                break;
                case "写入存储卡": {
                    array[i] = Manifest.permission.WRITE_EXTERNAL_STORAGE;
                }
                break;
                default:
                    break;
            }
        }
        try {
            rxPermissions.request(array)
                    .subscribe(granted -> {
                        if (granted) {
                            capermissionsmanagercallback.onCallBack(true);
                        } else {
                            new AlertDialog.Builder(context)
                                    .setTitle("权限申请")
                                    .setMessage("在设置-应用-" + CAPermissionsManager.getAppName() + "-权限中开启" +
                                            CAPermissionsManager.getPermissionStr(types)
                                            + "权限，以正常使用" + CAPermissionsManager.getAppName() + "相关功能")
                                    .setOnDismissListener(new DialogInterface.OnDismissListener() {
                                        @Override
                                        public void onDismiss(DialogInterface dialogInterface) {
                                            CameraView.closed();
                                        }
                                    })
                                    .setNegativeButton("确定", new DialogInterface.OnClickListener() {
                                        @Override
                                        public void onClick(DialogInterface dialog, int which) {
                                            dialog.dismiss();
                                        }
                                    })
                                    .create()
                                    .show();
                        }
                    }, new Consumer<Throwable>() {
                        @Override
                        public void accept(Throwable throwable) throws Exception {//处理异常
                            throwable.printStackTrace();
                            CameraView.closed();
                        }
                    });
        } catch (Exception e) {
        }
    }

    public interface CAPermissionsManagerCallBack {
        void onCallBack(Boolean isSucess);
    }

    public static String getAppName() {
        try {
            PackageManager packageManager = CrossAppActivity.getContext().getPackageManager();
            PackageInfo packageInfo = packageManager.getPackageInfo(
                    CrossAppActivity.getContext().getPackageName(), 0);
            int labelRes = packageInfo.applicationInfo.labelRes;
            return CrossAppActivity.getContext().getResources().getString(labelRes);
        } catch (Exception e) {
            e.printStackTrace();
            return "";
        }
    }

    public static String getPermissionStr(List<String> types) {
        String str = "";
        for (int i = 0; i < types.size(); i++) {
            if (i == types.size() - 1) {
                str += types.get(i);
            } else {
                str += types.get(i) + "、";
            }
        }
        return str;
    }

    private static native void imagePickerWriteCallback(int isSucess);

    public static void requestImagePickerWrite() {

        final CrossAppActivity context = CrossAppActivity.getContext();
        context.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                ArrayList<String> types = new ArrayList<String>();
                types.add("读取存储卡");
                types.add("写入存储卡");
                CAPermissionsManager.request(types, new CAPermissionsManager.CAPermissionsManagerCallBack() {
                    @Override
                    public void onCallBack(Boolean isSucess) {
                        imagePickerWriteCallback(isSucess ? 1 : 0);
                    }
                });
            }
        });
    }
}





