#ifndef EDITORPAGECONTROL_H
#define EDITORPAGECONTROL_H

#include <QObject>

class EditorPageControl : public QObject {
    Q_OBJECT

public:
    explicit EditorPageControl( QObject* parent = nullptr );
    ~EditorPageControl() override;
};

#endif // EDITORPAGECONTROL_H
