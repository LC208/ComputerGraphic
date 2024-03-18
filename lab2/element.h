#pragma once
enum ElementType{Active, Nonactive};

class Element
{
public:
    virtual void render() = 0;
    virtual void update() = 0;
    float getX() const{return x;};
    float getY() const{return y;};
    float getWidth() const{return width;};
    float getHeight() const{return height;};
    void setWidth(float width) {this->width=width;};
    void setHeight(float height) {this->height=height;};
    void setX(float x) {this->x=x;};
    void setY(float y) {this->y=y;};
    ElementType getType() const{return type;};
    void operator()(){action();};
    Element(float x,float y,float width, float height, void (*action)(), ElementType type): x{x},y{y},width{width},height{height},action{action},type{type}{};
private:
    float x;
    float y;
    ElementType type;
    float width = 0;
    float height = 0;
    void (*action)();
};
