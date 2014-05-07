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
    Game/gamecore.cpp \
    Game/mainmenu.cpp \
    Game/game.cpp

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
    Game/gamecore.h \
    Game/gamestates.h \
    Game/gamecoreinterface.h \
    Game/game.h

OTHER_FILES += \
    Shaders/FragmentShader.glsl \
    Shaders/VertexShader.glsl
