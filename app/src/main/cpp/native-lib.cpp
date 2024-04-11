#include <jni.h>
#include <string>
#include <ifaddrs.h>
#include <unistd.h>
#include <netdb.h>
#include <android/log.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <jni.h>
#include <string>
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
    std::string ipAddressLinkLocalV4 = "";
    std::string ipAddressV6 = "";
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
            } else if (iName.find("rmnet")!= std::string::npos && ipAddress.length() == 0) {
                // ((iName == "rmnet16" || iName == "rmnet2" || iName == "rmnet0"))
                ipAddress = host;
            }

            if(IN6_IS_ADDR_LINKLOCAL(host)) {
                LOGD("Check GLOBAL address: <%s>", host);
                ipAddressV6 = host;
            }
            uint32_t ipv4;
            struct in_addr addrp;
            if (0 != inet_aton(host, &addrp)) {
                ipv4 = addrp.s_addr;
            } else {
                ipv4 = 0;
            }
            LOGD("ipv4: <%u>", ipv4);
            if (iName == "lo" && (ipv4 & 0x0000ffff) == 0x0000fea9) {
                LOGD("Address is link local: <%u> %s", ipv4, host);
                ipAddressLinkLocalV4 = host;
            }
            LOGD("Address: <%s>", host);
        }
    }


    // Get host name
    char hostPublic[256];
    if (gethostname(hostPublic, sizeof(hostPublic)) == -1) {
        throw std::runtime_error("Error getting host name");
    }

    // Get host entry
    struct hostent *host_entry;
    host_entry = gethostbyname(hostPublic);
    if (host_entry == nullptr) {
        throw std::runtime_error("Error getting host entry");
    }

    // Convert IP address to string
    struct in_addr **addr_list = (struct in_addr **) host_entry->h_addr_list;
    std::string ip_a4 = inet_ntoa(*addr_list[0]);

    LOGD("Address Ip 4 public: <%s> ipAddressLinkLocalV4: <%s>", ip_a4.c_str(), ipAddressLinkLocalV4.c_str());
    if (ipAddressLinkLocalV4.length() == 0 && ip_a4.length() > 0) {
        ipAddressLinkLocalV4 = ip_a4;
    }
    freeifaddrs(ifaddr);
    if (ipAddressV6.length() > 0) {
        return env->NewStringUTF(ipAddressV6.c_str());
    } else if (ipAddressLinkLocalV4.length() > 0) {
        return env->NewStringUTF(ipAddressLinkLocalV4.c_str());
    } else {
        return env->NewStringUTF(ipAddress.c_str());
    }
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