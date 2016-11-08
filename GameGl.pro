TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11
QMAKE_LFLAGS += -lGL -lGLEW -lGLU -lSDL2 -lIL -lILU

OTHER_FILES += \
    Shaders/FragmentShader.glsl \
    Shaders/VertexShader.glsl

HEADERS += \
    Api/apibase.h \
    Core/events/eventshandler.h \
    Core/events/eventshandler_interfaces.h \
    Core/provider/config/defaultconfigprovider.h \
    Core/provider/provider.h \
    Core/provider/providerholder.h \
    Core/service/service.h \
    Core/service/service_cluster.h \
    Core/config.h \
    Core/configurable.h \
    Core/enginecore.h \
    Core/enginecoreincludes.h \
    Core/enginedatastructures.h \
    Core/engineinitializer.h \
    Core/engineinterface.h \
    Core/executable.h \
    Core/meta.h \
    Core/threadpool.h \
    Core/worker.h \
    Core/workerqueueproxy.h \
    Graphics/glprogram.h \
    Graphics/glshader.h \
    Graphics/gluniform.h \
    Graphics/graphicsapi.h \
    Graphics/graphicsclasses.h \
    Graphics/openglsdl.h \
    Graphics/renderdefs.h \
    Graphics/sdlclasses.h \
    Graphics/uniform.h \
    Helpers/action.h \
    Helpers/dataobject.h \
    Helpers/executor.h \
    Helpers/function_traits.h \
    Helpers/generic_invokable.h \
    Helpers/genericcondition.h \
    Helpers/helpers.h \
    Helpers/invokable.h \
    Helpers/invokablepack.h \
    Helpers/repeatedpromise.h \
    Helpers/repeteadfuture.h \
    Helpers/sharedstate.h \
    Helpers/size.h \
    Helpers/task.h \
    Input/inputapi.h \
    Input/inputdata.h \
    Input/inputevents.h \
    Input/sdlinput.h \
    Logger/logger.h \
    Math/mathconstants.h \
    Math/mathdefines.h \
    Math/mathhelpers.h \
    Modules/graphicsmodule.h \
    Modules/inputmodule.h \
    Modules/module.h \
    Modules/moduleinterface.h \
    Modules/resourcesmodule.h \
    Modules/soundmodule.h \
    Public/apis.h \
    Public/coreinterface.h \
    Public/meshreader3ds.h \
    Public/shaderreader.h \
    Public/ui.h \
    Resources/filereader.h \
    Resources/resourcesapi.h \
    Tests/tests.h \
    Ui/basicshapes.h \
    Ui/basicuielements.h \
    Ui/elementinterface.h \
    Ui/uielements.h \
    Ui/uilayer.h \
    Core/provider/config/configprovider.h \
    Core/configresolver.h \
    Public/mtlreader.h \
    Graphics/material.h \
    Public/meshreaderobj.h \
    Graphics/glhelpers.h \
    Helpers/invokationresult.h \
    Framework/world.h \
    Framework/camera.h \
    Framework/worldobject.h \
    Framework/drawableworldobject.h \
    Graphics/texture.h \
    Graphics/renderstep.h \
    Graphics/rendertarget.h \
    Graphics/renderpath.h \
    Graphics/glrenderer.h \
    Graphics/renderer.h \
    Graphics/rendering_attributtes.h \
    Platform/OpenGL/glbindings.h \
    Platform/OpenGL/gldebug.h

SOURCES += \
    Core/events/eventshandler.cpp \
    Core/provider/config/defaultconfigprovider.cpp \
    Core/service/service.cpp \
    Core/service/service_cluster.cpp \
    Core/config.cpp \
    Core/enginecore.cpp \
    Core/meta.cpp \
    Core/threadpool.cpp \
    Core/worker.cpp \
    Core/workerqueueproxy.cpp \
    Graphics/glprogram.cpp \
    Graphics/glshader.cpp \
    Graphics/openglsdl.cpp \
    Graphics/renderdefs.cpp \
    Graphics/sdlclasses.cpp \
    Graphics/vertexobject.cpp \
    Helpers/action.cpp \
    Helpers/generic_invokable.cpp \
    Helpers/helpers.cpp \
    Helpers/invokablepack.cpp \
    Helpers/repeteadfuture.cpp \
    Helpers/task.cpp \
    Input/sdlinput.cpp \
    Logger/logger.cpp \
    Math/mathhelpers.cpp \
    Modules/graphicsmodule.cpp \
    Modules/inputmodule.cpp \
    Modules/module.cpp \
    Modules/resourcesmodule.cpp \
    Modules/soundmodule.cpp \
    Public/coreinterface.cpp \
    Public/meshreader3ds.cpp \
    Public/shaderreader.cpp \
    Resources/filereader.cpp \
    Tests/tests.cpp \
    Ui/basicshapes.cpp \
    Ui/basicuielements.cpp \
    Ui/uielements.cpp \
    Ui/uilayer.cpp \
    main.cpp \
    Core/configresolver.cpp \
    Core/provider/provider.cpp \
    Core/provider/providerholder.cpp \
    Public/mtlreader.cpp \
    Graphics/material.cpp \
    Public/meshreaderobj.cpp \
    Graphics/glhelpers.cpp \
    Graphics/gluniform.cpp \
    Framework/world.cpp \
    Framework/camera.cpp \
    Framework/worldobject.cpp \
    Framework/drawableworldobject.cpp \
    Graphics/graphicsclasses.cpp \
    Graphics/glrenderer.cpp \
    Platform/OpenGL/glbindings.cpp \
    Platform/OpenGL/gldebug.cpp

DISTFILES += \
    Shaders/vertexshadertex.glsl \
    Shaders/fragmentshadertex.glsl \
    Shaders/vertexshadertex_copy.glsl \
    Shaders/fragmentshadertex_copy.glsl
