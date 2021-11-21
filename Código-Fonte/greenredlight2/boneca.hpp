#ifndef BONECA_HPP_
#define BONECA_HPP_

#include <vector>
#include <random>

#include "abcg.hpp"
#include "camera.hpp"
#include "gamedata.hpp"

struct Vertex3 {
  glm::vec3 position{};

  bool operator==(const Vertex3& other) const noexcept {
    return position == other.position;
  }
};

class OpenGLWindow;

class Boneca {
 public:
  void createBuffers(GLuint program);
  void loadObj(std::string_view path, bool standardize = true);
  void render(Camera m_camera);
  void update(const GameData &gameData, float deltaTime);
  void terminateGL();

 private:
  friend OpenGLWindow;

  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};
  GLuint m_program{};

  float m_angle = glm::radians(0.0f);

  glm::vec4 color { 225/255.0f, 125/255.0f, 15/255.0f, 1.0f};

  abcg::ElapsedTimer m_segundosTimer;
  abcg::ElapsedTimer m_deFrenteTimer;

  bool changeTimer = true;
  bool girarPraTras = false;
  bool girarPraFrente = false;
  bool greenLight = false;
  bool timerFoiResetado = false;

  int timer;

  std::vector<Vertex3> m_vertices;
  std::vector<GLuint> m_indices;

  void standardize();
};

#endif