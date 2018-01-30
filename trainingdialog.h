#ifndef TRAININGDIALOG_H
#define TRAININGDIALOG_H

#include <QDialog>

namespace Ui {
class TrainingDialog;
}

class TrainingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TrainingDialog(QWidget *parent = 0);
    ~TrainingDialog();

private:
    Ui::TrainingDialog *ui;
};

#endif // TRAININGDIALOG_H
