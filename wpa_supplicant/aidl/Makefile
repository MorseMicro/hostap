# ============================================================================
# Makefile for libwpa_aidl
# ============================================================================
CONFIG_FILE=../.config
include $(CONFIG_FILE)
CONFIG_FILE=

AOSP_ROOT := /opt/android
ANDROID_PREBUILTS := $(AOSP_ROOT)/prebuilts
ANDROID_SYSTEM := $(AOSP_ROOT)/system
ANDROID_EXTERNAL := $(AOSP_ROOT)/external
ANDROID_RPI_TARGET=$(AOSP_ROOT)/out/target/product/rpi4
SOONG_INT=$(AOSP_ROOT)/out/soong/.intermediates
NDK_INCLUDE := $(ANDROID_PREBUILTS)/vndk/v33/arm64/include/generated-headers/hardware/interfaces/wifi/supplicant/aidl/android.hardware.wifi.supplicant-V1-ndk-source/gen/include/
CUTILS_INCLUDE := $(ANDROID_SYSTEM)/core/libcutils/include/
BINDER_INCLUDE := $(AOSP_ROOT)/frameworks/native/libs/binder/ndk
LIBBASE_INCLUDE := $(ANDROID_SYSTEM)/libbase/include/
SRC_INCLUDE := ../../src
LIBCXX_INCLUDE := $(ANDROID_EXTERNAL)/libcxx/include/
LIBCXXABI_INCLUDE := $(ANDROID_EXTERNAL)/libcxxabi/include/
KEYSTORE_INCLUDE := $(ANDROID_SYSTEM)/keystore/include/
STATIC_LIB_INCLUDE := $(ANDROID_RPI_TARGET)/obj/STATIC_LIBRARIES/libwpa_aidl_intermediates
LIB_SSL_DIR := $(SOONG_INT)/external/boringssl/libssl/android_vendor_arm64_armv8-a_shared/
LIB_LOG_DIR := $(SOONG_INT)/system/logging/liblog/liblog/android_vendor_arm64_armv8-a_shared/
LIB_UTILS_DIR := $(ANDROID_PREBUILTS)/vndk/v33/arm64/arch-arm64-armv8-a/shared/vndk-sp/
LIB_BASE_DIR := $(SOONG_INT)/system/libbase/libbase/android_vendor_arm64_armv8-a_shared/
LIB_NDK_BINDER_DIR := $(SOONG_INT)/frameworks/native/libs/binder/ndk/libbinder_ndk/android_vendor_arm64_armv8-a_shared/
LIB_CPP := $(ANDROID_RPI_TARGET)/obj/SHARED_LIBRARIES/libc++_intermediates/
LIB_CPP_ABI := $(ANDROID_PREBUILTS)/clang/host/linux-x86/clang-r510928/android_libc++/ndk/aarch64/lib/

AIDL_SRCS := \
    aidl.cpp \
    aidl_manager.cpp \
    iface_config_utils.cpp \
    sta_iface.cpp \
    sta_network.cpp \
    supplicant.cpp \
    p2p_iface.cpp \
    p2p_network.cpp 

AIDL_OBJS := $(AIDL_SRCS:.cpp=.o)

CPPFLAGS += -I$(abspath $(BINDER_INCLUDE)/include_platform) \
            -I$(abspath $(BINDER_INCLUDE)/include_ndk) \
            -I$(abspath $(BINDER_INCLUDE)/include_cpp) \
            -I$(abspath $(NDK_INCLUDE)) \
            -I$(abspath $(LIBBASE_INCLUDE)) \
            -I$(abspath $(SRC_INCLUDE)) \
            -I$(abspath $(SRC_INCLUDE)/rsn_supp/) \
            -I$(abspath $(SRC_INCLUDE)/common/) \
            -I$(abspath $(SRC_INCLUDE)/utils/) \
            -I$(abspath $(SRC_INCLUDE)/eap_common/) \
            -I$(abspath $(SRC_INCLUDE)/eapol_supp/) \
            -I$(abspath $(SRC_INCLUDE)/eap_peer/) \
            -I$(abspath $(SRC_INCLUDE)/eap_server/) \
            -I$(abspath $(SRC_INCLUDE)/l2_packet/) \
            -I$(abspath $(SRC_INCLUDE)/radius/) \
            -I$(abspath $(SRC_INCLUDE)/tls/) \
            -I$(abspath $(SRC_INCLUDE)/wps/) \
            -I$(abspath $(CUTILS_INCLUDE)) \
	    -I$(abspath $(LIBCXX_INCLUDE)) \
	    -I$(abspath $(LIBCXXABI_INCLUDE)) \
	    -I$(abspath $(KEYSTORE_INCLUDE)) \
	    -I$(abspath $(STATIC_LIB_INCLUDE)) \
	    -I$(abspath $(ANDROID_SYSTEM)/core/libvendorsupport/include_llndk) \
	    -I$(abspath $(ANDROID_RPI_TARGET)/gen/STATIC_LIBRARIES/libwpa_aidl_intermediates) \
	    -I$(abspath $(ANDROID_SYSTEM)/core/libutils/include) \
	    -I$(abspath $(ANDROID_SYSTEM)/logging/liblog/include_vndk) \
	    -I$(abspath $(ANDROID_SYSTEM)/core/libprocessgroup/include) \
	    -I$(abspath $(ANDROID_SYSTEM)/core/libsystem/include) \
	    -I$(abspath $(ANDROID_SYSTEM)/core/libutils/binder/include) \
            -I$(abspath ../) \
	    -I$(abspath ../../)

CPPFLAGS +=-Wall -Werror \
        -Wno-unused-parameter \
        -Wno-unused-private-field \
        -Wno-unused-variable \
        -Wno-unknown-pragmas \
        -Wno-error=attributes \
        -fno-rtti

CPPFLAGS += -DCONFIG_CTRL_IFACE_CLIENT_DIR=\"/data/vendor/wifi/wpa/sockets\" \
        -DCONFIG_CTRL_IFACE_DIR=\"/data/vendor/wifi/wpa/sockets\" \
        -DCONFIG_WPA_CLI_HISTORY_DIR=\"/data/vendor/wifi/wpa\"

ifdef CONFIG_CTRL_IFACE_AIDL
CPPFLAGS += -DCONFIG_AIDL -DCONFIG_CTRL_IFACE_AIDL
endif

ifdef CONFIG_HS20
CPPFLAGS += -DCONFIG_HS20
CONFIG_INTERWORKING=y
endif

ifdef CONFIG_INTERWORKING
CPPFLAGS += -DCONFIG_INTERWORKING
NEED_GAS=y
endif

ifdef CONFIG_GAS
CPPFLAGS += -DCONFIG_GAS
endif

ifdef CONFIG_WPS
CPPFLAGS += -DCONFIG_WPS -DEAP_SERVER_WSC
endif

ifdef CONFIG_WEP
CPPFLAGS += -DCONFIG_WEP
endif

ifdef CONFIG_WEP_NFC
CPPFLAGS += -DCONFIG_WEP_NFC
endif

ifdef CONFIG_WEP_NR
CPPFLAGS += -DCONFIG_WEP_NR
endif

ifdef CONFIG_IEEE8021X_EAPOL
CPPFLAGS += -DIEEE8021X_EAPOL
endif

ifdef CONFIG_ANDROID
CPPFLAGS += -DANDROID -DCONFIG_ANDROID_LOG
endif

ifdef CONFIG_WNM
CPPFLAGS += -DCONFIG_WNM
endif

ifdef CONFIG_DRIVER_NL80211
CPPFLAGS += -DCONFIG_DRIVER_NL80211
ifdef CONFIG_DRIVER_NL80211_QCA
CPPFLAGS += -DCONFIG_DRIVER_NL80211_QCA
endif
endif

ifdef CONFIG_ERP
CPPFLAGS += -DCONFIG_ERP
endif

ifdef CONFIG_IEEE80211R
CPPFLAGS += -DCONFIG_IEEE80211R
endif

ifdef CONFIG_IEEE80211AH
CPPFLAGS += -DCONFIG_IEEE80211AH
endif

ifdef CONFIG_SAE
CPPFLAGS += -DCONFIG_SAE
NEED_HMAC_SHA256_KDF=y
NEED_ECC=y
ifdef CONFIG_SAE_PK
CPPFLAGS += -DCONFIG_SAE_PK
endif
endif

ifdef CONFIG_DPP
CPPFLAGS += -DCONFIG_DPP
NEED_HMAC_SHA384_KDF=y
NEED_HMAC_SHA512_KDF=y
NEED_SHA384=y
NEED_SHA512=y
NEED_GAS_SERVER=y
NEED_JSON=y
ifdef CONFIG_DPP2
CPPFLAGS += -DCONFIG_DPP2
endif
ifdef CONFIG_DPP3
CPPFLAGS += -DCONFIG_DPP3
endif
endif

ifdef CONFIG_OWE
CPPFLAGS += -DCONFIG_OWE
endif

ifdef CONFIG_EAP_TLS
CPPFLAGS += -DEAP_TLS
endif

ifdef CONFIG_EAP_PEAP
CPPFLAGS += -DEAP_PEAP
endif

ifdef CONFIG_EAP_TTLS
CPPFLAGS += -DEAP_TTLS
endif

ifdef CONFIG_EAP_MD5
CPPFLAGS += -DEAP_MD5
endif

ifdef CONFIG_EAP_MSCHAPV2
CPPFLAGS += -DEAP_MSCHAPV2
endif

ifdef CONFIG_EAP_GTC
CPPFLAGS += -DEAP_GTC
endif

ifdef CONFIG_EAP_OTP
CPPFLAGS += -DEAP_OTP
endif

ifdef CONFIG_EAP_SIM
CPPFLAGS += -DEAP_SIM
endif


ifdef CONFIG_EAP_LEAP
CPPFLAGS += -DEAP_LEAP
endif

ifdef CONFIG_EAP_OTP
CPPFLAGS += -DEAP_OTP
endif

ifdef CONFIG_EAP_AKA
CPPFLAGS += -DEAP_AKA
endif

ifdef CONFIG_EAP_AKA_PRIME
CPPFLAGS += -DEAP_AKA_PRIME
endif

ifdef CONFIG_EAP_PWD
CPPFLAGS += -DEAP_PWD
endif

ifdef CONFIG_WPS
CPPFLAGS += -DEAP_WSC -DCONFIG_WPS
endif

ifdef CONFIG_WPS_NFC
CPPFLAGS += -DCONFIG_WPS_NFC
endif

ifdef CONFIG_WPS_OOB
CPPFLAGS += -DCONFIG_WPS_OOB
endif

ifdef CONFIG_WPS_ER
CPPFLAGS += -DCONFIG_WPS_ER
endif

ifdef CONFIG_WPS_UPNP
CPPFLAGS += -DCONFIG_WPS_UPNP
endif

ifdef CONFIG_IEEE8021X_EAPOL
# IEEE 802.1X/EAPOL state machines (e.g., for RADIUS authentication)
CPPFLAGS += -DIEEE8021X_EAPOL
endif

ifdef CONFIG_AP
CPPFLAGS += -DCONFIG_AP \
	    -DCONFIG_NO_RADIUS \
	    -DCONFIG_NO_ACCOUNTING \
	    -DCONFIG_NO_VLAN \
	    -DEAP_SERVER \
	    -DEAP_SERVER_IDENTITY
endif

ifdef CONFIG_IEEE80211AC
CPPFLAGS += -DCONFIG_IEEE80211AC
endif

ifdef CONFIG_MBO
CPPFLAGS += -DCONFIG_MBO
endif

ifeq ($(CONFIG_TLS), openssl)
CPPFLAGS += -DCRYPTO_RSA_OAEP_SHA256
ifdef TLS_FUNCS
CPPFLAGS += -DEAP_TLS_OPENSSL
endif
endif

CPPFLAGS += -DCONFIG_SHA256

ifdef NEED_HMAC_SHA256_KDF
CPPFLAGS += -DCONFIG_HMAC_SHA256_KDF
endif

ifdef NEED_HMAC_SHA384_KDF
CPPFLAGS += -DCONFIG_HMAC_SHA256_KDF
endif

ifdef NEED_HMAC_SHA512_KDF
CPPFLAGS += -DCONFIG_HMAC_SHA256_KDF
endif

ifdef NEED_SHA384
CPPFLAGS += -DCONFIG_SHA384
endif

ifdef NEED_SHA512
CPPFLAGS += -DCONFIG_SHA512
endif

ifdef NEED_ECC
CPPFLAGS += -DCONFIG_ECC
endif

ifdef CONFIG_NO_RANDOM_POOL
CPPFLAGS += -DCONFIG_NO_RANDOM_POOL
endif

ifdef CONFIG_CTRL_IFACE
ifeq ($(CONFIG_CTRL_IFACE), y)
ifdef CONFIG_NATIVE_WINDOWS
CONFIG_CTRL_IFACE=named_pipe
else
CONFIG_CTRL_IFACE=unix
endif
endif
CPPFLAGS += -DCONFIG_CTRL_IFACE
ifeq ($(CONFIG_CTRL_IFACE), unix)
CPPFLAGS += -DCONFIG_CTRL_IFACE_UNIX
endif
endif

ifdef CONFIG_IPV6
# for eapol_test only
CPPFLAGS += -DCONFIG_IPV6
endif

ifdef NEED_GAS_SERVER
CPPFLAGS += -DCONFIG_GAS_SERVER
endif

ifdef CONFIG_P2P
CPPFLAGS += -DCONFIG_P2P
NEED_OFFCHANNEL=y
endif

ifdef NEED_OFFCHANNEL
CPPFLAGS += -DCONFIG_OFFCHANNEL
endif

ifdef NEED_JSON
CPPFLAGS += -DCONFIG_JSON
endif

ifdef CONFIG_PMKSA_CACHE_EXTERNAL
CPPFLAGS += -DCONFIG_PMKSA_CACHE_EXTERNAL
endif

ifdef CONFIG_MESH
CPPFLAGS += -DCONFIG_MESH
endif

ifdef CONFIG_MACSEC
CPPFLAGS += -DCONFIG_MACSEC
endif

ifdef CONFIG_MATCH_IFACE
CPPFLAGS += -DCONFIG_MATCH_IFACE
endif

ifdef CONFIG_NAN_USD
CPPFLAGS += -DCONFIG_NAN_USD
endif


LDFLAGS += 	-L$(LIBNDK_BINDER_DIR) \
    	   	-L$(LIB_BASE_DIR) \
           	-L$(LIB_UTILS_DIR) \
           	-L$(LIB_LOG_DIR) \
   	   	-L$(LIB_SSL_DIR) \
    	   	-L$(LIB_CPP) \
    	   	-L$(LIB_CPP_ABI) \
    		-lndk \
    		-lbinder_ndk \
    		-lbase \
    		-lutils \
    		-llog \
    		-lssl \
    		-lc++ \
    		-lc++abi

all: libwpa_aidl.a

libwpa_aidl.a: $(AIDL_OBJS)
	@echo "AR  $@"
	@rm -f $@
	$(AR) crs $@ $^

%.o: %.cpp
	@echo "CXX $<"
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o libwpa_aidl.a

