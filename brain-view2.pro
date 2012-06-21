VERSION = 0.1.1


# Note: Remember to run:
#      >qmake -spec macx-g++       [ regen the make file ]
#  OR  >qmake -spec linux-g++
#
#      >make                       [ build ]


# set where to get the Minc libs and includes, bicInventor, Quarter, hdf5
# ... there really *must* be a better way of doing this
#Example:
#qmake-qt4 MINCDIR=/projects/mice/share/arch/linux-x86_64-eglibc2_11_1/ QUARTERDIR=/projects/mice/share/arch/linux-x86_64-eglibc2_11_1/ HDF5DIR=/projects/mice/share/arch/linux-x86_64-eglibc2_11_1/ INVENTORDIR=/projects/mice/share/arch/linux-x86_64-eglibc2_11_1/ brain-view2.pro

COINLIBS = $$system(coin-config --ldflags)
COININCLUDE = $$system(coin-config --includedir)
CONFIG += qt
CONFIG += debug
QT += opengl

INCLUDEPATH += $${QUARTERDIR}/include
INCLUDEPATH += $${MINCDIR}/include
INCLUDEPATH += $$COININCLUDE

LIBS += $$COINLIBS
!macx { 
    LIBS += -lCoin
    LIBS += -L$${QUARTERDIR}/lib/ \
        -lQuarter
}
macx { 
    LIBS += -Wl,-framework \
        -Wl,Quarter
}
LIBS += -L$${MINCDIR}/lib \
        -loobicpl \
        -lpcre++ \
        -lbicpl \
        -lvolume_io2 \
        -lminc2

LIBS += -L$${INVENTORDIR}/lib \
        -lbicInventor

LIBS += -L$${HDF5DIR}/lib/ \
        -lhdf5 \
        -lhdf5_cpp

FORMS = textureColumnForm.ui \
    resourceForm.ui \
    geometryProps.ui \
    tagProps.ui \
    labelProps.ui
HEADERS = MainWindow.h \
    BrainQuarter.h \
    TreeItem.h \
    TreeModel.h \
    GeometryScene.h \
    ResourceForm.h \
    GeometryNode.h \
    textureColumn.h \
    textureFileItem.h \
    tagFileItem.h \
    tagPointItem.h
SOURCES = MainWindow.cpp \
    BrainQuarter.cpp \
    TreeItem.cpp \
    TreeModel.cpp \
    ResourceForm.cpp \
    GeometryScene.cpp \
    GeometryNode.cpp \
    main.cpp \
    textureColumn.cpp \
    textureFileItem.cpp \
    tagFileItem.cpp \
    tagPointItem.cpp
RESOURCES += colourbars.qrc
