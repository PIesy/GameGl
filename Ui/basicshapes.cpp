#include "basicshapes.h"

Mesh Shapes::Plane(int xSize, int ySize, StorageApi& api, const std::string& name, Vec4&& color)
{
    float vertCoordsX[] = {0, 0, xSize, xSize};
    float vertCoordsZ[] = {0, ySize, ySize, 0};
    float texCoordsU[] = {0, 0, 1, 1};
    float texCoordsV[] = {0, 1, 1, 0};

    std::vector<unsigned> indices = {0, 1, 2,  2, 3, 0};
    std::vector<Vertex> vertices;

    for (int i = 0; i < 4; i++)
    {
        Vertex vert;
        vert.coords = {vertCoordsX[i], 0, vertCoordsZ[i]};
        vert.normal = {0, 1, 0};
        vert.color = color;
        vert.uv = {texCoordsU[i], texCoordsV[i]};
        vert.tangent = {0, 0, 1};
        vertices.push_back(vert);
    }
    auto desc1 = api.Place(vertices.size() * sizeof(Vertex), vertices.data());
    auto desc2 = api.Place(indices.size() * sizeof(unsigned), indices.data());

    return Mesh({desc1.pointer, desc2.pointer}, {vertices.size(), indices.size()}, name);
}

Mesh Shapes::Rectangle(StorageApi& api, const std::string& name)
{
    float coordsX[] = {-1, -1, 1, 1};
    float coordsY[] = {-1, 1, 1, -1};
    float u[] = {0, 0, 1, 1};
    float v[] = {0, 1, 1, 0};

    std::vector<unsigned> indices = {0, 1, 2,  2, 3, 0};
    std::vector<Vertex> vertices;

    for (int i = 0; i < 4; i++)
    {
        Vertex vert;
        vert.coords = {coordsX[i], coordsY[i], 0};
        vert.normal = {0, 1, 0};
        vert.color = {0.5f, 0.5f, 0.5f, 1};
        vert.uv = {u[i], v[i]};
        vertices.push_back(vert);
    }
    auto desc1 = api.Place(vertices.size() * sizeof(Vertex), vertices.data());
    auto desc2 = api.Place(indices.size() * sizeof(unsigned), indices.data());
    return Mesh({desc1.pointer, desc2.pointer}, {vertices.size(), indices.size()}, name);
}
