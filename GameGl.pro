TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11
QMAKE_LFLAGS += -lGL -lGLEW -lGLU -lSDL2

SOURCES += main.cpp \
    Modules/graphicsmodule.cpp \
    Modules/soundmodule.cpp \
    Modules/resourcesmodule.cpp \
    Core/enginecore.cpp \
    Graphics/openglsdl.cpp \
    Helpers/helpers.cpp \
    Public/coreinterface.cpp \
    Core/worker.cpp \
    Graphics/renderer.cpp \
    Resources/filereader.cpp \
    Graphics/glshader.cpp \
    Graphics/glprogram.cpp \
    Ui/basicuielements.cpp \
    Ui/basicshapes.cpp \
    Ui/uilayer.cpp \
    Ui/uielements.cpp \
    Graphics/vertexobject.cpp \
    Logger/logger.cpp \
    Core/broadcaster.cpp \
    Core/threadpool.cpp \
    Tests/tests.cpp \
    Modules/module.cpp \
    Core/workerqueueproxy.cpp \
    Input/sdlinput.cpp \
    Modules/inputmodule.cpp \
    Helpers/action.cpp \
    Helpers/generic_invokable.cpp \
    Helpers/task.cpp \
    Core/eventshandler.cpp \
    Helpers/invokablepack.cpp \
    Core/service_cluster.cpp \
    Core/service.cpp \
    Helpers/repeteadfuture.cpp \
    Graphics/renderdefs.cpp \
    Public/shaderreader.cpp \
    Graphics/sdlclasses.cpp \
    Math/mathhelpers.cpp \
    Public/meshreader3ds.cpp

HEADERS += \
    Modules/graphicsmodule.h \
    Modules/moduleinterface.h \
    Modules/soundmodule.h \
    Modules/resourcesmodule.h \
    Core/enginecore.h \
    Core/enginecoreincludes.h \
    Graphics/graphicsapi.h \
    Graphics/openglsdl.h \
    Helpers/helpers.h \
    Public/coreinterface.h \
    Core/worker.h \
    Core/enginedatastructures.h \
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
    Core/threadpool.h \
    Core/eventshandler_interfaces.h \
    Tests/tests.h \
    Core/executable.h \
    Modules/module.h \
    Core/service.h \
    Api/apibase.h \
    Core/workerqueueproxy.h \
    Input/inputapi.h \
    Input/inputevents.h \
    Input/inputdata.h \
    Input/sdlinput.h \
    Modules/inputmodule.h \
    Helpers/task.h \
    Helpers/invokable.h \
    Helpers/action.h \
    Helpers/function_traits.h \
    Helpers/generic_invokable.h \
    Core/eventshandler.h \
    Helpers/invokablepack.h \
    Helpers/executor.h \
    Core/service_cluster.h \
    Helpers/repeteadfuture.h \
    Helpers/repeatedpromise.h \
    Helpers/dataobject.h \
    Math/matrix.h \
    Helpers/sharedstate.h \
    Graphics/graphicsclasses.h \
    Public/shaderreader.h \
    Graphics/sdlclasses.h \
    Helpers/genericcondition.h \
    Graphics/glprogram.h \
    Graphics/glshader.h \
    Helpers/size.h \
    Math/mathconstants.h \
    Math/mathdefines.h \
    Math/mathhelpers.h \
    Graphics/uniform.h \
    Graphics/gluniform.h \
    Public/meshreader3ds.h

OTHER_FILES += \
    Shaders/FragmentShader.glsl \
    Shaders/VertexShader.glsl
