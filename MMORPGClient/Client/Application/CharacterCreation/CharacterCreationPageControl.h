#ifndef CHARACTERCREATIONPAGECONTROL_H
#define CHARACTERCREATIONPAGECONTROL_H

#include <QObject>
#include <QStringList>
#include <QVariantList>
#include <QVariantMap>

#include <MMORPGEngine/Entity/Character/CharacterAppearanceModel.h>

class CharacterCreationPageControl : public QObject {
    Q_OBJECT

    Q_PROPERTY( QVariantMap appearance READ appearance NOTIFY appearanceChanged )
    Q_PROPERTY( bool hasValidAppearance READ hasValidAppearance NOTIFY appearanceChanged )

public:
    explicit CharacterCreationPageControl( QObject* parent = nullptr );
    ~CharacterCreationPageControl();

    QVariantMap appearance() const;
    bool hasValidAppearance() const;

    Q_INVOKABLE QStringList appearanceFeatures() const;
    Q_INVOKABLE QString appearanceFeatureTitle( const QString& feature ) const;
    Q_INVOKABLE QVariantList appearanceOptions( const QString& feature, const QVariantMap& selection ) const;

    static Engine::CharacterAppearanceModel appearanceFromSelections( const QVariantMap& selections );

public slots:
    void selectAppearanceOption( const QString& feature, const QString& id );

    void createCharacter( const QString& name );

signals:
    void appearanceChanged();

    void characterCreated();
    void characterCreationFailed( const QString& error );

private:
    Engine::CharacterAppearanceModel _appearance;
};

#endif // CHARACTERCREATIONPAGECONTROL_H
