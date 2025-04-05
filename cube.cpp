#include <ncurses.h>
#include <locale.h>
#include <vector>
#include <cmath>
#include <chrono>
#include <thread>

struct Vec3 {
    float x, y, z;
    Vec3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
    
    Vec3 operator+(const Vec3& v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
    Vec3 operator-(const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
    Vec3 operator*(float f) const { return Vec3(x * f, y * f, z * f); }
};

struct Vec2 {
    float x, y;
    Vec2(float x = 0, float y = 0) : x(x), y(y) {}
};

class Cube {
private:
    std::vector<Vec3> vertices;
    std::vector<std::vector<int>> edges;
    float rotationX = 0, rotationY = 0, rotationZ = 0;

public:
    Cube() {
        vertices = {
            Vec3(-1, -1, -1), Vec3(1, -1, -1),
            Vec3(1, 1, -1),   Vec3(-1, 1, -1),
            Vec3(-1, -1, 1),  Vec3(1, -1, 1),
            Vec3(1, 1, 1),    Vec3(-1, 1, 1)
        };

        edges = {
            {0,1}, {1,2}, {2,3}, {3,0},
            {4,5}, {5,6}, {6,7}, {7,4},
            {0,4}, {1,5}, {2,6}, {3,7}
        };
    }

    Vec2 project(const Vec3& v, float distance) {
        float z = v.z + distance;
        float scale = distance / z;
        return Vec2(v.x * scale, v.y * scale);
    }

    Vec3 rotate(const Vec3& v) {
       
        float y1 = v.y * cos(rotationX) - v.z * sin(rotationX);
        float z1 = v.y * sin(rotationX) + v.z * cos(rotationX);
        
        
        float x2 = v.x * cos(rotationY) + z1 * sin(rotationY);
        float z2 = -v.x * sin(rotationY) + z1 * cos(rotationY);
        
        float x3 = x2 * cos(rotationZ) - y1 * sin(rotationZ);
        float y3 = x2 * sin(rotationZ) + y1 * cos(rotationZ);
        
        return Vec3(x3, y3, z2);
    }

    void update() {
        rotationX += 0.03;
        rotationY += 0.02;
        rotationZ += 0.01;
    }

    void render() {
        int maxY, maxX;
        getmaxyx(stdscr, maxY, maxX);
        float centerX = maxX / 4;  
        float centerY = maxY / 2;
        float scale = std::min(maxX/2, maxY) / 4; 
        float distance = 5;

        clear();

        std::vector<Vec2> projected;
        for (const auto& vertex : vertices) {
            Vec3 rotated = rotate(vertex);
            Vec2 proj = project(rotated, distance);
            projected.push_back(Vec2(
                centerX + proj.x * scale,
                centerY + proj.y * scale
            ));
        }

        for (const auto& edge : edges) {
            Vec2 p1 = projected[edge[0]];
            Vec2 p2 = projected[edge[1]];
            
            int x1 = round(p1.x), y1 = round(p1.y);
            int x2 = round(p2.x), y2 = round(p2.y);
            
            int dx = abs(x2 - x1);
            int dy = abs(y2 - y1);
            // size of x/y pixels being drawn
            // handles horizontal/vertical pixels for coordinate system
            int sx = x1 < x2 ? 1 : -1;
            int sy = y1 < y2 ? 1 : -1;
            int err = (dx > dy ? dx : -dy) / 2;
            
            while (true) {
                if (x1 >= 0 && x1 < maxX-1 && y1 >= 0 && y1 < maxY) {
                    mvaddwstr(y1, x1*2, L"█");
                    mvaddwstr(y1, x1*2 + 1, L"█");
                    
                }
                
                if (x1 == x2 && y1 == y2) break;
                int e2 = err;
                if (e2 > -dx) { err -= dy; x1 += sx; }
                if (e2 < dy) { err += dx; y1 += sy; }
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
    Cube cube;
    while (true) {
        cube.render();
        cube.update();
        if (getch() == 'q') break;    
        std::this_thread::sleep_for(std::chrono::milliseconds(11)); // ~30 FPS
    }
    endwin();
    return 0;
}

