#ifndef EDITORPAGECONTROL_H
#define EDITORPAGECONTROL_H

#include <memory>

#include <QObject>
#include <QString>
#include <QVariantList>

#include <MMORPGEngine/Core/World/Model/WorldModel.h>

class EditorPageControl : public QObject {
    Q_OBJECT
    Q_PROPERTY( QString worldName READ worldName NOTIFY worldChanged )
    Q_PROPERTY( QVariantList grounds READ grounds NOTIFY worldChanged )

public:
    explicit EditorPageControl( QObject* parent = nullptr );

    QString worldName() const;
    QVariantList grounds() const;

    Q_INVOKABLE void loadWorld( const QString& path );

signals:
    void worldChanged();

private:
    std::unique_ptr<Engine::WorldModel> _world;

};

#endif // EDITORPAGECONTROL_H
