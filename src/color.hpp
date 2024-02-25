#pragma once

#include <ostream>
#include <glm/vec3.hpp>

namespace levin
{
    class Color
    {
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

    bool operator==(const Color &c1, const Color &c2)
    {
        return c1.r() == c2.r() && c1.g() == c2.g() && c1.b() == c2.b();
    }

    std::ostream &operator<<(std::ostream &os, const Color &c)
    {
        os << "Color(" << c.r() << ", " << c.g() << ", " << c.b() << ")";
        return os;
    }

    Color hadamard_product(const Color &c1, const Color &c2)
    {
        return Color(c1.r() * c2.r(), c1.g() * c2.g(), c1.b() * c2.b());
    }
}