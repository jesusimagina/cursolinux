DESCRIPTION = "Programa de arranque seguro"
PRIORITY = "optional"
SECTION = "examples"
LICENSE = "CLOSED"

SRC_URI = "file://secboot.c"

S = "${WORKDIR}"

inherit pkgconfig

DEPENDS = "hsmkm openssl libcursocrypto"

do_compile(){
    ${CC} ${CFLAGS} ${LDFLAGS} secboot.c -o secboot -L${STAGING_LIBDIR} -lcursocrypto `pkg-config --libs --cflags openssl`
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 secboot ${D}${bindir}
}


FILES_${PN} += "/usr/bin/secboot"
