#-------------------------------------------------
#
# Project created by QtCreator 2012-01-26T17:25:33
#
#-------------------------------------------------

QT       += core gui

TARGET = CellSim
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    simulation.cpp \
    atom.cpp \
    atomeditor.cpp \
    globals.cpp \
    elements/uranium.cpp \
    elements/sugar.cpp \
    elements/ribosome.cpp \
    elements/neuron.cpp \
    elements/motorneuron.cpp \
    elements/mitochondrion.cpp \
    elements/membrane.cpp \
    elements/gold.cpp \
    elements/enzyme.cpp \
    elements/dna.cpp \
    elements/delta.cpp \
    elements/carbon.cpp \
    elements/beta.cpp \
    elements/atp.cpp \
    elements/acid.cpp \
    document.cpp \
    bins.cpp \
    logger.cpp \
    logblock.cpp

HEADERS  += mainwindow.h \
    simulation.h \
    atom.h \
    atomeditor.h \
    globals.h \
    elements/uranium.h \
    elements/sugar.h \
    elements/ribosome.h \
    elements/neuron.h \
    elements/motorneuron.h \
    elements/mitochondrion.h \
    elements/membrane.h \
    elements/gold.h \
    elements/enzyme.h \
    elements/dna.h \
    elements/delta.h \
    elements/carbon.h \
    elements/beta.h \
    elements/atp.h \
    elements/acid.h \
    document.h \
    bins.h \
    logger.h \
    logblock.h

FORMS    += mainwindow.ui \
    atomeditor.ui

OTHER_FILES += \
    icons/002_80.png \
    icons/002_79.png \
    icons/002_78.png \
    icons/002_77.png \
    icons/002_76.png \
    icons/002_75.png \
    icons/002_74.png \
    icons/002_73.png \
    icons/002_72.png \
    icons/002_71.png \
    icons/exclamation.png

RESOURCES += \
    icons.qrc

INCLUDEPATH += C:/boost_1_49_0
