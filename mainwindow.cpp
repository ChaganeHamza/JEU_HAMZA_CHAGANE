#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "game.h"

#include <QDebug>

// Constructeur de la classe MainWindow
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // Initialisation de l'interface utilisateur
    ui->setupUi(this);

    // Connexion des signaux et slots pour les boutons
    connect(ui->aproposButton, &QPushButton::clicked, this, &MainWindow::showAboutDialog);
    connect(ui->play_btn, &QPushButton::clicked, this, &MainWindow::openGameWindow);
    connect(ui->play_btn2, &QPushButton::clicked, this, &MainWindow::openGameWindow);
}

// Destructeur de la classe MainWindow
MainWindow::~MainWindow()
{
    // Libération de la mémoire allouée pour l'interface utilisateur
    delete ui;
}

// Fonction pour afficher la boîte de dialogue "À propos"
void MainWindow::showAboutDialog() {
    // Création et exécution de la boîte de dialogue "À propos"
    AboutDialog dialog(this);
    dialog.exec();
}

// Fonction pour ouvrir la fenêtre de jeu
void MainWindow::openGameWindow() {
    // Création d'une instance de la classe Game
    Game *gameWindow = new Game();

    // Configuration de certains attributs de la fenêtre de jeu
    gameWindow->setAttribute(Qt::WA_DeleteOnClose);
    gameWindow->show();
    gameWindow->raise();
    gameWindow->activateWindow();
}
