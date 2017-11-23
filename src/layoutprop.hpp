#include <vector>
using std::vector;
#include <SFML/Graphics.hpp>
using sf::Drawable;
using sf::Transformable;
using sf::Transform;
using sf::RenderTarget;
using sf::RenderStates;
using sf::Vector2f;
#include "prop.hpp"

#ifndef LAYOUTPROP_H
#define LAYOUTPROP_H

class Layout : public Prop {
protected:
    vector<Prop*> prop;
public:
    Layout();
    virtual ~Layout();
    void addChild(Prop* p);
    void clear();
    virtual void draw(RenderTarget& target, RenderStates states) const =0;
};

class VerticalLayout : public Layout {
public:
    void draw(RenderTarget& target, RenderStates states) const;
};

class HorizontalLayout : public Layout {
public:
    void draw(RenderTarget& target, RenderStates states) const;
};

#endif // LAYOUTPROP_H
