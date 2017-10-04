 # This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt
include(../shared/config.pri)
TEMPLATE = lib
CONFIG += c++11
TARGET = objectfinder

QT += quick

DEFINES += OBJECT_FINDER_EXPORT

DESTDIR = $$PWD/../../lib/$$OS/$$TYPEDIR

HEADERS +=  \
    objectfinder.h \
    qt/qhooks_p.h \
    objectfinder_private.h

SOURCES +=  \
    objectfinder.cpp

message($$[QT_INSTALL_HEADERS])
message($${QT_MAJOR_VERSION})
message($${QT_VERSION})

windows{
lessThan(QT_MAJOR_VERSION, 6){
    lessThan(QT_MINOR_VERSION, 8){
        INCLUDEPATH += $$[QT_INSTALL_HEADERS]/../../Src/qtbase/include/QtCore/$${QT_VERSION}
        INCLUDEPATH += $$[QT_INSTALL_HEADERS]/../../Src/qtbase/include/QtCore/$${QT_VERSION}/QtCore
        INCLUDEPATH += $$[QT_INSTALL_HEADERS]/../../Src/qtbase/include/QtCore/$${QT_VERSION}/QtCore/private
        INCLUDEPATH += $$[QT_INSTALL_HEADERS]/../../Src/qtdeclarative/include/QtQml/$${QT_VERSION}
        INCLUDEPATH += $$[QT_INSTALL_HEADERS]/../../Src/qtdeclarative/include/QtQml/$${QT_VERSION}/QtQml
        INCLUDEPATH += $$[QT_INSTALL_HEADERS]/../../Src/qtdeclarative/include/QtQml/$${QT_VERSION}/QtQml/private
        } else {
        INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtCore/$${QT_VERSION}
        INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtCore/$${QT_VERSION}/QtCore
        INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtCore/$${QT_VERSION}/QtCore/private
        INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtQml/$${QT_VERSION}
        INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtQml/$${QT_VERSION}/QtQml
        INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtQml/$${QT_VERSION}/QtQml/private
        }
    }
}


linux{
    INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtCore/$${QT_VERSION}/QtCore
    INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtCore/$${QT_VERSION}/QtCore/private
    INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtQml/$${QT_VERSION}/QtQml/private
    INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtQml/$${QT_VERSION}/QtQml
}

message($$INCLUDEPATH)

macx{
    INCLUDEPATH += $$[QT_INSTALL_HEADERS]/../lib/QtCore.framework/Versions/$${QT_MAJOR_VERSION}/Headers/$${QT_VERSION}
    INCLUDEPATH += $$[QT_INSTALL_HEADERS]/../lib/QtCore.framework/Versions/$${QT_MAJOR_VERSION}/Headers/$${QT_VERSION}/QtCore/private
    INCLUDEPATH += $$[QT_INSTALL_HEADERS]/../lib/QtCore.framework/Versions/$${QT_MAJOR_VERSION}/Headers/$${QT_VERSION}/QtCore
    INCLUDEPATH += $$[QT_INSTALL_HEADERS]/../lib/QtQml.framework/Versions/$${QT_MAJOR_VERSION}/Headers/$${QT_VERSION}
    INCLUDEPATH += $$[QT_INSTALL_HEADERS]/../lib/QtQml.framework/Versions/$${QT_MAJOR_VERSION}/Headers/$${QT_VERSION}/QtQml
    INCLUDEPATH += $$[QT_INSTALL_HEADERS]/../lib/QtQml.framework/Versions/$${QT_MAJOR_VERSION}/Headers/$${QT_VERSION}/QtQml/private
    INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtQmlDevTools/$${QT_VERSION}/QtQmlDevTools
}

android{
    INCLUDEPATH += $$[QT_INSTALL_HEADERS]/../../android_armv7/include/QtCore/$${QT_VERSION}
    INCLUDEPATH += $$[QT_INSTALL_HEADERS]/../../android_armv7/include/QtCore/$${QT_VERSION}/QtCore/
    INCLUDEPATH += $$[QT_INSTALL_HEADERS]/../../android_armv7/include/QtCore/$${QT_VERSION}/QtCore/private
    INCLUDEPATH += $$[QT_INSTALL_HEADERS]/../../android_armv7/include/QtQml/$${QT_VERSION}
    INCLUDEPATH += $$[QT_INSTALL_HEADERS]/../../android_armv7/include/QtQml/$${QT_VERSION}/QtQml
    INCLUDEPATH += $$[QT_INSTALL_HEADERS]/../../android_armv7/include/QtQml/$${QT_VERSION}/QtQml/private
    INCLUDEPATH += $$[QT_INSTALL_HEADERS]/../../android_armv7/include/QtQml/$${QT_VERSION}/QtQmlDevTools
}


