#ifndef STREAMRENDERWORLD_H
#define STREAMRENDERWORLD_H

#include <QHash>
#include <QImage>
#include <QString>

#include <MMORPGEngine/Data/Appearance/AppearanceModel.h>
#include <MMORPGEngine/Entity/Character/CharacterAppearanceModel.h>
#include <MMORPGEngine/Entity/GroundItem/GroundItemModel.h>
#include <MMORPGEngine/Renderer/EntityAnimationModel.h>
#include <MMORPGEngine/Renderer/World/RenderWorld.h>

namespace Engine {

class StreamRenderWorld : public RenderWorld {
    Q_OBJECT

public:
    explicit StreamRenderWorld( QObject* parent = nullptr );

    QList<Entity> entities( int z ) const override;

    Q_INVOKABLE void setOwnCharacter( int idCharacter, int x, int y, int z, double movementSeconds = 0.0 );

    Q_INVOKABLE void addCharacter( int idCharacter, int x, int y, int z, double movementSeconds = 0.0 );
    Q_INVOKABLE void startCharacterAttack( int idCharacter, int targetX, int targetY, double castSeconds );
    Q_INVOKABLE void removeCharacter( int idCharacter );

    void setCharacterAppearance( int idCharacter, const CharacterAppearanceModel& characterAppearance );

    Q_INVOKABLE void addCreature( int idCreature, int x, int y, int z, double movementSeconds = 0.0, int footprint = 1, const QString& creatureType = QString() );
    Q_INVOKABLE void startCreatureAttack( int idCreature, int targetX, int targetY, double castSeconds );
    Q_INVOKABLE void removeCreature( int idCreature );

    Q_INVOKABLE void addGroundItem( int idGroundItem, const QString& idItem, int x, int y, int z );
    Q_INVOKABLE void removeGroundItem( int idGroundItem );
    Q_INVOKABLE int groundItemAt( int x, int y, int z ) const;

    Q_INVOKABLE void clearEntities();

private:
    static void updateEntity( QHash<int, EntityAnimationModel>& entities, int idEntity, int x, int y, int z, double movementSeconds );
    static QImage frameOf( const AppearanceModel& appearance, const EntityAnimationModel& animation );

    void appendCharacters( QList<Entity>& result, const QHash<int, EntityAnimationModel>& characters, int z ) const;
    const AppearanceModel* characterAppearance( int idCharacter ) const;

    QImage creatureTexture( int idCreature, const EntityAnimationModel& animation ) const;

private:
    QHash<int, EntityAnimationModel> _ownCharacter;
    QHash<int, EntityAnimationModel> _characters;
    QHash<int, EntityAnimationModel> _creatures;
    QHash<int, CharacterAppearanceModel> _characterAppearances;
    mutable QHash<QString, AppearanceModel> _composedAppearances;
    QHash<int, QString> _creatureTypes;
    QHash<int, int> _creatureFootprints;
    QHash<int, GroundItemModel> _groundItems;
};

} // namespace Engine

#endif // STREAMRENDERWORLD_H
