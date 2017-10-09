# This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt 
ios:OS = ios
macx:OS = osx
win32-g++:OS = mingw
win32:!win32-g++:OS = win32
android:OS = android
linux:!android:OS = linux

CONFIG-=debug_and_release

equals(OS, win32){
    DEFINES += _ALLOW_KEYWORD_MACROS
    DEFINES += constexpr=const
    #DEFINES += Q_COMPILER_INITIALIZER_LISTS
    DEFINES += BROKEN_COMPILER # msvc13, msvc15/7 to be tested
}
CONFIG(debug, debug|release) {
    TYPEDIR = "debug"
}else {
    TYPEDIR = "release"
}

HEADERS += \
    $$PWD/qutsdll.h
