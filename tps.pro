######################################################################
# Automatically generated by qmake (3.0) Wed Apr 1 12:44:09 2015
######################################################################

TEMPLATE = app
QT += widgets
QT += multimedia
QT += core
QT += sql
TARGET = tps 
INCLUDEPATH += . inc

OBJECTS_DIR=obj/
CONFIG+=qt debug c++17
MOC_DIR = moc/

# Input
HEADERS += inc/*.h
SOURCES += src/*.cpp 
LIBS += -L./lib/.
LIBS += -L./usr/lib/x86_64-linux-gnu/

TRANSLATIONS += \
    umzug_en_US.ts

