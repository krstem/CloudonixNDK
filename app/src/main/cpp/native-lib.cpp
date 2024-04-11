#include <jni.h>
#include <string>
#include <ifaddrs.h>
#include <unistd.h>
#include <netdb.h>
#include <android/log.h>

#define TAG "CloudonixNDK"

#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,    TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,     TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,     TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,    TAG, __VA_ARGS__)
extern "C" JNIEXPORT jstring

JNICALL
Java_com_sourcico_cloudonixndk_MainActivity_ipAddressFromJNI(
        JNIEnv *env,
        jobject /* this */) {

    struct ifaddrs *ifaddr;
    int family, s;
    char host[NI_MAXHOST];

    std::string ipAddress = "";
    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        ipAddress = "EXIT_FAILURE";
        return env->NewStringUTF(ipAddress.c_str());
    }


    /* Walk through linked list, maintaining head pointer so we
       can free list later. */

    for (struct ifaddrs *ifa = ifaddr; ifa != NULL;
         ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL)
            continue;

        family = ifa->ifa_addr->sa_family;

        /* Display interface name and family (including symbolic
           form of the latter for the common families). */

        LOGI("interface name and family: %-8s %s (%d)\n", ifa->ifa_name,
             (family == AF_PACKET) ? "AF_PACKET" :
             (family == AF_INET) ? "AF_INET" :
             (family == AF_INET6) ? "AF_INET6" : "???",
             family);


        /* For an AF_INET* interface address, display the address. */

        if (family == AF_INET || family == AF_INET6) {
            s = getnameinfo(ifa->ifa_addr,
                            (family == AF_INET) ? sizeof(struct sockaddr_in) :
                            sizeof(struct sockaddr_in6),
                            host, NI_MAXHOST,
                            NULL, 0, NI_NUMERICHOST);
            if (s != 0) {
                LOGW("getnameinfo() failed: %s\n", gai_strerror(s));
            }


            const std::string iName = ifa->ifa_name;
            // check WIFI first
            if (iName == "wlan0") {
                ipAddress = host;
            } else if (iName.find("rmnet")!= std::string::npos) {
                // ((iName == "rmnet16" || iName == "rmnet2" || iName == "rmnet0"))
                ipAddress = host;
            }
            LOGD("Address: <%s>", host);
        }
    }
    LOGD("ipAddress from server: ---------: <%s>", ipAddress.c_str());

    freeifaddrs(ifaddr);
    return env->NewStringUTF(ipAddress.c_str());
}


extern "C" JNIEXPORT jstring
JNICALL
Java_com_sourcico_cloudonixndk_MainActivity_process(
        JNIEnv *env,
        jobject mainActivityInstance) {
//return env->NewStringUTF(hello.c_str());
    const jclass mainActivityCls = env->GetObjectClass(mainActivityInstance);
    const jmethodID pJmethodId = env->GetMethodID(mainActivityCls, "processFromJNI",
                                                  "()Ljava/lang/String;");

    if (pJmethodId == nullptr) {
        return env->NewStringUTF("");
    }
    const jobject result = env->CallObjectMethod(mainActivityInstance, pJmethodId);

    const std::string java_msg = env->GetStringUTFChars((jstring) result, JNI_FALSE);
    const std::string c_msg = "Result from java => ";
    const std::string msg = c_msg + java_msg;

    return env->NewStringUTF(msg.c_str());
}