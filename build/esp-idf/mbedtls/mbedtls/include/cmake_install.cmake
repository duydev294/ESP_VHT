# Install script for directory: H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/VHT_21_6_1")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/Espressif/tools/xtensa-esp32-elf/esp-2021r2-patch3-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-objdump.exe")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/mbedtls" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ FILES
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/aes.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/aesni.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/arc4.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/aria.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/asn1.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/asn1write.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/base64.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/bignum.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/blowfish.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/bn_mul.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/camellia.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/ccm.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/certs.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/chacha20.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/chachapoly.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/check_config.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/cipher.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/cipher_internal.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/cmac.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/compat-1.3.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/config.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/config_psa.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/constant_time.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/ctr_drbg.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/debug.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/des.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/dhm.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/ecdh.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/ecdsa.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/ecjpake.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/ecp.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/ecp_internal.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/entropy.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/entropy_poll.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/error.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/gcm.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/havege.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/hkdf.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/hmac_drbg.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/md.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/md2.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/md4.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/md5.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/md_internal.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/memory_buffer_alloc.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/net.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/net_sockets.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/nist_kw.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/oid.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/padlock.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/pem.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/pk.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/pk_internal.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/pkcs11.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/pkcs12.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/pkcs5.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/platform.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/platform_time.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/platform_util.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/poly1305.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/psa_util.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/ripemd160.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/rsa.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/rsa_internal.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/sha1.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/sha256.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/sha512.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/ssl.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/ssl_cache.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/ssl_ciphersuites.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/ssl_cookie.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/ssl_internal.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/ssl_ticket.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/threading.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/timing.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/version.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/x509.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/x509_crl.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/x509_crt.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/x509_csr.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/mbedtls/xtea.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/psa" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ FILES
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/psa/crypto.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/psa/crypto_builtin_composites.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/psa/crypto_builtin_primitives.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/psa/crypto_compat.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/psa/crypto_config.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/psa/crypto_driver_common.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/psa/crypto_driver_contexts_composites.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/psa/crypto_driver_contexts_primitives.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/psa/crypto_extra.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/psa/crypto_platform.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/psa/crypto_se_driver.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/psa/crypto_sizes.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/psa/crypto_struct.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/psa/crypto_types.h"
    "H:/Code/ESP32-IDF/espIdf/components/mbedtls/mbedtls/include/psa/crypto_values.h"
    )
endif()

