#ifndef SELECTIONCONTROL_H
#define SELECTIONCONTROL_H

#include <QObject>

class SelectionControl : public QObject {
    Q_OBJECT
    Q_PROPERTY( bool hasSelection READ hasSelection NOTIFY selectionChanged )
    Q_PROPERTY( int x READ x NOTIFY selectionChanged )
    Q_PROPERTY( int y READ y NOTIFY selectionChanged )
    Q_PROPERTY( int z READ z NOTIFY selectionChanged )

public:
    explicit SelectionControl( QObject* parent = nullptr );

    bool hasSelection() const;

    int x() const;
    int y() const;
    int z() const;

public slots:
    void selectTile( int x, int y, int z );
    void clearSelection();

signals:
    void selectionChanged();

private:
    bool _hasSelection;
    int _x;
    int _y;
    int _z;
};

#endif // SELECTIONCONTROL_H
