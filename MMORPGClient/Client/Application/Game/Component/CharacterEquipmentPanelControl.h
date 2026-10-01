#ifndef CHARACTEREQUIPMENTPANELCONTROL_H
#define CHARACTEREQUIPMENTPANELCONTROL_H

#include <cstdint>
#include <vector>

#include <QObject>
#include <QString>
#include <QVariantList>

#include <MMORPGEngine/Entity/Character/CharacterEquipmentModel.h>
#include <MMORPGEngine/Entity/Character/OwnEquipmentDTO.h>

class CharacterEquipmentPanelControl : public QObject {
    Q_OBJECT
    Q_PROPERTY( QVariantList equipment READ equipment NOTIFY equipmentChanged )

public:
    explicit CharacterEquipmentPanelControl( QObject* parent = nullptr );
    ~CharacterEquipmentPanelControl();

    QVariantList equipment() const;

public slots:
    QString itemName( uint32_t idItem ) const;

signals:
    void equipmentChanged();

private slots:
    void onOwnEquipmentReceived( const Engine::OwnEquipmentDTO& state );

private:
    std::vector<Engine::CharacterEquipmentModel> _equipment;
};

#endif // CHARACTEREQUIPMENTPANELCONTROL_H
