#ifndef ENTITYSIZEMODEL_H
#define ENTITYSIZEMODEL_H

namespace Engine {

class EntitySizeModel {
public:
    EntitySizeModel();
    ~EntitySizeModel();

    int width() const;
    void setWidth( int width );

    int height() const;
    void setHeight( int height );

private:
    int _width;
    int _height;
};

} // namespace Engine

#endif // ENTITYSIZEMODEL_H
