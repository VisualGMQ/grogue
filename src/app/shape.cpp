#include "app/shape.hpp"

Shape Shape::CreateRect(const math::Vector2& center, const math::Vector2& halfLen, const Color& color) {
    Shape shape;
    shape.vertices.resize(4);
    shape.vertices[0].position = math::Vector2(center.x - halfLen.x, center.y - halfLen.y);
    shape.vertices[1].position = math::Vector2(center.x + halfLen.x, center.y - halfLen.y);
    shape.vertices[2].position = math::Vector2(center.x + halfLen.x, center.y + halfLen.y);
    shape.vertices[3].position = math::Vector2(center.x - halfLen.x, center.y + halfLen.y);

    for (auto& vertex : shape.vertices) {
        vertex.color = color;
        vertex.texcoord.Set(0, 0);
    }

    shape.indices = std::vector<uint32_t>{0, 1, 2, 0, 2, 3};
    return shape;
}

Shape Shape::CreateCircle(const math::Vector2& center, float radius, uint32_t slice, const Color& color) {
    auto degStep = 2.0 * math::PI / slice;
    std::vector<Vertex> vertices(slice + 1);
    vertices[0].position = center;
    vertices[0].color = color;
    vertices[0].texcoord.Set(0, 0);
    for (int i = 1; i <= slice; i++) {
        float deg = (i - 1) * degStep;
        vertices[i].position = center + math::Vector2(radius * std::cos(deg), radius * std::sin(deg));
        vertices[i].texcoord.Set(0, 0);
        vertices[i].color = color;
    }
    std::vector<uint32_t> indices(slice * 3);
    for (int i = 0; i < slice; i++) {
        indices[i * 3] = 0;
        indices[i * 3 + 1] = i + 1;
        indices[i * 3 + 2] = i + 2;
        if (i + 2 >= vertices.size()) {
            indices[i * 3 + 2] = 1;
        }
    }

    return Shape {
        std::move(vertices),
        std::move(indices),
    };
}