package com.zxc.hookdobbytest;

import android.app.Application;
import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.util.Log;

import java.util.List;

import de.robv.android.xposed.IXposedHookLoadPackage;
import de.robv.android.xposed.XC_MethodHook;
import de.robv.android.xposed.XposedBridge;
import de.robv.android.xposed.XposedHelpers;
import de.robv.android.xposed.callbacks.XC_LoadPackage;

public class Testose implements IXposedHookLoadPackage {
    @Override
    public void handleLoadPackage(XC_LoadPackage.LoadPackageParam lpparam) throws Throwable {

        XposedHelpers.findAndHookMethod(Application.class, "attach", Context.class, new XC_MethodHook() {
            @Override
            protected void beforeHookedMethod(MethodHookParam param) throws Throwable {

                Context context = (Context) param.args[0];
                XposedBridge.log("类加载器: " + context.getClassLoader());

//
//                //拿到被注入的so 文件的路径
//                PackageManager pm = context.getPackageManager();
//                List<PackageInfo> pkgList = pm.getInstalledPackages(0);  //拿到手机中安装的所有应用
//                if (pkgList.size() > 0) {
//                    for (int i = 0; i < pkgList.size(); i++) {
//                        PackageInfo pi = pkgList.get(i);
//                        String path = pi.applicationInfo.publicSourceDir;
//                        XposedBridge.log("path: " + path);
//                        if (path.startsWith("/data/app/com.zxc.hookdobbytest")) {
////                            Log.d("999", "path: "+path);
//                            // /data/app/com.gqghj.vb2345qw-f5v9qQ65FYeWsAq9kgvJCQ==/lib/arm64/libdemo.so
//                            // /data/app/com.gqghj.vb2345qw-f5v9qQ65FYeWsAq9kgvJCQ==/base.apk
//                            String new_path = path.replace("base.apk", "lib/arm64/libdemo.so");
//                            XposedBridge.log("path: " + new_path);
////                            injectSo(new_path);
//                        }
//
//
//                    }
//                }

                String path = "/data/local/tmp/libhookdobbytest.so";
                int version = android.os.Build.VERSION.SDK_INT;
                if (!path.equals("")) {
                    if (version >= 28) {
                        XposedBridge.log("注入1 " + path);
                        XposedHelpers.callMethod(Runtime.getRuntime(), "nativeLoad", path, null);
//                        XposedHelpers.callStaticMethod(System.class, "load", "/data/app/~~f4rmJb5vJAnlZuAe0Jndzw==/com.zxc.dobbytest-9rfhF6N859LRcDmsnR5LjQ==/lib/arm64/" + path);
                    } else {
                        XposedBridge.log("注入2");
                        XposedHelpers.callMethod(Runtime.getRuntime(), "doLoad", path, null);
                    }
                    XposedBridge.log("注入完毕");
                }
            }
        });
    }
}
