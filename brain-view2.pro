COINLIBS = $$system(coin-config --ldflags)
COININCLUDE == $$system(coin-config --includedir)

INCLUDEPATH += $$COININCLUDE
LIBS += $$COINLIBS -lCoin

LIBS += -L/usr/local/lib -lQuarter
INCLUDEPATH += /usr/local/include

FORMS = colourBarForm.ui
HEADERS = MainWindow.h \
          ColourBarForm.h \
          BrainQuarter.h
SOURCES = MainWindow.cpp \
          ColourBarForm.cpp \
          BrainQuarter.cpp \
          main.cpp
