#include "tissue_server.h"

Tissue_server::Tissue_server(int Port, QWidget* pwgt /*=0*/): QWidget(pwgt), nextBlockSize(0) {
    load_users();
    load_chats();

    server = new QTcpServer(this);
    if (!server->listen(QHostAddress::Any, Port)) {
        QMessageBox::critical(0, "Server Error", "Unable to start the server: " + server->errorString());
        server->close();
        return;
    }
    connect(server, &QTcpServer::newConnection, this, &Tissue_server::slotNewConnection);
    textedit = new QTextEdit;
    textedit->setReadOnly(true);

    QVBoxLayout *vbx = new QVBoxLayout;
    vbx->addWidget(new QLabel("<H1>Server</H1>"));
    vbx->addWidget(textedit);
    setLayout(vbx);
}

void Tissue_server::slotNewConnection() {
    QTcpSocket* clientsock = server->nextPendingConnection();
    clients.insert(clientsock);

    connect(clientsock, &QTcpSocket::disconnected, this, &Tissue_server::slotClientDisconnected);
    connect(clientsock, &QTcpSocket::readyRead, this, &Tissue_server::slotReadClient);
    ChatUsers *ch = new ChatUsers;
    sendToClient(clientsock, *ch, MessageChat("Server", "Server Response: Connected"));
}

void Tissue_server::slotClientDisconnected() {
    QTcpSocket* clientsock = qobject_cast<QTcpSocket*>(sender());
    if (clientsock) {
        clients.remove(clientsock);
        clientsock->deleteLater();
    }
}

void Tissue_server::slotReadClient() {
    QTcpSocket* clientsock = qobject_cast<QTcpSocket*>(sender());
    if (!clientsock) return;

    QDataStream in(clientsock);
    in.setVersion(QDataStream::Qt_6_7);




    for (;;) {
        if (!nextBlockSize) {
            if (clientsock->bytesAvailable() < sizeof(quint16)) {
                break;
            }
            in >> nextBlockSize;
        }
        if (clientsock->bytesAvailable() < nextBlockSize) {
            break;
        }
        ChatUsers *chat  =new ChatUsers;
        MessageChat *mess=new MessageChat;;
        in >> *chat>>*mess;
        if(!is_chat_name_exists(chat->getName())){
            create_chat(chat);
        }

        //for(auto &c : allchats){
         //   foreach (QTcpSocket* client, clients) {
          //      sendToClient(client, *c->getMessages().last());
          //  }
        //}

        QString strMessage=mess->get_full_message();
        if(mess->get_type()=="CODE_0001_VER_SYS"){
        textedit->append(strMessage);
        }
        else{
        textedit->append(strMessage);
        }
        // Broadcast the message to all clients
        if(mess->get_type()!="CODE_0001_VER_SYS"){
        foreach (QTcpSocket* client, clients) {
            sendToClient(client, *chat, *mess);
        }
        }
        else{
            qDebug()<<"login";
            if(!is_name_exists(mess->get_name())){
                int idx1=get_unique_id();
                append_user(idx1, mess->get_name());
                qDebug()<<"USER "+mess->get_name()+" "+QString::number(idx1)+" has been registered";
                textedit->append("USER "+mess->get_name()+" "+QString::number(idx1)+" has been registered");
                load_chats_for_user(clientsock);
            }
            else{
                qDebug()<<"USER "+mess->get_name()+" has been logged";
                textedit->append("USER "+mess->get_name()+" has been logged");
                load_chats_for_user(clientsock);
            }
        }
        update_chat_file(chat, mess);
        nextBlockSize = 0;
    }
}

void Tissue_server::sendToClient(QTcpSocket* socket, ChatUsers& chatt, const MessageChat& mess) {
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_7);
    out << quint16(0)<<chatt << mess;

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));
    socket->write(arrBlock);
}

void Tissue_server::load_chats() {
    int idx = 0;
    QDir dir("chats");

    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QString fileName;
    while (true) {
        fileName = dir.filePath("chat_" + QString::number(idx) + ".dat");
        if (!QFile::exists(fileName)) {
            break;
        }

        QFile chatfile(fileName);
        if (chatfile.open(QIODevice::ReadOnly)) {
            QDataStream in(&chatfile);
            ChatUsers* chat = new ChatUsers;
            in >> *chat;
            chatfile.close();
            allchats.append(chat);
        } else {
            qWarning("Could not load chat file: %s", qPrintable(fileName));
        }
        idx++;
    }
}



void Tissue_server::append_user(int ID, QString name) {
    int idx = 0;
    QDir dir("users");

    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QString fileName;
    while (true) {
        fileName = dir.filePath("user_" + QString::number(idx) + ".dat");
        if (!QFile::exists(fileName)) {
            break;
        }
        idx++;
    }

    QFile userfile(fileName);
    if (userfile.open(QIODevice::WriteOnly)) {
        QDataStream out(&userfile);
        out << ID << name;
        userfile.close();

        User user;
        user.ID = ID;
        user.name = name;
        allusers.append(user);
    } else {
        qWarning("Could not create user file: %s", qPrintable(fileName));
    }
}


void Tissue_server::create_chat(ChatUsers* chat) {
    int idx = 0;
    QDir dir("chats");

    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QString fileName;
    while (true) {
        fileName = dir.filePath("chat_" + QString::number(idx) + ".dat");
        if (!QFile::exists(fileName)) {
            break;
        }
        idx++;
    }

    QFile chatfile(fileName);
    if (chatfile.open(QIODevice::WriteOnly)) {
        QDataStream out(&chatfile);
        out << *chat;
        chatfile.close();
        allchats.append(chat);
    } else {
        qWarning("Could not create chat file: %s", qPrintable(fileName));
    }
}


void Tissue_server::load_users() {
    int idx = 0;
    QDir dir("users");

    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QString fileName;
    while (true) {
        fileName = dir.filePath("user_" + QString::number(idx) + ".dat");
        if (!QFile::exists(fileName)) {
            break;
        }

        QFile userfile(fileName);
        if (userfile.open(QIODevice::ReadOnly)) {
            QDataStream in(&userfile);
            int ID;
            QString name;
            in >> ID >> name;
            userfile.close();

            User user;
            user.ID = ID;
            user.name = name;
            allusers.append(user);
        } else {
            qWarning("Could not load user file: %s", qPrintable(fileName));
        }
        idx++;
    }
}

bool Tissue_server::is_name_exists(QString name)
{
    bool is=0;
    for (auto &u :allusers){
        if (u.name==name) return 1;
    }
    return is;
}

int Tissue_server::get_unique_id()
{
    int idx=0;
    for (auto &u :allusers){
        if (u.ID==idx) idx++;
        else return idx;
    }
    return idx;
}

bool Tissue_server::is_chat_name_exists(QString name)
{
    bool is=0;
    for (auto &u :allchats){
        if (u->getName()==name) return 1;
    }
    return is;
}

int Tissue_server::get_unique_chat_id()
{
    int idx=0;
    for (auto &u :allchats){
        if (u->getChat_id()==idx) idx++;
        else return idx;
    }
    return idx;
}


void Tissue_server::load_chats_for_user(QTcpSocket* clientsock) {
    // Here, you can load chats from a file specific to the user or load all chats
    // For example, you might have a function like load_user_chats(QString username) that loads chats for a specific user

    // For demonstration purposes, let's say you load all chats for now
    for (auto &chat : allchats) {
        for(auto &mess : chat->getMessages()){
         sendToClient(clientsock, *chat, *mess);
        }
    }

}


void Tissue_server::update_chat_file(ChatUsers* chat, MessageChat* mess) {
    QString fileName = "chats/" + QString::number(chat->getChat_id()) + ".dat";
    QFile chatfile(fileName);
    if (chatfile.open(QIODevice::ReadWrite)) {
        QDataStream in(&chatfile);
        ChatUsers tempChat;
        in >> tempChat;
        tempChat.push_message(mess);
        chatfile.seek(0);
        QDataStream out(&chatfile);
        out << tempChat;
        chatfile.close();
    } else {
        qWarning("Could not update chat file: %s", qPrintable(fileName));
    }
}
