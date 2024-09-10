#ifndef TISSUE_SERVER_H
#define TISSUE_SERVER_H

#include <QObject>
#include <QWidget>
#include <QTcpServer>
#include <QTextEdit>
#include <QTcpSocket>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QTime>
#include <QIODevice>
#include <QSet>
#include <QPointer>
#include <QFile>
#include <QDir>

#include "messagechat.h"
#include "chatusers.h"

struct User{
    int ID;
    QString name;
};

class Tissue_server:public QWidget
{
    Q_OBJECT
public:

    Tissue_server(int Port, QWidget *pwgt=0);

    void load_chats();

    void append_user(int ID, QString name);

    void create_chat(ChatUsers *chat);

    void load_users();

    bool is_name_exists(QString name);

    int get_unique_id();

    bool is_chat_name_exists(QString name);

    int get_unique_chat_id();

    void update_chat_file(ChatUsers* chat, MessageChat* mess);
private:
    QTcpServer *server;
    QTextEdit *textedit;
    quint16 nextBlockSize;
    QSet<QTcpSocket*> clients;
    //QSet<QPointer<QTcpSocket>> clients;
    void sendToClient(QTcpSocket* pSocket, ChatUsers& chattt, const MessageChat& mess);

    QVector<ChatUsers*> allchats;
    QVector<User> allusers;

public slots:
    virtual void slotNewConnection();
    void slotReadClient();
    void slotClientDisconnected();
    void load_chats_for_user(QTcpSocket* clientsock);
};

#endif // TISSUE_SERVER_H
