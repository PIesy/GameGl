cmake_minimum_required(VERSION 3.6)
project(GameGl)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
    Core/apibase.h
    Core/events/eventshandler.cpp
    Core/events/eventshandler.h
    Core/events/eventshandler_interfaces.h
    Core/service/service.cpp
    Core/service/service.h
    Core/service/service_cluster.cpp
    Core/service/service_cluster.h
    Core/config.cpp
    Core/config.h
    Core/enginecore.cpp
    Core/enginecore.h
    Core/enginecoreincludes.h
    Core/enginedatastructures.h
    Core/engineinitializer.h
    Core/engineinterface.h
    Core/meta.cpp
    Core/meta.h
    Core/threadpool.cpp
    Core/threadpool.h
    Core/worker.cpp
    Core/worker.h
    Framework/Loaders/meshloader.cpp
    Framework/Loaders/meshloader.h
    Framework/camera.cpp
    Framework/camera.h
    Framework/drawableworldobject.cpp
    Framework/drawableworldobject.h
    Framework/Rendering/scenebuilder.cpp
    Framework/Rendering/scenebuilder.h
    Framework/world.cpp
    Framework/world.h
    Framework/worldobject.cpp
    Framework/worldobject.h
    Platform/Graphics/OpenGL/glrenderer.cpp
    Platform/Graphics/OpenGL/glrenderer.h
    Graphics/graphicsapi.h
    Graphics/graphicsclasses.cpp
    Graphics/graphicsclasses.h
    Platform/Graphics/OpenGL/openglsdl.cpp
    Platform/Graphics/OpenGL/openglsdl.h
    Graphics/renderer.h
    Graphics/rendering_attributtes.h
    Graphics/renderpath.h
    Graphics/renderstep.h
    Graphics/rendertarget.h
    Graphics/scene.h
    Platform/Graphics/OpenGL/sdlclasses.cpp
    Platform/Graphics/OpenGL/sdlclasses.h
    Graphics/texture.h
    Graphics/uniform.h
    Helpers/action.cpp
    Helpers/action.h
    Core/executor.h
    Helpers/function_traits.h
    Helpers/generic_invokable.cpp
    Helpers/generic_invokable.h
    Helpers/genericcondition.h
    Helpers/helpers.h
    Core/invokable.h
    Helpers/invokablepack.cpp
    Helpers/invokablepack.h
    Helpers/invokationresult.h
    Helpers/sharedstate.h
    Helpers/size.h
    Helpers/task.cpp
    Helpers/task.h
    Input/inputapi.h
    Input/inputdata.h
    Input/inputevents.h
    Platform/Input/SDL2/sdlinput.cpp
    Platform/Input/SDL2/sdlinput.h
    Logger/logger.cpp
    Logger/logger.h
    Math/mathconstants.h
    Math/mathdefines.h
    Math/mathhelpers.cpp
    Math/mathhelpers.h
    Modules/module.cpp
    Modules/module.h
    Modules/moduleinterface.h
    Platform/Graphics/OpenGL/Wrappers/glbindings.cpp
    Platform/Graphics/OpenGL/Wrappers/glbindings.h
    Platform/Graphics/OpenGL/Wrappers/gldebug.cpp
    Platform/Graphics/OpenGL/Wrappers/gldebug.h
    Platform/Graphics/OpenGL/Wrappers/gltexture.cpp
    Platform/Graphics/OpenGL/Wrappers/gltexture.h
    Public/apis.h
    Public/coreinterface.cpp
    Public/coreinterface.h
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
    main.cpp Platform/Graphics/OpenGL/Wrappers/glbuffer.cpp Platform/Graphics/OpenGL/Wrappers/glbuffer.h Platform/Graphics/OpenGL/Wrappers/glvertexarray.cpp Platform/Graphics/OpenGL/Wrappers/glvertexarray.h Platform/Graphics/OpenGL/Wrappers/glframebuffer.cpp Platform/Graphics/OpenGL/Wrappers/glframebuffer.h Platform/Graphics/OpenGL/Wrappers/globject.cpp Platform/Graphics/OpenGL/Wrappers/globject.h Platform/Graphics/OpenGL/graphics_context_data.h Platform/Graphics/OpenGL/graphics_context_data.cpp Platform/Graphics/OpenGL/Wrappers/glrenderbuffer.cpp Platform/Graphics/OpenGL/Wrappers/glrenderbuffer.h Helpers/shared_thread_local_wrapper.h Helpers/storagepointer.h Storage/storageapi.h Storage/storagedescriptor.h Platform/Storage/Simple/simple_storage.cpp Platform/Storage/Simple/simple_storage.h Platform/Storage/Simple/storageservice.cpp Platform/Storage/Simple/storageservice.h Framework/Loaders/textureloader.cpp Framework/Loaders/textureloader.h Helpers/storagepointer.cpp Graphics/mesh.cpp Graphics/mesh.h Graphics/meshdescriptor.h Platform/Graphics/OpenGL/gltexturefactory.cpp Platform/Graphics/OpenGL/gltexturefactory.h Platform/Graphics/OpenGL/glcontext.cpp Platform/Graphics/OpenGL/glcontext.h Platform/Graphics/OpenGL/glmeshobject.h Platform/Graphics/OpenGL/glframebuffercontainer.h Framework/Loaders/programloader.cpp Framework/Loaders/programloader.h Framework/object_hints.h Graphics/framebufferproperties.h Framework/light.cpp Framework/light.h Framework/texturebuilder.cpp Framework/texturebuilder.h resources_helper.h resources_helper.cpp Framework/object_hints.cpp custom_hints.h custom_hints.cpp Framework/material.h step_builder.cpp step_builder.h Framework/Rendering/RenderStepWrapper.h Core/module_provider.h Platform/Graphics/OpenGL/opengl_module_provider.cpp Platform/Graphics/OpenGL/opengl_module_provider.h Platform/Storage/Simple/simple_storage_provider.cpp Platform/Storage/Simple/simple_storage_provider.h Platform/Input/SDL2/sdl2_input_provider.cpp Platform/Input/SDL2/sdl2_input_provider.h Helpers/SharedTaskList.cpp Helpers/SharedTaskList.h Graphics/graphics_events.h Platform/Graphics/OpenGL/Wrappers/glprogram.cpp Platform/Graphics/OpenGL/Wrappers/glprogram.h Platform/Graphics/OpenGL/Wrappers/glshader.cpp Platform/Graphics/OpenGL/Wrappers/glshader.h Platform/Graphics/OpenGL/Wrappers/glprogrampipeline.cpp Platform/Graphics/OpenGL/Wrappers/glprogrampipeline.h Helpers/synchronized_map.h Helpers/lock_holder.h Platform/Graphics/OpenGL/gl_program_handle.cpp Platform/Graphics/OpenGL/gl_program_handle.h Platform/Graphics/OpenGL/gl_generic.cpp Platform/Graphics/OpenGL/gl_generic.h Platform/Graphics/OpenGL/gl_program_proxy.cpp Platform/Graphics/OpenGL/gl_program_proxy.h Platform/Graphics/OpenGL/Wrappers/glfence.cpp Platform/Graphics/OpenGL/Wrappers/glfence.h)

add_executable(GameGl ${SOURCE_FILES})

target_link_libraries(GameGl PRIVATE -lGL)
target_link_libraries(GameGl PRIVATE -lGLEW)
target_link_libraries(GameGl PRIVATE -lSDL2)
target_link_libraries(GameGl PRIVATE -lGLU)
target_link_libraries(GameGl PRIVATE -lpthread)
target_link_libraries(GameGl PRIVATE -lassimp)

add_custom_target(COPY_RESOURCES ALL)

add_custom_command(TARGET GameGl COPY_RESOURCES
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Shaders $<TARGET_FILE_DIR:GameGl>)
add_custom_command(TARGET GameGl COPY_RESOURCES
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Data $<TARGET_FILE_DIR:GameGl>)