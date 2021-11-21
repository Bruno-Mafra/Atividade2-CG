#ifndef LISTRA_HPP_
#define LISTRA_HPP_

#include "abcg.hpp"

class Listra {
 public:
  void initializeGL(GLuint program);
  void paintGL();
  void terminateGL();

 private:
  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_program{};

  GLint m_modelMatrixLoc{};
  GLint m_colorLoc{};
};

#endif