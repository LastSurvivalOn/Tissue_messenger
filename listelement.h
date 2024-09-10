#ifndef LISTELEMENT_H
#define LISTELEMENT_H

#include <QWidget>

class ListElement:public QWidget
{
    Q_OBJECT
public:
    ListElement();
private:
    QString name;
    QString text;
    QString url;

};

#endif // LISTELEMENT_H
