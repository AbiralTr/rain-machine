add_executable(sandbox_game src/main.cpp)
target_link_libraries(sandbox_game PRIVATE engine)

add_custom_command(TARGET sandbox_game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:sandbox_game>/assets
)
