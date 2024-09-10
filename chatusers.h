#ifndef CHATUSERS_H
#define CHATUSERS_H

#include <QObject>
#include <QWidget>
#include "messagechat.h"

class ChatUsers:public QWidget
{
    Q_OBJECT
public:
    ChatUsers();
    ChatUsers(QString name);
    void push_message(MessageChat* mess);
    void push_user(QString name);

    friend QDataStream &operator<<(QDataStream &out, const ChatUsers &rhs) {
        out << rhs.chat_id << rhs.name << rhs.users;

        QVector<MessageChat> messagesByValue;
        for (const auto& msg : rhs.messages) {
            messagesByValue.append(*msg);
        }
        out << messagesByValue;

        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, ChatUsers &rhs) {
        in >> rhs.chat_id >> rhs.name >> rhs.users;

        QVector<MessageChat> messagesByValue;
        in >> messagesByValue;
        for (const auto& msg : messagesByValue) {
            rhs.messages.append(new MessageChat(msg));
        }

        return in;
    }

    QString getName() const;
    void setName(const QString &newName);
    QSet<QString> getUsers() const;
    void setUsers(const QSet<QString> &newUsers);
    QVector<MessageChat *> getMessages() const;
    void setMessages(const QVector<MessageChat *> &newMessages);
    int getChat_id() const;
    void setChat_id(int newChat_id);

private:
    QString name;
    QSet<QString> users;
    QVector<MessageChat*> messages;
    int chat_id;
};

#endif // CHATUSERS_H
