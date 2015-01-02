#-------------------------------------------------
#
# Project created by QtCreator 2014-10-09T15:47:48
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = TerrainGenerator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    camera.cpp \
    glwidget_input.cpp \
    glwidget_slots.cpp \
    texture.cpp \
    Terrain/terrainpatch.cpp \
    Terrain/terraingenerator.cpp \
    utils.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    camera.h \
    texture.h \
    terrainpatch.h \
    Terrain/terrainpatch.h \
    terraingenerator.h \
    Terrain/terraingenerator.h \
    utils.h \
    model.h \
    Terrain/terrainpatchsettings.h

FORMS    += mainwindow.ui

RESOURCES += \
    Resources.qrc
