#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>
#include <QWidget>
#include <Qt>

class ClickableLabel : public QLabel {
    Q_OBJECT  // Active les signaux et les slots

public:
    explicit ClickableLabel(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~ClickableLabel();

signals:
    void clicked();  // Signal émis lorsque le label est cliqué

protected:
    void mousePressEvent(QMouseEvent* event);  // Surcharge de la fonction pour gérer l'événement de clic de la souris
};

#endif // CLICKABLELABEL_H
