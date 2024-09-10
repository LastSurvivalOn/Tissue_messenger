#ifndef USERACCOUNT_H
#define USERACCOUNT_H

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
#include <QLineEdit>
#include <QPushButton>
#include <QInputDialog>
#include <QListWidget>
#include <QHBoxLayout>
#include "messagechat.h"
#include "chatusers.h"
#include "tissue_server.h"
class UserAccount:public QWidget
{
    Q_OBJECT
public:
    UserAccount(const QString host, int port, QWidget *pwgt = 0);
    void closeEvent(QCloseEvent *event) override;
    bool login();
    void openChat(QString name);
private:
    QTcpSocket* socket;
    QTextEdit* textedit;
    QLineEdit* lineedit;
    quint16 nextBlockSize;
    QLabel *label;
    QString Username;
    bool is_logged;
    QVector<ChatUsers*> chats;
    ChatUsers *current;

    QListWidget *list;
private slots:
    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError);
    void slotSendToServer();
    void slotConnected();
    void slotCreateChat();
    void slotItemClicked(QListWidgetItem *item);
};

#endif // USERACCOUNT_H
