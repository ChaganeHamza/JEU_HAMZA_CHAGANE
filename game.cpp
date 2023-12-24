#include "game.h"
#include <QPixmap>
#include <QLabel>
#include <ctime>
#include <algorithm>
#include <random>
#include <QDebug>
#include <QMessageBox>

// Constructeur de la classe Game
Game::Game(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Game),
    playerTurn(true) {
    // Initialisation de l'interface utilisateur
    ui->setupUi(this);

    // Initialisation des scores des joueurs
    playerScore = 0;
    computerScore = 0;

    // Connexion des signaux (clic sur les cartes) aux slots (fonctions correspondantes)
    connect(ui->player1, &ClickableLabel::clicked, this, &Game::onPlayerCardClicked);
    connect(ui->player2, &ClickableLabel::clicked, this, &Game::onPlayerCardClicked);
    connect(ui->player3, &ClickableLabel::clicked, this, &Game::onPlayerCardClicked);
    connect(ui->player4, &ClickableLabel::clicked, this, &Game::onPlayerCardClicked);

    // Connexion du clic sur le label de remplissage aux mains des joueurs
    connect(ui->fillLabel, &ClickableLabel::clicked, this, &Game::onFillHandsClicked);

    // Initialisation du deck, mélange et distribution des cartes, mise à jour des images
    initializeDeck();
    shuffleDeck();
    dealCards();
    updateCardImages();
}

// Fonction appelée lorsqu'une carte joueur est cliquée
void Game::onPlayerCardClicked() {
    if (!playerTurn) {
        return;
    }

    // Récupération de la carte cliquée
    ClickableLabel* clickedLabel = qobject_cast<ClickableLabel*>(sender());
    if (!clickedLabel)
        return;

    QString labelName = clickedLabel->objectName();
    int cardIndex = labelName.remove(0, 6).toInt() - 1;
    int selectedCard = playerCards[cardIndex];
    int selectedCardLastNumber = selectedCard % 10;

    // Vérification si la carte est déjà sur le plateau
    auto it = std::find_if(playAreaCards.begin(), playAreaCards.end(),
                           [selectedCardLastNumber](int card) { return card % 10 == selectedCardLastNumber; });

    if (it != playAreaCards.end()) {
        // La carte est déjà sur le plateau
        playAreaCards.erase(it);
        playerCards.erase(playerCards.begin() + cardIndex);
        playerScore += 2;
        updateScoreUI();
    } else {
        // La carte n'est pas sur le plateau, déplacer vers le plateau
        moveToPlayArea(cardIndex);
    }

    // Mettre à jour l'interface utilisateur
    updatePlayerHandUI();
    updatePlayAreaUI();

    // Passer au tour de l'ordinateur
    playerTurn = false;
    computerTurn();
}

// Fonction de mise à jour du score sur l'interface utilisateur
void Game::updateScoreUI() {
    QLabel* scoreLabel = findChild<QLabel*>("scoreLabel");
    if (scoreLabel) {
        scoreLabel->setText(QString("Score: %1").arg(playerScore));
    }
}

// Fonction appelée lors du clic sur le label de remplissage des mains
void Game::onFillHandsClicked() {
    if (!deck.empty() && playerCards.empty() && computerCards.empty()) {
        // Remplir les mains des joueurs avec de nouvelles cartes du deck
        for (int i = 0; i < 4; ++i) {
            if (!deck.empty()) {
                playerCards.push_back(deck.back());
                deck.pop_back();
            }
            if (!deck.empty()) {
                computerCards.push_back(deck.back());
                deck.pop_back();
            }
        }

        // Mettre à jour l'interface utilisateur
        updatePlayerHandUI();
        updateComputerHandUI();
    }
}

// Fonction du tour de l'ordinateur
void Game::computerTurn() {
    if (computerCards.empty()) {
        return;
    }

    // Choisir une carte aléatoire de la main de l'ordinateur
    int randomIndex = rand() % computerCards.size();
    int selectedCard = computerCards[randomIndex];
    int selectedCardLastNumber = selectedCard % 10;

    // Vérifier si la carte est déjà sur le plateau
    auto it = std::find_if(playAreaCards.begin(), playAreaCards.end(),
                           [selectedCardLastNumber](int card) { return card % 10 == selectedCardLastNumber; });

    if (it != playAreaCards.end()) {
        // La carte est déjà sur le plateau
        playAreaCards.erase(it);
        computerCards.erase(computerCards.begin() + randomIndex);
        computerScore += 2;
        updateComputerScoreUI();
    } else {
        // La carte n'est pas sur le plateau, ajouter au plateau
        playAreaCards.push_back(selectedCard);
        computerCards.erase(computerCards.begin() + randomIndex);
    }

    // Mettre à jour l'interface utilisateur
    updateComputerHandUI();
    updatePlayAreaUI();

    // Passer au tour du joueur
    playerTurn = true;

    // Vérifier si le jeu est terminé
    if (deck.empty() && computerCards.empty()) {
        gameOver();
        return;
    }
}

// Fonction appelée en fin de jeu
void Game::gameOver() {
    qDebug() << "Game Over";

    // Déterminer le résultat du jeu (victoire du joueur, de l'ordinateur ou égalité)
    QString result;
    if (playerScore > computerScore) {
        result = "Player wins!";
    } else if (computerScore > playerScore) {
        result = "Computer wins!";
    } else {
        result = "It's a tie!";
    }
    qDebug() << result;

    // Afficher un message d'information sur l'interface utilisateur
    QMessageBox::information(this, "Game Over", result);
}

// Fonction de mise à jour du score de l'ordinateur sur l'interface utilisateur
void Game::updateComputerScoreUI() {
    QLabel* scoreLabel = findChild<QLabel*>("computerScoreLabel");
    if (scoreLabel) {
        scoreLabel->setText(QString("Score: %1").arg(computerScore));
    }
}

// Fonction de mise à jour de l'interface utilisateur pour montrer la main de l'ordinateur
void Game::updateComputerHandUI() {
    // Cacher les images actuelles de la main de l'ordinateur
    for (int i = 1; i <= 4; ++i) {
        QLabel* label = findChild<QLabel*>(QString("computer%1").arg(i));
        if (label != nullptr) {
            label->setPixmap(QPixmap(":/path/to/default/image"));
        }
    }

    // Afficher les nouvelles images de la main de l'ordinateur
    for (size_t i = 0; i < computerCards.size(); ++i) {
        QString cardImagePath = QString(":/new/prefix1/%1.gif").arg(computerCards[i], 2, 10, QChar('0'));
        QLabel* label = findChild<QLabel*>(QString("computer%1").arg(i + 1));
        if (label != nullptr) {
            label->setPixmap(QPixmap(cardImagePath));
        }
    }
}

// Fonction de déplacement d'une carte du joueur vers le plateau
void Game::moveToPlayArea(int cardIndex) {
    if (cardIndex < 0 || cardIndex >= static_cast<int>(playerCards.size())) {
        return;
    }
    int cardToMove = playerCards[cardIndex];

    // Vérifier si la carte est déjà sur le plateau
    auto it = std::find(playAreaCards.begin(), playAreaCards.end(), cardToMove);
    if (it == playAreaCards.end()) {
        // La carte n'est pas sur le plateau, ajouter au plateau
        playAreaCards.push_back(cardToMove);
        playerCards.erase(playerCards.begin() + cardIndex);
        updatePlayAreaUI();
    } else {
        // La carte est déjà sur le plateau
        // TODO: Ajouter une logique supplémentaire si nécessaire
    }

    // Mettre à jour l'interface utilisateur
    updatePlayerHandUI();
}

// Fonction de mise à jour de l'interface utilisateur pour montrer les cartes sur le plateau
void Game::updatePlayAreaUI() {
    // Cacher les images actuelles du plateau
    for (size_t i = 0; i < 17; ++i) {
        QString cardImagePath = QString(":/new/prefix1/blank.gif");
        QLabel* label = findChild<QLabel*>(QString("playArea%1").arg(i + 1));
        if (label != nullptr) {
            label->setPixmap(QPixmap(cardImagePath));
        }
    }

    // Afficher les nouvelles images des cartes sur le plateau
    for (size_t i = 0; i < playAreaCards.size(); ++i) {
        QString cardImagePath = QString(":/new/prefix1/%1.gif").arg(playAreaCards[i], 2, 10, QChar('0'));
        QLabel* label = findChild<QLabel*>(QString("playArea%1").arg(i + 1));
        if (label != nullptr) {
            label->setPixmap(QPixmap(cardImagePath));
        }
    }
}

// Fonction de mise à jour de l'interface utilisateur pour montrer la main du joueur
void Game::updatePlayerHandUI() {
    // Cacher les images actuelles de la main du joueur
    for (int i = 1; i <= 4; ++i) {
        QLabel* label = findChild<QLabel*>(QString("player%1").arg(i));
        if (label != nullptr) {
            label->setPixmap(QPixmap(":/path/to/default/image"));
        }
    }

    // Afficher les nouvelles images de la main du joueur
    for (size_t i = 0; i < playerCards.size(); ++i) {
        QString cardImagePath = QString(":/new/prefix1/%1.gif").arg(playerCards[i], 2, 10, QChar('0'));
        QLabel* label = findChild<QLabel*>(QString("player%1").arg(i + 1));
        if (label != nullptr) {
            label->setPixmap(QPixmap(cardImagePath));
        }
    }
}

// Destructeur de la classe Game
Game::~Game() {
    delete ui;
}

// Initialiser le deck avec des cartes
void Game::initializeDeck() {
    deck.resize(40);
    std::iota(deck.begin(), deck.end(), 0);
}

// Mélanger le deck
void Game::shuffleDeck() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(deck.begin(), deck.end(), std::default_random_engine(seed));
}

// Distribuer les cartes aux joueurs
void Game::dealCards() {
    playerCards.clear();
    computerCards.clear();

    for (int i = 0; i < 4; ++i) {
        playerCards.push_back(deck[i]);
        computerCards.push_back(deck[i + 4]);
    }

    deck.erase(deck.begin(), deck.begin() + 8);

    // Mettre à jour les images des cartes sur l'interface utilisateur
    updateCardImages();
}

// Mettre à jour les images des cartes sur l'interface utilisateur
void Game::updateCardImages() {
    for (size_t i = 0; i < playerCards.size(); ++i) {
        QString playerCardPath = QString(":/new/prefix1/%1.gif").arg(playerCards[i], 2, 10, QChar('0'));
        QString computerCardPath = QString(":/new/prefix1/%1.gif").arg(computerCards[i], 2, 10, QChar('0'));

        QLabel* playerLabel = findChild<QLabel*>(QString("player%1").arg(i + 1));
        if (playerLabel != nullptr) {
            playerLabel->setPixmap(QPixmap(playerCardPath));
        }

        QLabel* computerLabel = findChild<QLabel*>(QString("computer%1").arg(i + 1));
        if (computerLabel != nullptr) {
            computerLabel->setPixmap(QPixmap(computerCardPath));
        }
    }
}
