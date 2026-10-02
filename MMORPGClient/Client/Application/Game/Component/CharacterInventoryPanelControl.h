#ifndef CHARACTERINVENTORYPANELCONTROL_H
#define CHARACTERINVENTORYPANELCONTROL_H

#include <cstdint>
#include <vector>

#include <QObject>
#include <QString>
#include <QVariantList>

#include <MMORPGEngine/Entity/Character/CharacterInventoryModel.h>
#include <MMORPGEngine/Entity/Character/OwnInventoryDTO.h>

class CharacterInventoryPanelControl : public QObject {
    Q_OBJECT
    Q_PROPERTY( QVariantList inventory READ inventory NOTIFY inventoryChanged )

public:
    explicit CharacterInventoryPanelControl( QObject* parent = nullptr );
    ~CharacterInventoryPanelControl();

    QVariantList inventory() const;

public slots:
    QString itemName( uint32_t idItem ) const;

signals:
    void inventoryChanged();

private slots:
    void onOwnInventoryReceived( const Engine::OwnInventoryDTO& state );

private:
    std::vector<Engine::CharacterInventoryModel> _inventory;
};

#endif // CHARACTERINVENTORYPANELCONTROL_H
