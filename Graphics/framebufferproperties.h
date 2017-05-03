//
// Created by akann on 5/3/17.
//

#ifndef GAMEGL_FRAMEBUFFERPROPERTIES_H
#define GAMEGL_FRAMEBUFFERPROPERTIES_H

struct FrameBufferProperties
{
    enum BufferName {ALL = 0, COLOR = 1, DEPTH = 1 << 1, STENCIL = 1 << 2};
    enum Type {NEW, REUSE_FROM_STEP, NEW_COPY_BUFFERS_FROM};
    Type frameBufferType = NEW;
    BufferName sourceBuffer = ALL;
    unsigned stepId = 0;
};


#endif //GAMEGL_FRAMEBUFFERPROPERTIES_H
