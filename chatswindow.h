#ifndef CHATSWINDOW_H
#define CHATSWINDOW_H

#include <QWidget>
#include <QListWidget>

class ChatsWindow: public QWidget
{
    Q_OBJECT
public:
    ChatsWindow();

private:
    int limit=100;

};

#endif // CHATSWINDOW_H
