QT       += core gui sql multimedia multimediawidgets printsupport charts network
QT +=svg
QT += core gui serialport


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    client.cpp \
    commande.cpp \
    connexion.cpp \
    dialog.cpp \
    generateurpdf.cpp \
    ihmgraphique.cpp \
    main.cpp \
    mainwindow.cpp \
    matieres.cpp \
    perso.cpp \
    piece.cpp \
    qrcode.cpp \
    qrcodegeneratordemo.cpp \
    qrcodegeneratorworker.cpp \
    qrwidget.cpp \
    service.cpp \
    smtpclient.cpp

HEADERS += \
    client.h \
    commande.h \
    connexion.h \
    dialog.h \
    generateurpdf.h \
    ihmgraphique.h \
    mainwindow.h \
    matieres.h \
    perso.h \
    piece.h \
    qrcode.h \
    qrwidget.h \
    service.h \
    smtpclient.h

FORMS += \
    dialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc

DISTFILES += \
    images/ajj.png \
    images/ajouter.png \
    images/ann.png \
    images/annuler.png \
    images/autogenius-logo.png \
    images/clients.png \
    images/commandes.png \
    images/env.png \
    images/equipements.png \
    images/fich.png \
    images/list_1625895.png \
    images/mail.png \
    images/modd.png \
    images/modifier.png \
    images/pdf.png \
    images/personnels.png \
    images/pieces.png \
    images/rech.png \
    images/ree.png \
    images/search.png \
    images/services.png \
    images/smm.png \
    images/sms.png \
    images/stat.png \
    images/supp.png \
    images/supprimer.png \
    images/user_6542967.png \
    images/valid.png
