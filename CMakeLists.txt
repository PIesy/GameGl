cmake_minimum_required(VERSION 3.6)
project(GameGl)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        Api/apibase.h
        Core/events/eventshandler.cpp
        Core/events/eventshandler.h
        Core/events/eventshandler_interfaces.h
        Core/provider/config/configprovider.h
        Core/provider/config/defaultconfigprovider.cpp
        Core/provider/config/defaultconfigprovider.h
        Core/provider/provider.cpp
        Core/provider/provider.h
        Core/provider/providerholder.cpp
        Core/provider/providerholder.h
        Core/service/service.cpp
        Core/service/service.h
        Core/service/service_cluster.cpp
        Core/service/service_cluster.h
        Core/config.cpp
        Core/config.h
        Core/configresolver.cpp
        Core/configresolver.h
        Core/configurable.h
        Core/enginecore.cpp
        Core/enginecore.h
        Core/enginecoreincludes.h
        Core/enginedatastructures.h
        Core/engineinitializer.h
        Core/engineinterface.h
        Core/executable.h
        Core/meta.cpp
        Core/meta.h
        Core/threadpool.cpp
        Core/threadpool.h
        Core/worker.cpp
        Core/worker.h
        Core/workerqueueproxy.cpp
        Core/workerqueueproxy.h
        Framework/Loaders/loader.cpp
        Framework/Loaders/loader.h
        Framework/Loaders/meshloader.cpp
        Framework/Loaders/meshloader.h
        Framework/camera.cpp
        Framework/camera.h
        Framework/drawableworldobject.cpp
        Framework/drawableworldobject.h
        Framework/scenebuilder.cpp
        Framework/scenebuilder.h
        Framework/world.cpp
        Framework/world.h
        Framework/worldobject.cpp
        Framework/worldobject.h
        Graphics/glhelpers.cpp
        Graphics/glhelpers.h
        Graphics/glprogram.cpp
        Graphics/glprogram.h
        Graphics/glrenderer.cpp
        Graphics/glrenderer.h
        Graphics/glshader.cpp
        Graphics/glshader.h
        Graphics/gluniform.cpp
        Graphics/gluniform.h
        Graphics/graphicsapi.h
        Graphics/graphicsclasses.cpp
        Graphics/graphicsclasses.h
        Graphics/material.cpp
        Graphics/material.h
        Graphics/openglsdl.cpp
        Graphics/openglsdl.h
        Graphics/renderdefs.cpp
        Graphics/renderdefs.h
        Graphics/renderer.h
        Graphics/rendering_attributtes.h
        Graphics/renderpath.h
        Graphics/renderstep.h
        Graphics/rendertarget.h
        Graphics/scene.h
        Graphics/sdlclasses.cpp
        Graphics/sdlclasses.h
        Graphics/texture.h
        Graphics/uniform.h
        Graphics/vertexobject.cpp
        Helpers/action.cpp
        Helpers/action.h
        Helpers/dataobject.h
        Helpers/executor.h
        Helpers/function_traits.h
        Helpers/generic_invokable.cpp
        Helpers/generic_invokable.h
        Helpers/genericcondition.h
        Helpers/helpers.cpp
        Helpers/helpers.h
        Helpers/invokable.h
        Helpers/invokablepack.cpp
        Helpers/invokablepack.h
        Helpers/invokationresult.h
        Helpers/repeatedpromise.h
        Helpers/repeteadfuture.cpp
        Helpers/repeteadfuture.h
        Helpers/sharedstate.h
        Helpers/size.h
        Helpers/task.cpp
        Helpers/task.h
        Input/inputapi.h
        Input/inputdata.h
        Input/inputevents.h
        Input/sdlinput.cpp
        Input/sdlinput.h
        Logger/logger.cpp
        Logger/logger.h
        Math/mathconstants.h
        Math/mathdefines.h
        Math/mathhelpers.cpp
        Math/mathhelpers.h
        Modules/graphicsmodule.cpp
        Modules/graphicsmodule.h
        Modules/inputmodule.cpp
        Modules/inputmodule.h
        Modules/module.cpp
        Modules/module.h
        Modules/moduleinterface.h
        Modules/resourcesmodule.cpp
        Modules/resourcesmodule.h
        Modules/soundmodule.cpp
        Modules/soundmodule.h
        Platform/OpenGL/glbindings.cpp
        Platform/OpenGL/glbindings.h
        Platform/OpenGL/gldebug.cpp
        Platform/OpenGL/gldebug.h
        Platform/OpenGL/gltexture.cpp
        Platform/OpenGL/gltexture.h
        Public/apis.h
        Public/coreinterface.cpp
        Public/coreinterface.h
        Public/meshreader3ds.cpp
        Public/meshreader3ds.h
        Public/meshreaderobj.cpp
        Public/meshreaderobj.h
        Public/mtlreader.cpp
        Public/mtlreader.h
        Public/shaderreader.cpp
        Public/shaderreader.h
        Public/ui.h
        Resources/filereader.cpp
        Resources/filereader.h
        Resources/resourcesapi.h
        Tests/tests.cpp
        Tests/tests.h
        Ui/basicshapes.cpp
        Ui/basicshapes.h
        Ui/basicuielements.cpp
        Ui/basicuielements.h
        Ui/elementinterface.h
        Ui/uielements.cpp
        Ui/uielements.h
        Ui/uilayer.cpp
        Ui/uilayer.h
        main.cpp Platform/OpenGL/glbuffer.cpp Platform/OpenGL/glbuffer.h Platform/OpenGL/glvertexarray.cpp Platform/OpenGL/glvertexarray.h Platform/OpenGL/glframebuffer.cpp Platform/OpenGL/glframebuffer.h Platform/OpenGL/globjectmanager.cpp Platform/OpenGL/globjectmanager.h Platform/OpenGL/globject.cpp Platform/OpenGL/globject.h)

add_executable(GameGl ${SOURCE_FILES})

target_link_libraries(GameGl PRIVATE -lGL)
target_link_libraries(GameGl PRIVATE -lGLEW)
target_link_libraries(GameGl PRIVATE -lSDL2)
target_link_libraries(GameGl PRIVATE -lGLU)
target_link_libraries(GameGl PRIVATE -lpthread)
target_link_libraries(GameGl PRIVATE -lassimp)

add_custom_command(TARGET GameGl POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Shaders $<TARGET_FILE_DIR:GameGl>)
add_custom_command(TARGET GameGl POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Data $<TARGET_FILE_DIR:GameGl>)