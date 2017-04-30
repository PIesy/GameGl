//
// Created by akann on 4/10/17.
//

#ifndef GAMEGL_GLOBJECT_H
#define GAMEGL_GLOBJECT_H

#include "glbindings.h"

namespace gl {

    class GlObject
    {
    public:
        virtual ~GlObject(){}
        virtual GLuint GetId() const = 0;
    };

}


#endif //GAMEGL_GLOBJECT_H
