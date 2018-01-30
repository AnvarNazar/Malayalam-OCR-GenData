#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "trainingdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->startTrainingButton->setEnabled(false);

    ui->blurSizeBox->setValue(blurSize);
    ui->blurSizeSlider->setValue(blurSize);
    ui->charWidthBox->setValue(charWidth);
    ui->charWidthSlider->setValue(charWidth);
    ui->charHeightBox->setValue(charHeight);
    ui->charHeightSlider->setValue(charHeight);
    ui->blockSizeBox->setValue(blockSize);
    ui->blockSizeSlider->setValue(blockSize);
    ui->contourAreaBox->setValue(contourArea);
    ui->contourAreaSlider->setValue(contourArea);


    connect(ui->blurSizeSlider, SIGNAL(valueChanged(int)), ui->blurSizeBox, SLOT(setValue(int)));
    connect(ui->charWidthSlider, SIGNAL(valueChanged(int)), ui->charWidthBox, SLOT(setValue(int)));
    connect(ui->charHeightSlider, SIGNAL(valueChanged(int)), ui->charHeightBox, SLOT(setValue(int)));
    connect(ui->blockSizeSlider, SIGNAL(valueChanged(int)), ui->blockSizeBox, SLOT(setValue(int)));
    connect(ui->contourAreaSlider, SIGNAL(valueChanged(int)), ui->contourAreaBox, SLOT(setValue(int)));

    connect(ui->blurSizeBox, SIGNAL(valueChanged(int)), ui->blurSizeSlider, SLOT(setValue(int)));
    connect(ui->charWidthBox, SIGNAL(valueChanged(int)), ui->charWidthSlider, SLOT(setValue(int)));
    connect(ui->charHeightBox, SIGNAL(valueChanged(int)), ui->charHeightSlider, SLOT(setValue(int)));
    connect(ui->blockSizeBox, SIGNAL(valueChanged(int)), ui->blockSizeSlider, SLOT(setValue(int)));
    connect(ui->contourAreaBox, SIGNAL(valueChanged(int)), ui->contourAreaSlider, SLOT(setValue(int)));

    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(open_image()));

    connect(ui->blurSizeBox, SIGNAL(valueChanged(int)), this, SLOT(receiveBlurSize(int)));
    connect(ui->blurSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(receiveBlurSize(int)));

    connect(ui->charWidthBox, SIGNAL(valueChanged(int)), this, SLOT(receiveCharWidth(int)));
    connect(ui->charWidthSlider, SIGNAL(valueChanged(int)), this, SLOT(receiveCharWidth(int)));

    connect(ui->charHeightBox, SIGNAL(valueChanged(int)), this, SLOT(receiveCharHeight(int)));
    connect(ui->charHeightSlider, SIGNAL(valueChanged(int)), this, SLOT(receiveCharHeight(int)));

    connect(ui->blockSizeBox, SIGNAL(valueChanged(int)), this, SLOT(receiveBlockSize(int)));
    connect(ui->blockSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(receiveBlockSize(int)));

    connect(ui->contourAreaBox, SIGNAL(valueChanged(int)), this, SLOT(receiveContourArea(int)));
    connect(ui->contourAreaSlider, SIGNAL(valueChanged(int)), this, SLOT(receiveContourArea(int)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::open_image()
{
    imagefileName = QFileDialog::getOpenFileName(this, "Open Image", QDir::currentPath(), tr("JPEG Format(*.jpg *.jpeg);;PNG Format(*.png);;BMP(*.bmp)")).toStdString();
    if(imagefileName.empty()) {
        return;
    }
    this->image.release();
    process_image();
    display_image();
    ui->startTrainingButton->setEnabled(true);
}

void MainWindow::process_image()
{
    if(image.empty() && imagefileName.empty())
        return;
    if(image.empty()) {
        if(imagefileName.empty()) {
            return;
        }
        this->image = cv::imread(imagefileName);
        if(image.empty()) {
            QMessageBox::information(this, "Error in opening image", "The specified image file is not supported.");
            return;
        }
        cv::cvtColor(image, imageGrayScale, CV_BGR2GRAY);
        cv::GaussianBlur(imageGrayScale, imageBlurred, cv::Size(blurSize, blurSize), 0);

        cv::adaptiveThreshold(imageBlurred, imageThresholded, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV, blockSize, 2);
        imageThresholdedCopy = imageThresholded.clone();
    } else {
        cv::GaussianBlur(imageGrayScale, imageBlurred, cv::Size(blurSize, blurSize), 0);
        cv::adaptiveThreshold(imageBlurred, imageThresholded, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV, blockSize, 2);
        imageThresholdedCopy = imageThresholded.clone();
    }
}

void MainWindow::display_image()
{
    if(imageThresholdedCopy.empty()) {
        return;
    }

    QImage d_image(imageThresholdedCopy.data, imageThresholdedCopy.cols, imageThresholdedCopy.rows, QImage::Format_Grayscale8);
    ui->label->setPixmap(QPixmap::fromImage(d_image));
}

void MainWindow::receiveBlurSize(int value)
{
    if(value > 1 && value % 2 == 1) {
        blurSize = value;
        process_image();
        display_image();
    }
    return;
}

void MainWindow::receiveCharWidth(int value)
{
    charWidth = value;
}

void MainWindow::receiveCharHeight(int value)
{
    charHeight = value;
}

void MainWindow::receiveBlockSize(int value)
{
    if(value > 1 && value % 2 == 1) {
        blurSize = value;
        process_image();
        display_image();
    }
}

void MainWindow::receiveContourArea(int value)
{
    contourArea = value;
}

void MainWindow::on_actionExit_triggered()
{
    MainWindow::close();
}

void MainWindow::on_startTrainingButton_clicked()
{
    TrainingDialog dialog(this, imageThresholded, charWidth, charHeight, contourArea);
    dialog.exec();
}
