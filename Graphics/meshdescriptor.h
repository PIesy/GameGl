//
// Created by akann on 4/27/17.
//

#ifndef GAMEGL_MESHDESCRIPTOR_H
#define GAMEGL_MESHDESCRIPTOR_H

#include "mesh.h"

struct MeshDescriptor
{
    Mesh mesh;
    ConfigFunction configuration;

    MeshDescriptor(const Mesh& mesh, const ConfigFunction& configuration) : mesh(mesh), configuration(configuration) {}
    MeshDescriptor() {}
};


#endif //GAMEGL_MESHDESCRIPTOR_H
