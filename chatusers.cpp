#include "chatusers.h"

ChatUsers::ChatUsers() {}

ChatUsers::ChatUsers(QString name)
{
    this->name=name;
}

void ChatUsers::push_message(MessageChat *mess)
{
    messages.push_back(mess);
}

void ChatUsers::push_user(QString name){
    users.insert(name);
}

QString ChatUsers::getName() const
{
    return name;
}

void ChatUsers::setName(const QString &newName)
{
    name = newName;
}

QSet<QString> ChatUsers::getUsers() const
{
    return users;
}

void ChatUsers::setUsers(const QSet<QString> &newUsers)
{
    users = newUsers;
}

QVector<MessageChat *> ChatUsers::getMessages() const
{
    return messages;
}

void ChatUsers::setMessages(const QVector<MessageChat *> &newMessages)
{
    messages = newMessages;
}

int ChatUsers::getChat_id() const
{
    return chat_id;
}

void ChatUsers::setChat_id(int newChat_id)
{
    chat_id = newChat_id;
}


