#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    masker = new Masker;

    this->setCentralWidget(masker);
    this->resize(800, 600);
}

MainWindow::~MainWindow()
{
    delete ui;
}
