#include "ClickableLabel.h"
#include <QMouseEvent>

ClickableLabel::ClickableLabel(QWidget* parent, Qt::WindowFlags f)
    : QLabel(parent) {
    // Constructeur de la classe ClickableLabel, héritant de la classe QLabel

    // Le constructeur ne réalise actuellement aucune initialisation spécifique
    // Il utilise l'initialisation de la classe de base QLabel avec le widget parent fourni
}

ClickableLabel::~ClickableLabel() {
    // Destructeur de la classe ClickableLabel
    // Aucune action particulière n'est effectuée ici car la classe ne détient pas de ressources nécessitant une libération explicite
}

void ClickableLabel::mousePressEvent(QMouseEvent* event) {
    // Surcharge de la fonction mousePressEvent pour gérer l'événement de clic de la souris

    emit clicked();  // Émet le signal "clicked()" lorsque le label est cliqué
    // Ce signal peut être connecté à un slot pour effectuer des actions en réponse au clic du label
}
