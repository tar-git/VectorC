TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    main.c \
    test_suite.c \
    vector.c \
    vector_service.c

HEADERS += \
    test_suite.h \
    vector.h \
    vector_service.h
