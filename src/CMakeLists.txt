set(SOURCES "modulus.cpp;shader.cpp;VertexArray.cpp;texture.cpp;physics.cpp;transform.cpp;input.cpp;modulus_time.cpp;model.cpp;framebuffer.cpp;text.cpp")
set(SHADER_SOURCES "PolygonShader.cpp;TextureShader.cpp;SpriteShader.cpp;TextShader.cpp;LampShader.cpp;FBOShader.cpp;LineShader.cpp")

file(GLOB HEADERS CONFIGURE_DEPENDS "${Modulus_SOURCE_DIR}/include/*.h")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/obj)

add_library(modulus ${SOURCES} ${SHADER_SOURCES} ${HEADERS})

target_include_directories(modulus PUBLIC ${Modulus_SOURCE_DIR}/include ${GLM_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIR_ft2build} ${assimp_INCLUDE_DIRS} ${LUA_INCLUDE_DIRS})

target_link_libraries(modulus SDL2::Main SDL2::Image GLEW::GLEW ${OPENGL_LIBRARIES} ${FREETYPE_LIBRARIES} ${assimp_LIBRARIES} ${LUA_LIBRARIES})

set_target_properties(modulus PROPERTIES POSITION_INDEPENDENT_CODE True)

source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADERS})
