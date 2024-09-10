#ifndef MESSAGECHAT_H
#define MESSAGECHAT_H
#include <QTime>
#include <QString>
class MessageChat
{
public:
    MessageChat(QString Username, QString message);
    MessageChat();
    void setID(int ID);
    void settypeM(QString type);
    QString get_message();
    QString get_type();
    QString get_full_message();
    QString get_name();

    friend QDataStream &operator<<(QDataStream &out, const MessageChat &rhs) {
        out<<rhs.time<<rhs.Username<<rhs.message<<rhs.typeM<<rhs.chatID;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, MessageChat &rhs) {
        in>>rhs.time>>rhs.Username>>rhs.message>>rhs.typeM>>rhs.chatID;
        return in;
    }

private:
    QTime time;
    QString Username;
    QString message;
    QString typeM;
    int chatID;
};

#endif // MESSAGECHAT_H
