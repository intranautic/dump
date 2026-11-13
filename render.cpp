#include <locale.h>
#include <ncurses.h>

#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
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

  Vec3 &operator+=(const Vec3 &v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }

  float length() const { return std::sqrt(x * x + y * y + z * z); }

  Vec3 normalized() const {
    float len = length();
    if (len <= 1e-6f)
      return Vec3();
    float invLen = 1.0f / len;
    return (*this) * invLen;
  }
};

struct Vec4 {
  float x, y, z, w;
  Vec4(float x = 0, float y = 0, float z = 0, float w = 1)
      : x(x), y(y), z(z), w(w) {}
  Vec4(const Vec3 &v, float w = 1) : x(v.x), y(v.y), z(v.z), w(w) {}
  Vec3 toVec3() const { return Vec3(x / w, y / w, z / w); }

  Vec4 operator+(const Vec4 &v) const {
    return Vec4(x + v.x, y + v.y, z + v.z, w + v.w);
  }

  Vec4 operator-(const Vec4 &v) const {
    return Vec4(x - v.x, y - v.y, z - v.z, w - v.w);
  }

  Vec4 operator*(float f) const { return Vec4(x * f, y * f, z * f, w * f); }
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

  static Matrix4x4 rotationFromEuler(const Vec3 &rotation) {
    return Matrix4x4::rotationZ(rotation.z) * Matrix4x4::rotationY(rotation.y) *
           Matrix4x4::rotationX(rotation.x);
  }

  static Matrix4x4 fromPositionRotation(const Vec3 &position,
                                        const Vec3 &rotation) {
    Matrix4x4 translation =
        Matrix4x4::translation(position.x, position.y, position.z);
    Matrix4x4 rotationMatrix = rotationFromEuler(rotation);
    return translation * rotationMatrix;
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

  void setPosition(const Vec3 &pos) { position = pos; }
  void setRotation(const Vec3 &rot) { rotation = rot; }
  void setTransform(const Vec3 &pos, const Vec3 &rot) {
    position = pos;
    rotation = rot;
  }

  const Vec3 &getPosition() const { return position; }
  const Vec3 &getRotation() const { return rotation; }

  Matrix4x4 getRotationMatrix() const {
    return Matrix4x4::rotationFromEuler(rotation);
  }

  Vec3 transformDirection(const Vec3 &dir) const {
    Vec4 result = getRotationMatrix() * Vec4(dir, 0.0f);
    return Vec3(result.x, result.y, result.z);
  }

  Vec3 getForward() const {
    return transformDirection(Vec3(0, 0, -1)).normalized();
  }

  Vec3 getRight() const {
    return transformDirection(Vec3(1, 0, 0)).normalized();
  }

  Vec3 getUp() const { return transformDirection(Vec3(0, 1, 0)).normalized(); }

  void moveLocal(const Vec3 &localOffset) {
    Vec4 offset(localOffset, 0.0f);
    Vec4 worldOffset = getRotationMatrix() * offset;
    position += Vec3(worldOffset.x, worldOffset.y, worldOffset.z);
  }

  void moveWorld(const Vec3 &worldOffset) { position += worldOffset; }

  void addRotation(const Vec3 &deltaRotation) { rotation += deltaRotation; }

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
};

struct Transform {
  Vec3 position;
  Vec3 rotation;
  Transform(const Vec3 &pos = Vec3(), const Vec3 &rot = Vec3())
      : position(pos), rotation(rot) {}
};

struct WorldConfig {
  Vec3 objectRotationSpeed;
  explicit WorldConfig(const Vec3 &rotationSpeed = Vec3(1.8f, 1.2f, 0.6f))
      : objectRotationSpeed(rotationSpeed) {}
};

class Object {
private:
  struct ClipPlane {
    float a, b, c, d;
  };

  std::vector<Vec3> vertices;
  std::vector<std::vector<int>> faces; // Each face is a list of vertex indices

  static float evaluatePlane(const ClipPlane &plane, const Vec4 &vertex) {
    return plane.a * vertex.x + plane.b * vertex.y + plane.c * vertex.z +
           plane.d * vertex.w;
  }

  static Vec4 intersectPlane(const ClipPlane &plane, const Vec4 &from,
                             const Vec4 &to) {
    float fromEval = evaluatePlane(plane, from);
    float toEval = evaluatePlane(plane, to);
    float denom = fromEval - toEval;
    if (std::abs(denom) < 1e-6f)
      return to;
    float t = fromEval / (fromEval - toEval);
    return from + (to - from) * t;
  }

  static std::vector<Vec4>
  clipPolygonToFrustum(const std::vector<Vec4> &polygon) {
    static const ClipPlane planes[] = {
        {1.0f, 0.0f, 0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f, 1.0f},
        {0.0f, 1.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, -1.0f, 1.0f}};

    std::vector<Vec4> result = polygon;
    std::vector<Vec4> temp;

    for (const auto &plane : planes) {
      temp.clear();
      if (result.empty())
        break;

      Vec4 previous = result.back();
      float previousEval = evaluatePlane(plane, previous);
      bool previousInside = previousEval >= 0.0f;

      for (const auto &current : result) {
        float currentEval = evaluatePlane(plane, current);
        bool currentInside = currentEval >= 0.0f;

        if (currentInside != previousInside) {
          temp.push_back(intersectPlane(plane, previous, current));
        }
        if (currentInside) {
          temp.push_back(current);
        }

        previous = current;
        previousEval = currentEval;
        previousInside = currentInside;
      }

      result = temp;
    }

    return result;
  }

  static Vec2 clipToScreen(const Vec4 &clip, int screenWidth,
                           int screenHeight) {
    Vec3 ndc = Vec3(clip.x / clip.w, clip.y / clip.w, clip.z / clip.w);
    float screenX = (ndc.x + 1.0f) * 0.5f * (screenWidth / 2.0f);
    float screenY = (1.0f - ndc.y) * 0.5f * screenHeight;
    return Vec2(screenX, screenY);
  }

  static void drawEdge(const Vec2 &p1, const Vec2 &p2, int screenWidth,
                       int screenHeight) {
    int x1 = static_cast<int>(std::round(p1.x));
    int y1 = static_cast<int>(std::round(p1.y));
    int x2 = static_cast<int>(std::round(p2.x));
    int y2 = static_cast<int>(std::round(p2.y));

    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2;

    while (true) {
      if (x1 >= 0 && x1 < screenWidth / 2 && y1 >= 0 && y1 < screenHeight) {
        mvaddwstr(y1, x1 * 2, L"█");
        mvaddwstr(y1, x1 * 2 + 1, L"█");
        //       mvaddstr(y1, x1 * 2, "'");
        //       mvaddstr(y1, x1 * 2 + 1, "'");
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

public:
  bool loadFromObj(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open())
      return false;

    vertices.clear();
    faces.clear();

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

  void render(const Camera &camera, const Matrix4x4 &modelMatrix,
              int screenWidth, int screenHeight) const {
    Matrix4x4 view = camera.getViewMatrix();
    Matrix4x4 projection = camera.getProjectionMatrix();
    Matrix4x4 mvp = projection * view * modelMatrix;

    for (const auto &face : faces) {
      std::vector<Vec4> clipPolygon;
      clipPolygon.reserve(face.size());
      for (int index : face) {
        Vec4 vertex(vertices[index], 1.0f);
        clipPolygon.push_back(mvp * vertex);
      }

      clipPolygon = clipPolygonToFrustum(clipPolygon);
      if (clipPolygon.size() < 2)
        continue;

      std::vector<Vec2> screenSpace;
      screenSpace.reserve(clipPolygon.size());
      for (const auto &clipVertex : clipPolygon) {
        if (std::abs(clipVertex.w) < 1e-5f)
          continue;
        screenSpace.push_back(
            clipToScreen(clipVertex, screenWidth, screenHeight));
      }

      if (screenSpace.size() < 2)
        continue;

      for (size_t i = 0; i < screenSpace.size(); ++i) {
        const Vec2 &p1 = screenSpace[i];
        const Vec2 &p2 = screenSpace[(i + 1) % screenSpace.size()];
        drawEdge(p1, p2, screenWidth, screenHeight);
      }
    }
  }
};

class World {
private:
  struct ObjectInstance {
    Object object;
    Transform transform;
  };

  std::vector<ObjectInstance> objects;
  WorldConfig config;

public:
  explicit World(const WorldConfig &config = WorldConfig()) : config(config) {}

  bool loadObject(const std::string &filename,
                  const Transform &transform = Transform()) {
    Object mesh;
    if (!mesh.loadFromObj(filename)) {
      return false;
    }

    objects.push_back(ObjectInstance{std::move(mesh), transform});
    return true;
  }

  void update(float deltaTime) {
    Vec3 deltaRotation = config.objectRotationSpeed * deltaTime;
    for (auto &instance : objects) {
      instance.transform.rotation += deltaRotation;
    }
  }

  void render(Camera &camera) const {
    int maxY = 0;
    int maxX = 0;
    getmaxyx(stdscr, maxY, maxX);
    if (maxY == 0 || maxX == 0)
      return;

    camera.setAspect((maxX * 0.5f) / static_cast<float>(maxY));

    clear();

    for (const auto &instance : objects) {
      Matrix4x4 model = Matrix4x4::fromPositionRotation(
          instance.transform.position, instance.transform.rotation);
      instance.object.render(camera, model, maxX, maxY);
    }

    refresh();
  }
};

struct CameraControlsConfig {
  float translationSpeed;
  float verticalSpeed;
  float rotationSpeed;
  float inputHoldTimeout;
};

struct AppConfig {
  WorldConfig worldConfig;
  CameraControlsConfig cameraControls;
  Transform cameraTransform;
  std::chrono::milliseconds frameDuration;
};

struct CameraInputState {
  Vec3 localMovement;
  float verticalMovement;
  Vec3 rotation;
  CameraInputState() : localMovement(), verticalMovement(0.0f), rotation() {}
};

struct CameraControlRuntime {
  CameraInputState state;
  Vec3 localHoldTimer;
  float verticalHoldTimer;
  Vec3 rotationHoldTimer;
  CameraControlRuntime()
      : state(), localHoldTimer(), verticalHoldTimer(0.0f),
        rotationHoldTimer() {}
};

const AppConfig kDefaultAppConfig = {
    WorldConfig(Vec3(1.8f, 1.2f, 0.6f)),
    CameraControlsConfig{3.0f, 3.0f, 1.5f, 0.2f},
    Transform(Vec3(0.0f, 0.0f, 5.0f), Vec3()), std::chrono::milliseconds(32)};

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <model.obj>\n";
    return 1;
  }

  setlocale(LC_ALL, "");
  initscr();
  curs_set(0);
  timeout(0);
  keypad(stdscr, TRUE);

  const AppConfig appConfig = kDefaultAppConfig;

  World world(appConfig.worldConfig);
  if (!world.loadObject(argv[1])) {
    endwin();
    return 1;
  }

  std::cout << "here" << std::endl;
  Camera camera(appConfig.cameraTransform.position,
                appConfig.cameraTransform.rotation);

  auto lastFrame = std::chrono::steady_clock::now();
  bool running = true;
  CameraControlRuntime controlRuntime;
  while (running) {
    auto now = std::chrono::steady_clock::now();
    float deltaTime = std::chrono::duration<float>(now - lastFrame).count();
    lastFrame = now;

    controlRuntime.localHoldTimer.x += deltaTime;
    controlRuntime.localHoldTimer.y += deltaTime;
    controlRuntime.localHoldTimer.z += deltaTime;
    controlRuntime.verticalHoldTimer += deltaTime;
    controlRuntime.rotationHoldTimer.x += deltaTime;
    controlRuntime.rotationHoldTimer.y += deltaTime;
    controlRuntime.rotationHoldTimer.z += deltaTime;

    int ch = 0;
    while ((ch = getch()) != ERR) {
      switch (ch) {
      case 'w':
      case 'W':
        controlRuntime.state.localMovement.z = -2.0f;
        controlRuntime.localHoldTimer.z = 0.0f;
        break;
      case 's':
      case 'S':
        controlRuntime.state.localMovement.z = 2.0f;
        controlRuntime.localHoldTimer.z = 0.0f;
        break;
      case 'a':
      case 'A':
        controlRuntime.state.localMovement.x = -2.0f;
        controlRuntime.localHoldTimer.x = 0.0f;
        break;
      case 'd':
      case 'D':
        controlRuntime.state.localMovement.x = 2.0f;
        controlRuntime.localHoldTimer.x = 0.0f;
        break;
      case 'r':
      case 'R':
        controlRuntime.state.verticalMovement = 2.0f;
        controlRuntime.verticalHoldTimer = 0.0f;
        break;
      case 'f':
      case 'F':
        controlRuntime.state.verticalMovement = -2.0f;
        controlRuntime.verticalHoldTimer = 0.0f;
        break;
      case KEY_UP:
        controlRuntime.state.rotation.x = -1.0f;
        controlRuntime.rotationHoldTimer.x = 0.0f;
        break;
      case KEY_DOWN:
        controlRuntime.state.rotation.x = 1.0f;
        controlRuntime.rotationHoldTimer.x = 0.0f;
        break;
      case KEY_LEFT:
        controlRuntime.state.rotation.y = -1.0f;
        controlRuntime.rotationHoldTimer.y = 0.0f;
        break;
      case KEY_RIGHT:
        controlRuntime.state.rotation.y = 1.0f;
        controlRuntime.rotationHoldTimer.y = 0.0f;
        break;
      case 'q':
      case 'Q':
        running = false;
        break;
      default:
        break;
      }

      if (!running)
        break;
    }

    if (!running)
      break;

    const float holdTimeout = appConfig.cameraControls.inputHoldTimeout;
    if (controlRuntime.localHoldTimer.x > holdTimeout)
      controlRuntime.state.localMovement.x = 0.0f;
    if (controlRuntime.localHoldTimer.z > holdTimeout)
      controlRuntime.state.localMovement.z = 0.0f;
    if (controlRuntime.verticalHoldTimer > holdTimeout)
      controlRuntime.state.verticalMovement = 0.0f;
    if (controlRuntime.rotationHoldTimer.x > holdTimeout)
      controlRuntime.state.rotation.x = 0.0f;
    if (controlRuntime.rotationHoldTimer.y > holdTimeout)
      controlRuntime.state.rotation.y = 0.0f;

    if (controlRuntime.state.localMovement.length() > 0.0f) {
      Vec3 direction = controlRuntime.state.localMovement.normalized();
      camera.moveLocal(direction *
                       (appConfig.cameraControls.translationSpeed * deltaTime));
    }

    if (std::abs(controlRuntime.state.verticalMovement) > 0.0f) {
      camera.moveWorld(Vec3(0.0f,
                            controlRuntime.state.verticalMovement *
                                appConfig.cameraControls.verticalSpeed *
                                deltaTime,
                            0.0f));
    }

    if (controlRuntime.state.rotation.length() > 0.0f) {
      camera.addRotation(controlRuntime.state.rotation.normalized() *
                         (appConfig.cameraControls.rotationSpeed * deltaTime));
    }

    world.update(deltaTime);
    world.render(camera);

    std::this_thread::sleep_for(appConfig.frameDuration);
  }
  endwin();
  return 0;
}
