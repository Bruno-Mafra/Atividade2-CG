#ifndef ARVORE_HPP_
#define ARVORE_HPP_

#include <vector>

#include "abcg.hpp"
#include "camera.hpp"

struct Vertex5 {
  glm::vec3 position{};

  bool operator==(const Vertex5& other) const noexcept {
    return position == other.position;
  }
};

class Arvore {
 public:
  void createBuffers(GLuint program);
  void loadObj(std::string_view path, bool standardize = true);
  void render(Camera m_camera);
  void terminateGL();

 private:
  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};
  GLuint m_program{};

  glm::vec4 color { 101/255.0f, 109/255.0f, 85/255.0f, 1.0f};

  std::vector<Vertex5> m_vertices;
  std::vector<GLuint> m_indices;

  void standardize();
};

#endif