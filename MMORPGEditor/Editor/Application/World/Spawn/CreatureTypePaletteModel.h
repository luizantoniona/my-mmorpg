#ifndef CREATURETYPEPALETTEMODEL_H
#define CREATURETYPEPALETTEMODEL_H

#include <QAbstractListModel>
#include <QStringList>

class CreatureTypePaletteModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum Role {
        TypeRole = Qt::UserRole + 1,
        NameRole
    };

    explicit CreatureTypePaletteModel( QObject* parent = nullptr );

    int rowCount( const QModelIndex& parent = QModelIndex() ) const override;
    QVariant data( const QModelIndex& index, int role ) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    QStringList _types;
};

#endif // CREATURETYPEPALETTEMODEL_H
