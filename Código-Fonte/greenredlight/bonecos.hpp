#ifndef BONECOS_HPP_
#define BONECOS_HPP_

#include <vector>

#include "abcg.hpp"
#include "camera.hpp"

struct Vertex6 {
  glm::vec3 position{};

  bool operator==(const Vertex6& other) const noexcept {
    return position == other.position;
  }
};

class Bonecos {
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

  glm::vec4 color { 10/255.0f, 130/255.0f, 118/255.0f, 1.0f};

  std::vector<Vertex6> m_vertices;
  std::vector<GLuint> m_indices;

  void standardize();
};

#endif