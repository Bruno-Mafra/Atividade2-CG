#ifndef GROUND_HPP_
#define GROUND_HPP_

#include <vector>

#include "abcg.hpp"
#include "camera.hpp"

struct Vertex {
  glm::vec3 position{};

  bool operator==(const Vertex& other) const noexcept {
    return position == other.position;
  }
};

class Ground {
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

  glm::vec4 color { (244.0/255.0)*2.0f, (164.0/255.0)*2.0f, (96.0/255.0)*2.0f, 1.0f}; // *2 pq essa é a cor de trás dos triangulos

  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;

  void standardize();
};

#endif