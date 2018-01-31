//
// Created by akann on 4/27/17.
//

#ifndef GAMEGL_MESHDESCRIPTOR_H
#define GAMEGL_MESHDESCRIPTOR_H

#include "mesh.h"

struct MeshDescriptor
{
    Mesh mesh;
    ShaderConfiguration configuration;

    MeshDescriptor(const Mesh& mesh, const ShaderConfiguration& configuration) : mesh(mesh), configuration(configuration) {}
    MeshDescriptor() {}
};


#endif //GAMEGL_MESHDESCRIPTOR_H
