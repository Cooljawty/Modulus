add_executable(run main.cpp)

target_include_directories(run PUBLIC ${Modulus_SOURCE_DIR}/include ${FREETYPE_INCLUDE_DIR_ft2build})

target_link_libraries(run modulus ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${FREETYPE_LIBRARIES} ${ASSIMP_LIBRARIES})

add_custom_command(TARGET run POST_BUILD
				   COMMAND ${CMAKE_COMMAND} -E copy_directory
				   ${CMAKE_SOURCE_DIR}/shaders/ $<TARGET_FILE_DIR:run>/shaders
					DEPENDS "${CMAKE_SOURCE_DIR}/shaders/")

add_custom_command(TARGET run POST_BUILD
				   COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:run>/assets
					DEPENDS "${CMAKE_SOURCE_DIR}/assets/")
