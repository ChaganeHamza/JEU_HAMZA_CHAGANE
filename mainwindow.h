#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "game.h"  // Inclut le fichier d'en-tête de la classe Game

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT  // Active les signaux et les slots

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;  // Pointeur vers l'objet d'interface utilisateur généré
private slots:
    void showAboutDialog();  // Slot appelé pour afficher la boîte de dialogue "À propos"
    void openGameWindow();   // Slot appelé pour ouvrir la fenêtre de jeu
};
#endif // MAINWINDOW_H
