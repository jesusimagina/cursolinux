DESCRIPTION = "Libreria crypto"
PRIORITY = "optional"
SECTION = "examples"
LICENSE = "CLOSED"

SRC_URI = "file://common.c \
           file://verifyCert.c \
           file://verifySignature.c \
           file://common.h \
           file://linuxCrypt.h \
           "
           
S = "${WORKDIR}"

DEPENDS = "openssl"

do_compile(){
    ${CC} ${CFLAGS} ${LDFLAGS} -shared -fpic -g -o libcript.so.1.0 verifyCert.c verifySignature.c common.c
}

do_install() {
    install -d ${D}${libdir}
    install -d ${D}${includedir}
    install -m 0755 libcript.so.1.0 ${D}${libdir}
    install common.h ${D}${includedir}
    install linuxCrypt.h ${D}${includedir}
    cd ${D}${libdir}
    ln -s libcript.so.1.0 libcript.so
}


FILES_${PN} += "/usr/lib/*"
FILES_${PN} += "/usr/include/*"
