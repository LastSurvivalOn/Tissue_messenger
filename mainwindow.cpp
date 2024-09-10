#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "barwidget.h"
#include <QThread>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    BarWidget *n = new BarWidget();
    delete n;
}

MainWindow::~MainWindow()
{
    delete ui;
}
