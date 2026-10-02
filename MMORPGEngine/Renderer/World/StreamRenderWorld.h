#ifndef STREAMRENDERWORLD_H
#define STREAMRENDERWORLD_H

#include <QHash>
#include <QImage>

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
    Q_INVOKABLE void removeCharacter( int idCharacter );

    Q_INVOKABLE void addCreature( int idCreature, int x, int y, int z, double movementSeconds = 0.0 );
    Q_INVOKABLE void removeCreature( int idCreature );

    Q_INVOKABLE void clearEntities();

private:
    static void updateEntity( QHash<int, EntityAnimationModel>& entities, int idEntity, int x, int y, int z, double movementSeconds );
    static void appendEntities( QList<Entity>& result, const QHash<int, EntityAnimationModel>& entities, int z, const QImage& texture );

private:
    QHash<int, EntityAnimationModel> _ownCharacter;
    QHash<int, EntityAnimationModel> _characters;
    QHash<int, EntityAnimationModel> _creatures;
};

} // namespace Engine

#endif // STREAMRENDERWORLD_H
