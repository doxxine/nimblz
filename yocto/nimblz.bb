SUMMARY = "nimblz — BLE Reliability Layer for BlueZ"
DESCRIPTION = "C library wrapping BlueZ/D-Bus with a NimBLE-like API. \
Provides automatic reconnect, advertising lifecycle management, \
multi-connection support, dual-role (server + client), and \
chip-specific quirk handling for Intel, Broadcom, and Realtek."
HOMEPAGE = "https://github.com/mathandemotion/nimblz"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${S}/LICENSE;md5=placeholder"

SRC_URI = "git://github.com/mathandemotion/nimblz.git;protocol=https;branch=main"
SRCREV = "${AUTOREV}"

S = "${WORKDIR}/git"

DEPENDS = "glib-2.0 bluez5"

inherit cmake pkgconfig

EXTRA_OECMAKE = " \
    -DNIMBLZ_BUILD_EXAMPLES=ON \
    -DNIMBLZ_BUILD_TESTS=OFF \
"

FILES:${PN} = " \
    ${libdir}/libnimblz.so* \
"

FILES:${PN}-dev = " \
    ${includedir}/nimblz/ \
    ${libdir}/libnimblz.a \
"

FILES:${PN}-examples = " \
    ${bindir}/peripheral \
    ${bindir}/central \
    ${bindir}/dual_role \
"

PACKAGES =+ "${PN}-examples"

RDEPENDS:${PN} = "bluez5"
