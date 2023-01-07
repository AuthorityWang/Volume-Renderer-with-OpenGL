#include <mesh.h>
#include <utils.h>
#include <fstream>
#include <vector>
#include <sstream>

Mesh::Mesh(const std::string &path) { loadDataFromFile(getPath(path)); }

/**
 * TODO: implement load mesh data from file here
 * you need to open file with [path]
 *
 * File format
 *
 *
 * For each line starting with 'v' contains 3 floats, which
 * represents the position of a vertex
 *
 * For each line starting with 'n' contains 3 floats, which
 * represents the normal of a vertex
 *
 * For each line starting with 'f' contains 6 integers
 * [v0, n0, v1, n1, v2, n2], which represents the triangle face
 * v0, n0 means the vertex index and normal index of the first vertex
 * v1, n1 means the vertex index and normal index of the second vertex
 * v2, n2 means the vertex index and normal index of the third vertex
 */
void Mesh::loadDataFromFile(const std::string &path) {
    /**
     * path stands for the where the object is storaged
     * */
  std::ifstream infile(path);
  std::string line;
  std::vector<vec3> temp;
  int index = 0;
  while (!infile.eof()) {
    getline(infile, line);
    if (line[0] == 'v') {
      std::stringstream ss(line.erase(0, 1));
      float x, y, z;
      ss >> x >> y >> z;
      Vertex vertex;
      vertex.position = vec3(x, y, z);
      vertex.normal = vec3(0, 0, 0);
      vertices.push_back(vertex);
    }
    else if (line[0] == 'n') {
      std::stringstream ss(line.erase(0, 1));
      float x, y, z;
      ss >> x >> y >> z;
      temp.push_back(vec3(x, y, z));
    }
    else if (line[0] == 'f') {
      std::stringstream ss(line.erase(0, 1));
      int x, y;
      for (int i = 0; i<3; i++) {
        ss >> x >> y;
        vertices[x].normal = temp[y];
        indices.push_back(x);
      }
    }
  }
  infile.close();
}

/**
 * TODO: implement your draw code here
 */
void Mesh::draw() const {
    // define three var for future binding
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glBindVertexArray(0);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}