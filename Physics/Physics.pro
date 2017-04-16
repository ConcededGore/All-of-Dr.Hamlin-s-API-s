QT += widgets
QT += multimedia

CONFIG += gui
CONFIG += c++11

HEADERS     = \
    window.h \
    graphics.h \
    mssm.h \
    plugin.h \
    vec2d.h \
    vec3d.h
SOURCES     = \
    main.cpp \
    graphics.cpp \
    mssm.cpp \
    plugin.cpp \
    vec2d.cpp \
    vec3d.cpp

DISTFILES += \
    data/ShortLaser.wav \
    data/comp.png

copydata.commands = $(COPY_DIR) \"$$shell_path($$PWD\\data)\" \"$$shell_path($$OUT_PWD)\"
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata
