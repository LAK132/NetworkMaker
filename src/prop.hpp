#include <SFML/Graphics.hpp>
using sf::Drawable;
using sf::Transformable;
using sf::RenderTarget;
using sf::RenderStates;
using sf::Vector2f;

#ifndef PROP_H
#define PROP_H

enum sizeMode : uint8_t { AUTO = 0, RELATIVE_LEFT = 1, RELATIVE_RIGHT = 1<<1, RELATIVE_TOP = 1<<2, RELATIVE_BOTTOM = 1<<3, ABSOLUTE = 1<<4, FILL = 1<<5};

class Prop : public Drawable, public Transformable {
private:
protected:
    Vector2f size;
    sizeMode mode;
public:
    virtual void draw(RenderTarget& target, RenderStates states) const =0;
};

#endif // PROP_H
