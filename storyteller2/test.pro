QT += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = storyteller
TEMPLATE = app

SOURCES += main.cpp \
           DecisionNode.cpp \
           Edge.cpp \
           MainWindow.cpp \
           PrimaryNode.cpp \
           DecisionNode.cpp \
           Edge.cpp

HEADERS += MainWindow.h \
           PrimaryNode.h \
           DecisionNode.h \
           Edge.h

FORMS +=
