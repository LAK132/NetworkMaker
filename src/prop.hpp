#include "imgui.h"

#ifndef PROP_H
#define PROP_H

class Prop {
private:
protected:
public:
    ImVec2 pos;
    ImVec2 size;
    ImVec2 padding;
    virtual bool render(ImDrawList*, ImVec2 = ImVec2(0.0f, 0.0f)) =0;
};

#endif // PROP_H
