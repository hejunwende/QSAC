#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_myplot.h"
#include <qmessagebox.h>
#include <QVector>
#include <iostream>
#include <rsac.h>
#include <qcustomplot.h>

class MyPlate : public QMainWindow
{
    Q_OBJECT

public:

    MyPlate(QWidget* parent = nullptr);
    ~MyPlate();
private slots:
    void open_FileName_triggered();

private:
    Ui::myplotClass ui;
    void updateUI(const QVector<double>& sacVector, const SACHEAD& header);
};
