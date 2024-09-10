#ifndef BARWIDGET_H
#define BARWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
class BarWidget:public QWidget
{
    Q_OBJECT
public:
    BarWidget();
    ~BarWidget();
    bool InitBarInterface();
    QPushButton *SettingsButton;
    QPushButton *AppearanceButton;
    QPushButton *SearchButton;
    QLineEdit *SearchArea;
    QHBoxLayout *layout;
};

#endif // BARWIDGET_H
