# This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt 
include(shared/config.pri)

TEMPLATE = subdirs
SUBDIRS =   \
    objectfinder \
    lib     \
    test    \
    quts    \
    store   \
    qt      \
    audio   \
    time    \
    std     \
    math    \
    graphics


lib.depends += objectfinder
lib.file = qutslib/qutslib.pro
test.file = test/QutsConsole/QutsConsole.pro
test.depends += lib
quts.depends += lib
audio.depends += lib
graphics.depends += lib

DISTFILES += \
    License.txt \
    $$PWD/test/scripts/twinkletwinkle.qts \
    $$PWD/test/scripts/fibonacci.qts \
    $$PWD/test/scripts/inputtest.qts \
    $$PWD/test/scripts/test1.qts \
    $$PWD/test/scripts/test2.qts \
    $$PWD/test/scripts/testSignal.qts \
    $$PWD/test/scripts/signals.qts \
    $$PWD/test/scripts/eval.qts \
    $$PWD/test/scripts/math.qts \
    $$PWD/test/scripts/testfind.qts \
    $$PWD/test/scripts/testfind2.qts \
    $$PWD/test/scripts/testfind3.qts \
    $$PWD/test/scripts/unit.qts \
    $$PWD/test/scripts/99bottlesOfBeer.qts \
    $$PWD/test/scripts/testQt1.qts \
    $$PWD/test/scripts/testSignal.qts \
    $$PWD/test/scripts/fibonacci2.qts \
    $$PWD/test/scripts/gwtlchallenge.qts \
    $$PWD/test/scripts/gwtlchallenge2.qts \
    $$PWD/test/scripts/fibonacci3.qts \
    $$PWD/test/scripts/testlocals.qts \
    $$PWD/test/scripts/store.qts \
    $$PWD/test/scripts/functions.qts \
    $$PWD/test/scripts/graphics.qts \
    $$PWD/test/scripts/watch.qts



