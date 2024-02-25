#pragma once

#include <glm/vec3.hpp>

class Color
{
private:
    glm::vec3 _rgb;

public:
    Color(float r, float g, float b) : _rgb(r, g, b)
    {
        assert(r >= 0 && r <= 1);
        assert(g >= 0 && g <= 1);
        assert(b >= 0 && b <= 1);
    }

    Color(glm::vec3 rgb) : Color(rgb.x, rgb.y, rgb.z) {}

    Color() : Color(0, 0, 0) {}

    float r() const { return _rgb.x; }

    float g() const { return _rgb.y; }

    float b() const { return _rgb.z; }
};
