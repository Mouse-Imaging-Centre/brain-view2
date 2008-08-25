QT += opengl

COINLIBS = $$system(coin-config --ldflags)
COININCLUDE == $$system(coin-config --includedir)

INCLUDEPATH += $$COININCLUDE
LIBS += $$COINLIBS

!macx {
 	LIBS += -lCoin
 	LIBS += -L/usr/local/lib -lQuarter
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
          ResourceForm.h
SOURCES = MainWindow.cpp \
          ColourBarForm.cpp \
          BrainQuarter.cpp \
          TreeItem.cpp \
          TreeModel.cpp \
          ResourceForm.cpp \
          main.cpp
