#include "soldado.hpp"

#include <fmt/core.h>
#include <tiny_obj_loader.h>

#include <cppitertools/itertools.hpp>
#include <glm/gtx/hash.hpp>
#include <unordered_map>

// Explicit specialization of std::hash for Vertex
namespace std {
template <>
struct hash<Vertex4> {
  size_t operator()(Vertex4 const& vertex) const noexcept {
    const std::size_t h1{std::hash<glm::vec3>()(vertex.position)};
    return h1;
  }
};
}  // namespace std

void Soldado::createBuffers(GLuint program) {
  m_program = program;

  // Delete previous buffers
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteBuffers(1, &m_VBO);

  // VBO
  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices[0]) * m_vertices.size(),
                     m_vertices.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // EBO
  abcg::glGenBuffers(1, &m_EBO);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(m_indices[0]) * m_indices.size(), m_indices.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Create VAO
  glGenVertexArrays(1, &m_VAO);

  // Bind vertex attributes to current VAO
  glBindVertexArray(m_VAO);

  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  GLint positionAttribute{glGetAttribLocation(m_program, "inPosition")};
  glEnableVertexAttribArray(positionAttribute);
  glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE,
                        sizeof(Vertex4), nullptr);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

  // End of binding to current VAO
  glBindVertexArray(0);
}

void Soldado::loadObj(std::string_view path, bool standardize) {
  tinyobj::ObjReaderConfig readerConfig;
  readerConfig.mtl_search_path = "";  // Path to material files

  tinyobj::ObjReader reader;

  if (!reader.ParseFromFile(path.data(), readerConfig)) {
    if (!reader.Error().empty()) {
      throw abcg::Exception{abcg::Exception::Runtime(
          fmt::format("Failed to load model {} ({})", path, reader.Error()))};
    }
    throw abcg::Exception{
        abcg::Exception::Runtime(fmt::format("Failed to load model {}", path))};
  }

  if (!reader.Warning().empty()) {
    fmt::print("Warning: {}\n", reader.Warning());
  }

  const auto& attrib{reader.GetAttrib()};
  const auto& shapes{reader.GetShapes()};

  m_vertices.clear();
  m_indices.clear();

  // A key:value map with key=Vertex and value=index
  std::unordered_map<Vertex4, GLuint> hash{};

  // Loop over shapes
  for (const auto& shape : shapes) {
    // Loop over faces(polygon)
    size_t indexOffset{0};
    for (const auto faceNumber :
         iter::range(shape.mesh.num_face_vertices.size())) {
      // Number of vertices composing face f
      std::size_t numFaceVertices{shape.mesh.num_face_vertices[faceNumber]};
      // Loop over vertices in the face
      std::size_t startIndex{};
      for (const auto vertexNumber : iter::range(numFaceVertices)) {
        // Access to vertex
        tinyobj::index_t index{shape.mesh.indices[indexOffset + vertexNumber]};

        // Vertex coordinates
        startIndex = 3 * index.vertex_index;
        tinyobj::real_t vx = attrib.vertices[startIndex + 0];
        tinyobj::real_t vy = attrib.vertices[startIndex + 1];
        tinyobj::real_t vz = attrib.vertices[startIndex + 2];

        Vertex4 vertex{};
        vertex.position = {vx, vy, vz};

        // If uniqueVertices doesn't contain this vertex
        if (hash.count(vertex) == 0) {
          // Add this index (size of m_vertices)
          hash[vertex] = m_vertices.size();
          // Add this vertex
          m_vertices.push_back(vertex);
        }

        m_indices.push_back(hash[vertex]);
      }
      indexOffset += numFaceVertices;
    }
  }

  if (standardize) {
    this->standardize();
  }
}

void Soldado::render(Camera m_camera) {
  glUseProgram(m_program);
  glBindVertexArray(m_VAO);

  m_camera.init(m_program);

  GLint modelMatrixLoc{glGetUniformLocation(m_program, "modelMatrix")};
  GLint colorLoc{glGetUniformLocation(m_program, "color")};

  glm::mat4 model{1.0f};
  model = glm::translate(model, glm::vec3(-1.5f, 0.0f, -2.0f)); // reposicionar o centro
  //model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 1, 0)); // angulo de rotação e em torno de qual eixo
  model = glm::scale(model, glm::vec3(0.01f)); // destorcer objeto nas coordenadas x,y,z
  glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  glUniform4f(colorLoc, color[0], color[1], color[2], 1.0f);
  glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);

  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(1.5f, 0.0f, -2.0f)); // reposicionar o centro
  //model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 1, 0)); // angulo de rotação e em torno de qual eixo
  model = glm::scale(model, glm::vec3(0.01f)); // destorcer objeto nas coordenadas x,y,z
  glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  glUniform4f(colorLoc, color[0], color[1], color[2], 1.0f);
  glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);

  glBindVertexArray(0);
  glUseProgram(0);
}

void Soldado::standardize() {
  // Center to origin and normalize largest bound to [-1, 1]

  // Get bounds
  glm::vec3 max(std::numeric_limits<float>::lowest());
  glm::vec3 min(std::numeric_limits<float>::max());
  for (const auto& vertex : m_vertices) {
    max.x = std::max(max.x, vertex.position.x);
    max.y = std::max(max.y, vertex.position.y);
    max.z = std::max(max.z, vertex.position.z);
    min.x = std::min(min.x, vertex.position.x);
    min.y = std::min(min.y, vertex.position.y);
    min.z = std::min(min.z, vertex.position.z);
  }

  // Center and scale
  const auto center{(min + max) / 2.0f};
  const auto scaling{2.0f / glm::length(max - min)};
  for (auto& vertex : m_vertices) {
    vertex.position = (vertex.position - center) * scaling;
  }
}

void Soldado::terminateGL() {
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}