CONFIG += qt debug

QT += opengl

COINLIBS = $$system(coin-config --ldflags)
COININCLUDE == $$system(coin-config --includedir)

INCLUDEPATH += $$COININCLUDE
LIBS += $$COINLIBS

!macx {
 	LIBS += -lCoin
 	LIBS += -L/usr/local/lib -lQuarter
 	
 	LIBS += -L/projects/mice/share/arch/linux64/lib/ -lbicInventor -lbicpl -lvolume_io2 -lminc2
}

macx {
	LIBS += -Wl,-framework -Wl,Quarter
}

INCLUDEPATH += /usr/local/include

FORMS = colourBarForm.ui \
		resourceForm.ui
		
HEADERS = MainWindow.h \
          ColourBarForm.h \
          BrainQuarter.h \
          TreeItem.h \
          TreeModel.h \
          GeometryScene.h \
          ResourceForm.h
SOURCES = MainWindow.cpp \
          ColourBarForm.cpp \
          BrainQuarter.cpp \
          TreeItem.cpp \
          TreeModel.cpp \
          ResourceForm.cpp \
          GeometryScene.cpp \
          main.cpp
