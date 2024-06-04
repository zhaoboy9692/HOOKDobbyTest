#include <jni.h>
#include <string>
#include <stdio.h>
#include "include/Log.h"
#include "include/dobby.h"
#include <sys/system_properties.h>

#define TAG "HOOKDobbyTestsposed"

void *(*old_strstr)(char *, char *);

void *new_strstr(char *a1, char *a2) {
    __android_log_print(6, TAG, "strstr hook  %s  %s", a1, a2);
    void *res = old_strstr(a1, a2);
    return res;
}

FILE *(*old_fopen)(char *, char *);

FILE *new_fopen(char *filePath, char *mode) {
    __android_log_print(6, TAG, "fopen  filePath %s  mode %s", filePath, mode);
    FILE *res = old_fopen(filePath, mode);
    return res;
}

int (*orig__system_property_get)(const char *name, char *value);

int fake__system_property_get(const char *name, char *value) {
    int t = orig__system_property_get(name, value);
    __android_log_print(6, TAG, "### fake: __system_property_get(%s, %s) == 0x%x", name, value, t);
    return t;
}


jint JNICALL JNI_OnLoad(JavaVM *vm, void *unused) {
    __android_log_print(6, TAG, "jni_onload开始加载");
    JNIEnv *env = nullptr;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        __android_log_print(6, TAG, "jni_onload没加载成功");
        return 0;
    }
    __android_log_print(6, TAG, "jni_onload加载成功");
    __android_log_print(6, TAG, "DobbyGetVersion %s", DobbyGetVersion());
    void *addr = DobbySymbolResolver("/system/lib64/libc.so", "strstr", nullptr);
    void *fopen_addr = DobbySymbolResolver("libc.so", "fopen", nullptr);
    if (addr != nullptr) {
        __android_log_print(6, TAG, "开始hook strstr");
        DobbyHook(addr, (void *) new_strstr, (void **) &old_strstr);
    }
    if (fopen_addr != nullptr) {
        __android_log_print(6, TAG, "开始hook fopen");
        DobbyHook(fopen_addr, (void *) new_fopen, (void **) &old_fopen);
        //第二种
//        DobbyHook((void *) fopen, (void *) new_fopen, (void **) &old_fopen);
    }
    DobbyHook((void *) __system_property_get, (void *) fake__system_property_get,
              (void **) &orig__system_property_get);

    __android_log_print(6, TAG, "startstartstartstartstart");
    char *temp = new char[20]{};
    FILE *fp = fopen("/proc/sys/kernel/random/boot_id", "r");
    char boot[37];
    if (fp == NULL) {
        __android_log_print(6, TAG, "boot_id111 失败");
    } else {
        __android_log_print(6, TAG, "boot_id000000 开始");
        unsigned char c;
        int i = 0;
        while (i < 37) {
            c = fgetc(fp);
            boot[i] = c;
            i = i + 1;
        }
        if (ferror(fp)) {
            __android_log_print(6, TAG, "boot_id2222 失败");
        }
    }

    __android_log_print(6, TAG, "boot_id  %s ", boot);
    __system_property_get("ro.build.id", temp);
    __android_log_print(6, TAG, "id:%s", temp);
    __system_property_get("ro.build.version.sdk", temp);
    __android_log_print(6, TAG, "version:%s", temp);
    __system_property_get("ro.board.platform", temp);
    __android_log_print(6, TAG, "platform:%s", temp);
    __system_property_get("ro.product.cpu.abi2", temp);
    __android_log_print(6, TAG, "abi2:%s", temp);

    __system_property_get("ro.product.model", temp);
    __android_log_print(6, TAG, "model:%s", temp);

    __system_property_get("ro.product.brand", temp);
    __android_log_print(6, TAG, "brand:%s", temp);

    __system_property_get("ro.product.manufacturer", temp);
    __android_log_print(6, TAG, "manufacturer:%s", temp);

    __system_property_get("ro.gpu", temp);
    __android_log_print(6, TAG, "gpu:%s", temp);

    return JNI_VERSION_1_6;
}


extern "C" JNIEXPORT jstring JNICALL
Java_com_zxc_hookdobbytest_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

    return env->NewStringUTF(DobbyGetVersion());
}