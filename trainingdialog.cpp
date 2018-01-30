#include "trainingdialog.h"
#include "ui_trainingdialog.h"

TrainingDialog::TrainingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TrainingDialog)
{
    ui->setupUi(this);
}

TrainingDialog::~TrainingDialog()
{
    delete ui;
}
