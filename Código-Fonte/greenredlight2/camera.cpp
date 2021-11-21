#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

void Camera::init(GLuint m_program) {
  GLint viewMatrixLoc{glGetUniformLocation(m_program, "viewMatrix")};
  GLint projMatrixLoc{glGetUniformLocation(m_program, "projMatrix")};

  // Set uniform variables for viewMatrix and projMatrix
  // These matrices are used for every scene object
  glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &m_viewMatrix[0][0]);
  glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_projMatrix[0][0]);
}

void Camera::computeProjectionMatrix(int width, int height) {
  m_projMatrix = glm::mat4(1.0f);
  const auto aspect{static_cast<float>(width) / static_cast<float>(height)};
  m_projMatrix = glm::perspective(glm::radians(70.0f), aspect, 0.1f, 100.0f);
}

void Camera::computeViewMatrix() {
  m_viewMatrix = glm::lookAt(m_eye, m_at, m_up);
}

void Camera::dolly(float speed, const GameData &gameData) {
  if(gameData.m_state == State::Playing || gameData.m_state == State::Win) {
    // Compute forward vector (view direction)
    const glm::vec3 forward{glm::normalize(m_at - m_eye)};

    glm::vec3 old_m_at = m_at;

    // Move eye and center forward (speed > 0) or backward (speed < 0)
    m_eye += forward * speed;
    m_at += forward * speed;

    if(m_eye.x > 2.5f) { // colisão com as paredes
      m_eye.x = 2.5f;
      m_at = old_m_at;
    }

    if( m_eye.x < -2.5f) {
      m_eye.x = -2.5f;
      m_at = old_m_at;
    }

    if(m_eye.z > 4.5f) {
      m_eye.z = 4.5f;
      m_at = old_m_at;
    }

    if(m_eye.z < -4.5f) {
      m_eye.z = -4.5f;
      m_at = old_m_at;
    }

    computeViewMatrix();
  }
}

void Camera::truck(float speed, const GameData &gameData) {
  if(gameData.m_state == State::Playing || gameData.m_state == State::Win) {
    // Compute forward vector (view direction)
    const glm::vec3 forward{glm::normalize(m_at - m_eye)};
    // Compute vector to the left
    const glm::vec3 left{glm::cross(m_up, forward)};

    glm::vec3 old_m_at = m_at;

    // Move eye and center to the left (speed < 0) or to the right (speed > 0)
    m_at -= left * speed;
    m_eye -= left * speed;

    if(m_eye.x > 2.5f) { // colisão com as paredes
      m_eye.x = 2.5f;
      m_at = old_m_at;
    }

    if( m_eye.x < -2.5f) {
      m_eye.x = -2.5f;
      m_at = old_m_at;
    }

    if(m_eye.z > 4.5f) {
      m_eye.z = 4.5f;
      m_at = old_m_at;
    }

    if(m_eye.z < -4.5f) {
      m_eye.z = -4.5f;
      m_at = old_m_at;
    }

    computeViewMatrix();
  }
}

void Camera::pan(float speed) {
  glm::mat4 transform{glm::mat4(1.0f)};

  // Rotate camera around its local y axis
  transform = glm::translate(transform, m_eye);
  transform = glm::rotate(transform, -speed, m_up);
  transform = glm::translate(transform, -m_eye);

  m_at = transform * glm::vec4(m_at, 1.0f);

  computeViewMatrix();
}