#include <jni.h>
#include <android/log.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "zygisk.hpp"

#define LOG_TAG "UniversalAuthZygisk"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

using zygisk::Api;
using zygisk::AppSpecializeArgs;
using zygisk::ServerSpecializeArgs;

class UniversalAuthModule : public zygisk::Module {
public:
    void onLoad(Api *api, JNIEnv *env) override {
        this->api = api;
        this->env = env;
    }

    void preAppSpecialize(AppSpecializeArgs *args) override {
        if (!args || !args->nice_name) {
            return;
        }
        const char *name = env->GetStringUTFChars(*args->nice_name, nullptr);
        if (name) {
            if (strcmp(name, "com.android.systemui") == 0) {
                LOGD("Detected SystemUI process");
                is_system_ui = true;
            }
            env->ReleaseStringUTFChars(*args->nice_name, name);
        }
    }

    void postAppSpecialize(const AppSpecializeArgs *args) override {
        if (is_system_ui) {
            LOGD("SystemUI specialized, attempting injection...");
            inject(env);
        }
    }

    void preServerSpecialize(ServerSpecializeArgs *args) override {}
    void postServerSpecialize(const ServerSpecializeArgs *args) override {}

private:
    Api *api;
    JNIEnv *env;
    bool is_system_ui = false;

    void inject(JNIEnv *env) {
        // TODO: Implement DEX loading mechanism
        // 1. Read classes.dex from module directory
        // 2. Create PathClassLoader or InMemoryDexClassLoader
        // 3. Load the entry class (e.g. ax.nd.universalauth.zygisk.Entry)
        // 4. Invoke a static method (e.g. init)
        
        LOGD("Injection logic not implemented yet.");
    }
};

REGISTER_ZYGISK_MODULE(UniversalAuthModule)