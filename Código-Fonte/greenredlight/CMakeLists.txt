project(greenredlight2)
add_executable(${PROJECT_NAME} camera.cpp ground.cpp main.cpp openglwindow.cpp
          boneca.cpp paredes.cpp soldado.cpp arvore.cpp listra.cpp bonecos.cpp)
enable_abcg(${PROJECT_NAME})
