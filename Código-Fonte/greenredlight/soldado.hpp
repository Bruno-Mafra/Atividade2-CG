#ifndef SOLDADO_HPP_
#define SOLDADO_HPP_

#include <vector>

#include "abcg.hpp"
#include "camera.hpp"

struct Vertex4 {
  glm::vec3 position{};

  bool operator==(const Vertex4& other) const noexcept {
    return position == other.position;
  }
};

class Soldado {
 public:
  void createBuffers(GLuint program);
  void loadObj(std::string_view path, bool standardize = true);
  void render(Camera m_camera);
  //void setupVAO(GLuint program);
  void terminateGL();

  //[[nodiscard]] int getNumTriangles() const {
  //  return static_cast<int>(m_indices.size()) / 3;
  //}

 private:
  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};
  GLuint m_program{};

  glm::vec4 color { 202/255.0f, 58.0/255.0f, 83.0/255.0f, 1.0f};

  std::vector<Vertex4> m_vertices;
  std::vector<GLuint> m_indices;

  void standardize();
};

#endif