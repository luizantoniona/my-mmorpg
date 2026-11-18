#ifndef CHARACTEROPTIONMODEL_H
#define CHARACTEROPTIONMODEL_H

#include <QString>

namespace Engine {

class CharacterOptionModel {
public:
    CharacterOptionModel();
    CharacterOptionModel( const QString& id, const QString& name );

    const QString& id() const;
    const QString& name() const;

private:
    QString _id;
    QString _name;
};

} // namespace Engine

#endif // CHARACTEROPTIONMODEL_H
