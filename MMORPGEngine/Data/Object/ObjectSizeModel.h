#ifndef OBJECTSIZEMODEL_H
#define OBJECTSIZEMODEL_H

namespace Engine {

class ObjectSizeModel {
public:
    ObjectSizeModel();
    ~ObjectSizeModel();

    int width() const;
    void setWidth( int width );

    int height() const;
    void setHeight( int height );

private:
    int _width;
    int _height;
};

} // namespace Engine

#endif // OBJECTSIZEMODEL_H
