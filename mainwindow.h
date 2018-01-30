#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QMessageBox>
#include <QPixmap>
#include <iostream>
#include <string>
#include <QDir>
#include <QFileDialog>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    std::string imagefileName;

    int blurSize = 5;
    int charWidth = 20;
    int charHeight = 30;
    int blockSize = 11;
    int contourArea = 100;

    cv::Mat image;
    cv::Mat imageGrayScale,
                imageBlurred,
                imageThresholded,
                imageThresholdedCopy;

private slots:
    void open_image();
    void process_image();
    void display_image();

    void receiveBlurSize(int value);
    void receiveCharWidth(int value);
    void receiveCharHeight(int value);
    void receiveBlockSize(int value);
    void receiveContourArea(int value);
    void on_actionExit_triggered();
    void on_startTrainingButton_clicked();
};

#endif // MAINWINDOW_H
