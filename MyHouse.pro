#-------------------------------------------------
#
# Project created by QtCreator 2013-03-29T21:59:00
#
#-------------------------------------------------

QT       += core gui sql

TARGET = MyHouse
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    ThumbnailsView.cpp \
    CategoriesView.cpp \
    Sqlite.cpp \
    SlideWidget.cpp \
    ImageUtil.cpp \
    Term.cpp \
    TermTaxonomy.cpp \
    TermRelationship.cpp \
    Image.cpp \
    AddImagesThread.cpp \
    AddTermDialog.cpp \
    ListItemDelegate.cpp \
    TableTermTaxonomyUtil.cpp \
    TableTermsUtil.cpp \
    TableTermRelationshipsUtil.cpp \
    TableImagesUtil.cpp \
    AddImagesProgressDialog.cpp \
    EditTermDialog.cpp \
    Favorites.cpp \
    TableFavoritesUtil.cpp \
    ImagePropertyWidget.cpp

HEADERS  += MainWindow.h \
    ThumbnailsView.h \
    CategoriesView.h \
    Sqlite.h \
    SlideWidget.h \
    ImageUtil.h \
    Term.h \
    TermTaxonomy.h \
    TermRelationship.h \
    Image.h \
    AddImagesThread.h \
    AddTermDialog.h \
    ListItemDelegate.h \
    TableTermTaxonomyUtil.h \
    TableTermsUtil.h \
    TableTermRelationshipsUtil.h \
    TableImagesUtil.h \
    AddImagesProgressDialog.h \
    EditTermDialog.h \
    Favorites.h \
    TableFavoritesUtil.h \
    ImagePropertyWidget.h

FORMS    += \
    AddTermDialog.ui \
    AddImagesProgressDialog.ui \
    ImagePropertyWidget.ui

RESOURCES += \
    MyHouse.qrc

win32 {
    RC_FILE = MyHouse.rc
}
