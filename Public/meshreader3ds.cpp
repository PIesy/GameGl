#include "meshreader3ds.h"

void MeshReader3ds::Analyze(std::string input)
{
    std::string type = input.substr(0, 2);
    if(type == "v ")
        addVertex(input.substr(2));
    if(type == "f ")
        addIndex(input.substr(2));
    if(type == "vn")
        addNormal(input.substr(2));
}

void MeshReader3ds::addVertex(std::string vertex)
{
    size_t cnt = 0;
    size_t offset = 0;
    Vertex v;

    for(int i = 0; i < 3; i++)
    {
        v.coords[i] = std::stof(vertex.substr(offset), &cnt) / 100;
        offset += cnt;
    }
    result.vertices.push_back(v);
}

void MeshReader3ds::addIndex(std::string index)
{
    size_t cnt = 0;
    size_t offset = 0;
    Vertex v;

    for(int i = 0; i < 9; i++)
    {
        while(index[offset] != ' ' && (index[offset] > '9' || index[offset] < '0'))
            offset++;
        if(i % 3 != 0)
            std::stoul(index.substr(offset), &cnt);
        else
            result.indices.push_back(std::stoul(index.substr(offset), &cnt) - 1);
        offset += cnt;
    }
}

void MeshReader3ds::addNormal(std::string normal)
{
    static int i = 0;
    size_t cnt = 0;
    size_t offset = 0;
    Vec3 v;

    for(int j = 0; j < 3; j++)
    {
        v[j] = std::stof(normal.substr(offset), &cnt);
        offset += cnt;
    }
    result.vertices[i].normal = v;
    i++;
}

void MeshReader3ds::Clear()
{
    result.indices.clear();
    result.vertices.clear();
}

VertexObject MeshReader3ds::getResult()
{
    return result;
}
