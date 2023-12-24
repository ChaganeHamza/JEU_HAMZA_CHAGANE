#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <vector>
#include "ui_game.h"
#include "ClickableLabel.h"

class Game : public QWidget {
    Q_OBJECT  // Active les signaux et les slots

public:
    explicit Game(QWidget *parent = nullptr);
    virtual ~Game();

private slots:
    void onPlayerCardClicked();  // Slot appelé lorsqu'une carte du joueur est cliquée
    void onFillHandsClicked();   // Slot appelé lorsqu'il faut remplir les mains des joueurs

private:
    Ui::Game *ui;  // Pointeur vers l'objet d'interface utilisateur généré
    std::vector<int> deck;  // Pile de cartes complète
    std::vector<int> playerCards;  // Cartes du joueur
    std::vector<int> computerCards;  // Cartes de l'ordinateur
    std::vector<int> playAreaCards;  // Cartes sur la table
    bool playerTurn;  // Indique si c'est le tour du joueur
    int playerScore;  // Score du joueur
    int computerScore;  // Score de l'ordinateur

    void initializeDeck();  // Initialise la pile de cartes
    void shuffleDeck();  // Mélange la pile de cartes
    void dealCards();  // Distribue les cartes aux joueurs
    void updateCardImages();  // Met à jour les images des cartes à l'écran
    void moveToPlayArea(int cardIndex);  // Déplace une carte de la main vers la table de jeu
    void updatePlayAreaUI();  // Met à jour l'interface utilisateur de la table de jeu
    void updatePlayerHandUI();  // Met à jour l'interface utilisateur de la main du joueur
    void updateComputerHandUI();  // Met à jour l'interface utilisateur de la main de l'ordinateur
    void updateScoreUI();  // Met à jour l'interface utilisateur du score du joueur
    void updateComputerScoreUI();  // Met à jour l'interface utilisateur du score de l'ordinateur
    void computerTurn();  // Fonction pour gérer le tour de l'ordinateur
    void gameOver();  // Fonction appelée en fin de partie
};

#endif
