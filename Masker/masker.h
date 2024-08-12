#ifndef MASKER_H
#define MASKER_H

#include <QWidget>

#include <QSlider>
#include <QImage>
#include <QVBoxLayout>
#include <QPushButton>

#include <QFileDialog>

class Masker : public QWidget
{
    Q_OBJECT

private:
    QLayout* mainLayout,
           * btnLayout;

    QWidget* btnWidget;

    QImage* image;

    QSlider* lowHue,
             lowSat,
             upHue,
             upSat;

    QPushButton* openBtn,
               * saveBtn;

private slots:
    void OpenFile();
    void SaveFile();

public:
    explicit Masker(QWidget *parent = nullptr);


signals:
};

#endif // MASKER_H
