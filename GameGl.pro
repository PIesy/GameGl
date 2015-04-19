TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++1y
QMAKE_LFLAGS += -lGL -lGLEW -lGLU -lSDL2

SOURCES += main.cpp \
    Modules/graphicsmodule.cpp \
    Modules/soundmodule.cpp \
    Modules/resourcesmodule.cpp \
    Core/enginecore.cpp \
    Core/eventshandler.cpp \
    Graphics/sdlwrapper.cpp \
    Graphics/openglsdl.cpp \
    Helpers/helpers.cpp \
    Public/coreinterface.cpp \
    Core/worker.cpp \
    Graphics/openglwrapper.cpp \
    Graphics/renderqueue.cpp \
    Graphics/renderer.cpp \
    Resources/filereader.cpp \
    Graphics/dummyglcontext.cpp \
    Graphics/glshader.cpp \
    Graphics/glprogram.cpp \
    Ui/basicuielements.cpp \
    Ui/basicshapes.cpp \
    Ui/uilayer.cpp \
    Ui/uielements.cpp \
    Graphics/vertexobject.cpp \
    Logger/logger.cpp \
    Core/broadcaster.cpp \
    Core/invokable.cpp \
    Core/threadpool.cpp \
    Core/eventshandler_new.cpp \
    Tests/tests.cpp \
    Modules/iomodule.cpp \
    Modules/module.cpp \
    Core/workerqueueproxy.cpp \
    Core/actionpack.cpp

HEADERS += \
    Modules/graphicsmodule.h \
    Modules/moduleinterface.h \
    Modules/soundmodule.h \
    Modules/resourcesmodule.h \
    Core/enginecore.h \
    Core/enginecoreincludes.h \
    Core/eventshandler.h \
    Graphics/graphicsapi.h \
    Graphics/openglsdl.h \
    Helpers/helpers.h \
    Public/coreinterface.h \
    Core/worker.h \
    Core/enginedatastructures.h \
    Graphics/renderqueue.h \
    Graphics/renderdefs.h \
    Graphics/renderer.h \
    Resources/resourcesapi.h \
    Resources/filereader.h \
    Core/engineinterface.h \
    Core/engineinitializer.h \
    Public/apis.h \
    Ui/basicuielements.h \
    Ui/elementinterface.h \
    Ui/basicshapes.h \
    Public/ui.h \
    Ui/uilayer.h \
    Ui/uielements.h \
    Logger/logger.h \
    Core/broadcaster.h \
    Core/invokable.h \
    Core/threadpool.h \
    Core/eventshandler_new.h \
    Core/eventshandler_interfaces.h \
    Tests/tests.h \
    Core/executable.h \
    IO/ioapi.h \
    Modules/iomodule.h \
    Modules/module.h \
    IO/ioevents.h \
    IO/devid.h \
    Core/service.h \
    Api/apibase.h \
    Core/workerqueueproxy.h \
    Core/actionpack.h

OTHER_FILES += \
    Shaders/FragmentShader.glsl \
    Shaders/VertexShader.glsl
