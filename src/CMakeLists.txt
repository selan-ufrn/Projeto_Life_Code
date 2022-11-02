#=== Main App ===
# include_directories(${CMAKE_SOURCE_DIR}/lib)
add_executable( ${APP_NAME} main.cpp life.cpp  )
target_include_directories( ${APP_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/lib )
target_compile_features( ${APP_NAME}  PUBLIC cxx_std_14 )
target_link_libraries( ${APP_NAME} PRIVATE ${CANVAS_LIB} ${TIP_LIB} ${LODEPNG_LIB})
