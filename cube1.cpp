#include <chrono>
#include <cmath>
#include <fstream>
#include <locale.h>
#include <ncurses.h>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

struct Vec3 {
  float x, y, z;
  Vec3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

  Vec3 operator+(const Vec3 &v) const {
    return Vec3(x + v.x, y + v.y, z + v.z);
  }
  Vec3 operator-(const Vec3 &v) const {
    return Vec3(x - v.x, y - v.y, z - v.z);
  }
  Vec3 operator*(float f) const { return Vec3(x * f, y * f, z * f); }
};

struct Vec4 {
  float x, y, z, w;
  Vec4(float x = 0, float y = 0, float z = 0, float w = 1)
      : x(x), y(y), z(z), w(w) {}
  Vec4(const Vec3 &v, float w = 1) : x(v.x), y(v.y), z(v.z), w(w) {}
  Vec3 toVec3() const { return Vec3(x / w, y / w, z / w); }
};

struct Matrix4x4 {
  float m[16];
  Matrix4x4() {
    for (int i = 0; i < 16; ++i)
      m[i] = 0;
    m[0] = m[5] = m[10] = m[15] = 1; // Identity
  }

  Matrix4x4 operator*(const Matrix4x4 &other) const {
    Matrix4x4 result;
    // Column-major (OpenGL-style) multiplication; vectors are column vectors:
    // result = this * other
    for (int r = 0; r < 4; ++r) {
      for (int c = 0; c < 4; ++c) {
        result.m[c * 4 + r] = m[0 * 4 + r] * other.m[c * 4 + 0] +
                              m[1 * 4 + r] * other.m[c * 4 + 1] +
                              m[2 * 4 + r] * other.m[c * 4 + 2] +
                              m[3 * 4 + r] * other.m[c * 4 + 3];
      }
    }
    return result;
  }

  Vec4 operator*(const Vec4 &v) const {
    return Vec4(m[0] * v.x + m[4] * v.y + m[8] * v.z + m[12] * v.w,
                m[1] * v.x + m[5] * v.y + m[9] * v.z + m[13] * v.w,
                m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14] * v.w,
                m[3] * v.x + m[7] * v.y + m[11] * v.z + m[15] * v.w);
  }

  static Matrix4x4 rotationX(float angle) {
    Matrix4x4 mat;
    float c = cos(angle), s = sin(angle);
    mat.m[5] = c;
    mat.m[6] = -s;
    mat.m[9] = s;
    mat.m[10] = c;
    return mat;
  }

  static Matrix4x4 rotationY(float angle) {
    Matrix4x4 mat;
    float c = cos(angle), s = sin(angle);
    mat.m[0] = c;
    mat.m[2] = s;
    mat.m[8] = -s;
    mat.m[10] = c;
    return mat;
  }

  static Matrix4x4 rotationZ(float angle) {
    Matrix4x4 mat;
    float c = cos(angle), s = sin(angle);
    mat.m[0] = c;
    mat.m[1] = -s;
    mat.m[4] = s;
    mat.m[5] = c;
    return mat;
  }

  static Matrix4x4 translation(float x, float y, float z) {
    Matrix4x4 mat;
    mat.m[12] = x;
    mat.m[13] = y;
    mat.m[14] = z;
    return mat;
  }

  static Matrix4x4 scale(float x, float y, float z) {
    Matrix4x4 mat;
    mat.m[0] = x;
    mat.m[5] = y;
    mat.m[10] = z;
    return mat;
  }

  static Matrix4x4 perspective(float fov, float aspect, float near, float far) {
    Matrix4x4 mat;
    float tanHalfFov = tan(fov / 2.0f);
    mat.m[0] = 1.0f / (aspect * tanHalfFov);
    mat.m[5] = 1.0f / tanHalfFov;
    mat.m[10] = -(far + near) / (far - near);
    mat.m[11] = -1.0f;
    mat.m[14] = -2.0f * far * near / (far - near);
    mat.m[15] = 0.0f;
    return mat;
  }
};

struct Vec2 {
  float x, y;
  Vec2(float x = 0, float y = 0) : x(x), y(y) {}
};

class Camera {
private:
  Vec3 position;
  Vec3 rotation; // Euler angles
  float fov;
  float aspect;
  float near;
  float far;

public:
  Camera(Vec3 pos = Vec3(0, 0, 0), Vec3 rot = Vec3(0, 0, 0), float f = M_PI / 3,
         float n = 0.1f, float ffar = 100.0f)
      : position(pos), rotation(rot), fov(f), near(n), far(ffar), aspect(1.0f) {
  }

  void setAspect(float a) { aspect = a; }

  Matrix4x4 getViewMatrix() const {
    // Inverse rotation and translation
    Matrix4x4 rot = Matrix4x4::rotationZ(-rotation.z) *
                    Matrix4x4::rotationY(-rotation.y) *
                    Matrix4x4::rotationX(-rotation.x);
    Matrix4x4 trans =
        Matrix4x4::translation(-position.x, -position.y, -position.z);
    return rot * trans;
  }

  Matrix4x4 getProjectionMatrix() const {
    return Matrix4x4::perspective(fov, aspect, near, far);
  }

  void update(const Matrix4x4 &delta) {
    // For camera movement, but for now, not used
  }
};

class Object {
private:
  std::vector<Vec3> vertices;
  std::vector<std::vector<int>> faces; // Each face is a list of vertex indices
  Matrix4x4 transform;                 // Object's transformation matrix
  Vec3 position;                       // Object's position

public:
  Object(Vec3 pos = Vec3(0, 0, 0)) : transform(), position(pos) {
    // Initialize transform with position
    transform = Matrix4x4::translation(position.x, position.y, position.z);
  }

  bool loadFromObj(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open())
      return false;

    std::string line;
    while (std::getline(file, line)) {
      std::istringstream iss(line);
      std::string type;
      iss >> type;
      if (type == "v") {
        float x, y, z;
        iss >> x >> y >> z;
        vertices.push_back(Vec3(x, y, z));
      } else if (type == "f") {
        std::vector<int> face;
        std::string vertex;
        while (iss >> vertex) {
          // Handle vertex indices (ignore texture/normal if present)
          size_t slash = vertex.find('/');
          int idx =
              std::stoi(vertex.substr(0, slash)) - 1; // OBJ indices are 1-based
          face.push_back(idx);
        }
        faces.push_back(face);
      }
    }
    return true;
  }

  void setPosition(Vec3 pos) {
    position = pos;
    // Update transform with new position
    transform = Matrix4x4::translation(position.x, position.y, position.z) *
                transform; // Keep rotation
  }

  void update(const Matrix4x4 &delta) {
    transform = transform * delta; // Apply delta after current transform to
                                   // rotate around object's center
  }

  // Reimplemented method to handle pixel projection
  Vec2 projectToScreen(const Vec3 &worldPos, const Camera &camera,
                       int screenWidth, int screenHeight) const {
    Matrix4x4 view = camera.getViewMatrix();
    Matrix4x4 proj = camera.getProjectionMatrix();
    Matrix4x4 mvp = proj * view * transform;

    Vec4 v4(worldPos);
    Vec4 clip = mvp * v4;
    Vec3 ndc = clip.toVec3(); // Perspective divide

    // Viewport transform
    float screenX = (ndc.x + 1.0f) * 0.5f *
                    (screenWidth / 2.0f); // screenWidth is maxX, /2 for pixels
    float screenY = (1.0f - ndc.y) * 0.5f * screenHeight;

    return Vec2(screenX, screenY);
  }

  void render(Camera &camera) {
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    camera.setAspect((maxX * 0.5f) /
                     (float)maxY); // Correct aspect for wide characters

    clear();

    std::vector<Vec2> projected;
    for (const auto &vertex : vertices) {
      Vec2 screenPos = projectToScreen(vertex, camera, maxX, maxY);
      projected.push_back(screenPos);
    }

    // Draw edges from faces
    for (const auto &face : faces) {
      for (size_t i = 0; i < face.size(); ++i) {
        int idx1 = face[i];
        int idx2 = face[(i + 1) % face.size()];
        Vec2 p1 = projected[idx1];
        Vec2 p2 = projected[idx2];

        int x1 = round(p1.x), y1 = round(p1.y);
        int x2 = round(p2.x), y2 = round(p2.y);

        int dx = abs(x2 - x1);
        int dy = abs(y2 - y1);
        int sx = x1 < x2 ? 1 : -1;
        int sy = y1 < y2 ? 1 : -1;
        int err = (dx > dy ? dx : -dy) / 2;

        while (true) {
          if (x1 >= 0 && x1 < maxX / 2 && y1 >= 0 && y1 < maxY) {
            mvaddwstr(y1, x1 * 2, L"█");
            mvaddwstr(y1, x1 * 2 + 1, L"█");
          }

          if (x1 == x2 && y1 == y2)
            break;
          int e2 = err;
          if (e2 > -dx) {
            err -= dy;
            x1 += sx;
          }
          if (e2 < dy) {
            err += dx;
            y1 += sy;
          }
        }
      }
    }

    refresh();
  }
};

int main() {
  setlocale(LC_ALL, "");
  initscr();
  curs_set(0);
  timeout(0);
  Object obj(Vec3(0, 0, 0)); // Position at camera's location to rotate relative
                             // to camera's origin
  if (!obj.loadFromObj("cube.obj")) {
    // Fallback to hard-coded cube if file not found
    // For now, assume file exists or handle error
    endwin();
    return 1;
  }
  Camera camera(Vec3(0, 0, -2)); // Camera at same position
  while (true) {
    obj.render(camera);
    // Create delta transformation matrix for rotation around own axis
    Matrix4x4 delta = Matrix4x4::rotationX(0.03) * Matrix4x4::rotationY(0.03) *
                      Matrix4x4::rotationZ(0.03);
    obj.update(delta);
    if (getch() == 'q')
      break;
    std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
  }
  endwin();
  return 0;
}
