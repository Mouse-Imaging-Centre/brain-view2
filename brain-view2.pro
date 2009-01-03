MINCDIR = /usr/local/minc2

# MINCDIR = /projects/mice/share/arch/linux64
QUARTERDIR = /usr/local
COINLIBS = $$system(coin-config --ldflags)
COININCLUDE = $$system(coin-config --includedir)
CONFIG += qt
CONFIG += debug
QT += opengl
INCLUDEPATH += $$COININCLUDE
LIBS += $$COINLIBS
!macx { 
    LIBS += -lCoin
    LIBS += -L$${QUARTERDIR} \
        -lQuarter
    INCLUDEPATH += $${MINCDIR}/include
    INCLUDEPATH += $${QUARTERDIR}/include
    LIBS += -L$${MINCDIR}/lib/ \
        -loobicpl \
        -lpcre++ \
        -lpcre \
        -lbicpl \
        -lvolume_io2 \
        -lminc2 \
        -lhdf5 \
        -lnetcdf
}
macx { 
    LIBS += -Wl,-framework \
        -Wl,Quarter
    LIBS += -L$${MINCDIR}/lib \
        -loobicpl \
        -lpcre++ \
        -lpcre \
        -lbicpl \
        -lvolume_io2 \
        -lminc2
    INCLUDEPATH += $${MINCDIR}/include
}
INCLUDEPATH += /usr/local/include
FORMS = textureColumnForm.ui \
    resourceForm.ui \
    geometryProps.ui
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
