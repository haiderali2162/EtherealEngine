file(GLOB_RECURSE libsrc *.h *.cpp *.hpp *.c *.cc)

add_executable (editor ${libsrc})

target_link_libraries(editor PUBLIC editor_core)
target_link_libraries(editor PUBLIC runtime)

target_include_directories (editor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

if(MINGW)
	set_target_properties(editor PROPERTIES LINK_FLAGS "-static-libgcc -static-libstdc++ -static")
endif()
