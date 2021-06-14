set(SOURCES "modulus;shader;VertexArray;texture;physics;transform;input;modulus_time;mesh;model;framebuffer")
set(SHADER_SOURCES "PolygonShader;TextureShader;SpriteShader;TextShader;LampShader;FBOShader;LineShader")

file(GLOB HEADERS CONFIGURE_DEPENDS "${Modulus_SOURCE_DIR}/include/*.h")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/obj)

add_library(modulus ${SOURCES} ${SHADER_SOURCES} ${HEADERS})

target_include_directories(modulus PUBLIC ${Modulus_SOURCE_DIR}/include ${FREETYPE_INCLUDE_DIR} ${ASSIMP_INCLUDE_DIR})

target_link_libraries(modulus SDL2::Main SDL2::Image ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${FREETYPE_LIBRARIES} ${ASSIMP_LIBRARIES})

set_target_properties(modulus PROPERTIES POSITION_INDEPENDENT_CODE True)

source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADERS})
