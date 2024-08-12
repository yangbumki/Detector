#include "masker.h"

Masker::Masker(QWidget *parent)
    : QWidget{parent}
{
    mainLayout = new QVBoxLayout;
    btnLayout = new QHBoxLayout;

    btnWidget = new QWidget;

    openBtn = new QPushButton("열기");
    saveBtn = new QPushButton("저장");

    connect(openBtn, &QPushButton::clicked, this, &Masker::OpenFile);
    connect(saveBtn, &QPushButton::clicked, this, &Masker::SaveFile);

    btnLayout->addWidget(openBtn);
    btnLayout->addWidget(saveBtn);

    btnWidget->setLayout(btnLayout);

    mainLayout->addWidget(btnWidget);

    this->setLayout(mainLayout);
}

void Masker::OpenFile() {
    QString dir;

    dir = QFileDialog::getOpenFileName(this, "파일 선택", QDir::currentPath(), "Image Files(*png, *bmp)");

}

void Masker::SaveFile() {
}

