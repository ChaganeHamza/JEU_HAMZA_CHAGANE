#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)  // Instancie l'objet d'interface utilisateur
{
    ui->setupUi(this);  // Configure l'interface utilisateur pour cette boîte de dialogue
}

AboutDialog::~AboutDialog()
{
    delete ui;  // Libère la mémoire occupée par l'objet d'interface utilisateur
}
