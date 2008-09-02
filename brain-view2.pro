CONFIG += qt debug

QT += opengl

COINLIBS = $$system(coin-config --ldflags)
COININCLUDE = $$system(coin-config --includedir)

INCLUDEPATH += $$COININCLUDE
LIBS += $$COINLIBS

!macx {
 	LIBS += -lCoin
 	LIBS += -L/usr/local/lib -lQuarter
 	INCLUDEPATH += /projects/mice/share/arch/linux64/include
 	LIBS += -L/projects/mice/share/arch/linux64/lib/ -lbicInventor -lbicpl -lvolume_io2 -lminc2
}

macx {
	LIBS += -Wl,-framework -Wl,Quarter
	LIBS += -L/usr/local/minc2/lib -lbicpl -lvolume_io2 -lminc2
	INCLUDEPATH += /usr/local/minc2/include
}

INCLUDEPATH += /usr/local/include

FORMS = colourBarForm.ui \
		resourceForm.ui \
		geometryProps.ui
		
HEADERS = MainWindow.h \
          ColourBarForm.h \
          BrainQuarter.h \
          TreeItem.h \
          TreeModel.h \
          GeometryScene.h \
          ResourceForm.h \
          GeometryNode.h
          
SOURCES = MainWindow.cpp \
          ColourBarForm.cpp \
          BrainQuarter.cpp \
          TreeItem.cpp \
          TreeModel.cpp \
          ResourceForm.cpp \
          GeometryScene.cpp \
          GeometryNode.cpp \
          main.cpp
