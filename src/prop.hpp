#include <SFML/Graphics.hpp>
using sf::Drawable;

#ifndef PROP_H
#define PROP_H

template<typename T>
struct vec2_s {
    T x, y;
};
template<typename T>
using vec2 = struct vec2_s<T>;

enum sizeMode : char { FIXED = 'f', RELATIVE = 'r' };

class Prop : public Drawable {
private:
protected:
    vec2<float> pos;
    vec2<float> minSize;
    vec2<float> size;
    sizeMode mode;
public:
    Prop();
    ~Prop();
};

#endif // PROP_H
