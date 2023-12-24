#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class AboutDialog;  // Déclaration anticipée de la classe d'interface utilisateur (UI)
}

class AboutDialog : public QDialog
{
    Q_OBJECT  // Active les signaux et les slots

public:
    explicit AboutDialog(QWidget *parent = nullptr);
    ~AboutDialog();

private:
    Ui::AboutDialog *ui;  // Pointeur vers la classe d'interface utilisateur (UI)
};

#endif // ABOUTDIALOG_H
