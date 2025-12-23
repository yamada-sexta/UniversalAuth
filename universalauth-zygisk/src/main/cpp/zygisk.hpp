/*
 * Copyright (C) 2022-2023 John "topjohnwu" Wu
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#pragma once

#include <jni.h>

#define ZYGISK_API_VERSION 1

namespace zygisk {

struct AppSpecializeArgs {
    jint *uid;
    jint *gid;
    jint *gids;
    jint gids_count;
    jint *rlimits;
    jint rlimits_count;
    jint *mount_external;
    jstring *se_info;
    jstring *nice_name;
    jstring *instruction_set;
    jstring *app_data_dir;
};

struct ServerSpecializeArgs {
    jint *uid;
    jint *gid;
    jint *gids;
    jint gids_count;
};

enum Option {
    FORCE_DENYLIST_UNMOUNT = 0,
    DLOPEN_COMPATIBLE = 1,
};

class Api {
public:
    virtual void connectCompanion(int fd) = 0;
    virtual int getModuleDir() = 0;
    virtual void setOption(Option option) = 0;
    virtual void sendModuleAsset(int fd) = 0;
    virtual void hookJniNativeMethods(JNIEnv *env, const char *className, JNINativeMethod *methods, int numMethods) = 0;
    virtual void pltHookRegister(const char *regex, const char *symbol, void *newFunc, void **oldFunc) = 0;
};

class Module {
public:
    virtual void onLoad(Api *api, JNIEnv *env) {}
    virtual void preAppSpecialize(AppSpecializeArgs *args) {}
    virtual void postAppSpecialize(const AppSpecializeArgs *args) {}
    virtual void preServerSpecialize(ServerSpecializeArgs *args) {}
    virtual void postServerSpecialize(const ServerSpecializeArgs *args) {}
};

} // namespace zygisk

extern "C" {
    void zygisk_module_entry(zygisk::Api *api, JNIEnv *env);
}

#define REGISTER_ZYGISK_MODULE(clazz) \
void zygisk_module_entry(zygisk::Api *api, JNIEnv *env) { \
    static clazz module; \
    module.onLoad(api, env); \
}