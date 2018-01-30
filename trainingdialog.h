#ifndef TRAININGDIALOG_H
#define TRAININGDIALOG_H

#include <QDialog>
#include <QImage>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "letters.h"

namespace Ui {
class TrainingDialog;
}

class TrainingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TrainingDialog(QWidget *parent = 0, cv::Mat image = cv::Mat(), int minCharWidth = 20, int minCharHeight = 30, int contourArea = 100);
    ~TrainingDialog();
    QImage qCharImage;
    cv::Mat image;
    std::string trainingFileName;
    int minCharWidth, minCharHeight;
    int contourArea;

    std::string characterStringEntry;
    std::vector<std::vector<cv::Point>> ptContours;
    int currentContour = 0;
    std::vector<cv::Vec4i> v4iHierarchy;
    cv::Mat matClassificationInts;
    cv::Mat matTrainingImagesAsFlattenedFloats;
    cv::Mat charMat;
    cv::Mat charMatResized;
    Letters letters = Letters("Letters");

private slots:
    void on_lineEdit_returnPressed();

    void on_cancelButton_clicked();

    void on_saveButton_clicked();

    void on_skipButton_clicked();

private:
    Ui::TrainingDialog *ui;

    void displayCharacter();
    void advanceContour();


};

#endif // TRAININGDIALOG_H
