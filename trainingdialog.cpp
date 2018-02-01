#include "trainingdialog.h"
#include "ui_trainingdialog.h"

TrainingDialog::TrainingDialog(QWidget *parent, cv::Mat image, int minCharWidth, int minCharHeight, int contourArea) :
    QDialog(parent),
    ui(new Ui::TrainingDialog)
{
    ui->setupUi(this);
    this->image = image;
    this->minCharWidth = minCharWidth;
    this->minCharHeight = minCharHeight;
    this->contourArea = contourArea;

    cv::findContours(image, ptContours, cv::RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    advanceContour();
    displayCharacter();
}

TrainingDialog::~TrainingDialog()
{
    delete ui;
}

void TrainingDialog::displayCharacter()
{
    cv::Rect boundingRect = cv::boundingRect(ptContours[currentContour]);
    charMat = image(boundingRect);
    cv::resize(charMat, charMatResized, cv::Size(minCharWidth, minCharHeight));
    cv::Mat displayChar = charMatResized.clone();
    qCharImage = QImage(displayChar.data, displayChar.cols, displayChar.rows, QImage::Format_Grayscale8);
    ui->charPixelLabel->setPixmap(QPixmap::fromImage(qCharImage));
}

void TrainingDialog::advanceContour()
{
    for(unsigned int i = currentContour; i < ptContours.size(); i++) {
        if(cv::contourArea(ptContours[i]) >= contourArea) {
            currentContour = i;
            break;
        }
    }
}

void TrainingDialog::on_lineEdit_returnPressed()
{
    if(ui->lineEdit->text().isEmpty())
        return;
    auto result = letters.mLetters.find(ui->lineEdit->text().toStdString());
    if(result != letters.mLetters.end()) {
        matClassificationInts.push_back(result->second);
        cv::Mat matImageFloat;
        charMatResized.convertTo(matImageFloat, CV_32FC1);
        cv::Mat matImageFlattenedFloat = matImageFloat.reshape(1, 1);
          matTrainingImagesAsFlattenedFloats.push_back(matImageFlattenedFloat);
        ui->lineEdit->clear();
    }
    currentContour++;
    if(currentContour>=ptContours.size()) {
        QMessageBox::information(this, "Success", "Training complete");
        ui->lineEdit->setEnabled(false);
        ui->skipButton->setEnabled(false);
        return;
    }
    advanceContour();
    displayCharacter();
}

void TrainingDialog::on_cancelButton_clicked()
{
    TrainingDialog::close();
}

void TrainingDialog::on_saveButton_clicked()
{
    trainingFileName = QFileDialog::getSaveFileName(this, "Save File", QDir::currentPath()).toStdString();

    if(trainingFileName.empty()) {
        return;
    }

    cv::FileStorage trainingDataFile(trainingFileName, cv::FileStorage::WRITE);

    if(!trainingDataFile.isOpened()) {
        QMessageBox::information(this, "Error", "Unable to save the file");
        return;
    }
    trainingDataFile<<"classifications"<<matClassificationInts;
    trainingDataFile<<"images"<<matTrainingImagesAsFlattenedFloats;
    QMessageBox::information(this, "Save", "Training data file saved");
    trainingDataFile.release();
    TrainingDialog::close();
}

void TrainingDialog::on_skipButton_clicked()
{
    currentContour++;
    advanceContour();
    displayCharacter();
}
