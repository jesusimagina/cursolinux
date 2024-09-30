DESCRIPTION = "CursoCrypto"
LICENSE = "CLOSED"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/LICENSE;md5=<checksum>"

SRC_URI = "file://common.c \
           file://verifyCert.c \
           file://verifySignature.c \
           file://common.h \
           file://linuxCrypt.h"

S = "${WORKDIR}"

inherit pkgconfig

do_compile() {
    ${CC} ${CFLAGS} -c common.c verifyCert.c verifySignature.c `pkg-config --cflags openssl`
    ar rcs libcursocrypto.a common.o verifyCert.o verifySignature.o
}

do_install() {
    install -d ${D}${libdir}
    install -m 0755 libcursocrypto.a ${D}${libdir}
    install -d ${D}${includedir}
    install common.h ${D}${includedir}
    install linuxCrypt.h ${D}${includedir}
}

FILES_${PN} = "${libdir}/libcursocrypto.a"
FILES_${PN} += "/usr/include/*"

DEPENDS = "openssl"
