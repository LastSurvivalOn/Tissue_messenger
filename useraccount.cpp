#include "useraccount.h"

UserAccount::UserAccount(const QString host, int port, QWidget* pwgt):QWidget(pwgt), nextBlockSize(0), current(nullptr), list(new QListWidget), label(new QLabel("<H1>Client</H1>")) {
    socket=new QTcpSocket(this);

    socket->connectToHost(host, port);
    connect(socket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(socket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    //connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred), this, &UserAccount::slotError);
    textedit=new QTextEdit;
    lineedit=new QLineEdit;
    //connect(socket, &Tissue_server::newChatCreated, this, &UserAccount::handleNewChatCreated);

    textedit->setReadOnly(1);
    QPushButton* cmd = new QPushButton("&Send");

    connect(cmd, SIGNAL(clicked()), SLOT(slotSendToServer()));
    connect(lineedit, SIGNAL(returnPressed()), this, SLOT(slotSendToServer()));

    QVBoxLayout* vbx = new QVBoxLayout;
    QHBoxLayout* hbx = new QHBoxLayout;

    QPushButton* create = new QPushButton("Create New");
    connect(create, SIGNAL(clicked()), SLOT(slotCreateChat()));
    connect(list, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(slotItemClicked(QListWidgetItem*)));
    hbx->addWidget(list);
    hbx->addLayout(vbx);
    vbx->addWidget(label);
    vbx->addWidget(create);
    vbx->addWidget(textedit);
    vbx->addWidget(lineedit);
    vbx->addWidget(cmd);
    setLayout(hbx);
    login();
}

void UserAccount::slotReadyRead(){
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_7);
    for(;;){
        if(!nextBlockSize){
            if(socket->bytesAvailable()<sizeof(quint16)){
                break;
            }
            in>>nextBlockSize;
        }
        if(socket->bytesAvailable()<nextBlockSize){
            break;
        }
        ChatUsers *chat=new ChatUsers;
        MessageChat mess;
        in>>*chat>>mess;
        for(auto &a :chat->getUsers()) {if(a==Username&&a!=mess.get_name()) {

                QList<QListWidgetItem*> existingItems = list->findItems(chat->getName(), Qt::MatchExactly);
                if (!existingItems.isEmpty()) {
                    //QMessageBox::warning(this, tr("Duplicate Chat Name"), tr("A chat with the same name already exists."));
                    //return; // Exit without creating the chat
                }
                else{
                    chats.append(chat);
                    qDebug()<<Username<<chats.size();
                    list->addItem(chat->getName());
                    current=chats.last();

                }
                //for(auto &m: chat.getMessages()) {
                    textedit->append(mess.get_full_message());
                //}
            }
        }
        nextBlockSize=0;
    }
}



void UserAccount::slotError(QAbstractSocket::SocketError err){
    QString error="Error"+(err==QAbstractSocket::HostNotFoundError?
                                "The host not found":err==QAbstractSocket::RemoteHostClosedError?
                                "The remote host is closed": err==QAbstractSocket::ConnectionRefusedError?
                                "The connection was refused":QString(socket->errorString()));
    textedit->append(error);
}

void UserAccount::slotSendToServer(){
    if(lineedit->text()!=""&&current!=nullptr){
        QByteArray arrBlock;
        QDataStream out(&arrBlock, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_6_7);
        MessageChat *mess = new MessageChat(Username, lineedit->text());
        current->push_message(mess);
        out<<quint16(0)<<*current<<*mess;

        out.device()->seek(0);
        out<<quint16(arrBlock.size()-sizeof(quint16));
        textedit->append(mess->get_full_message());
        socket->write(arrBlock);
        lineedit->clear();
    }
}

void UserAccount::slotConnected(){
    //textedit->append("Receieved the connected() signal");

}

void UserAccount::slotCreateChat()
{
    QString chatname = QInputDialog::getText(this, tr("Enter chatname"), tr("Chatname:"), QLineEdit::Normal);
    QString username = QInputDialog::getText(this, tr("Enter username"), tr("Username:"), QLineEdit::Normal);
    ChatUsers *chat = new ChatUsers(chatname);
    chat->push_user(this->Username);
    chat->push_user(username);
    chats.append(chat);

    qDebug()<<Username<<chats.size();

    list->addItem(chatname);
    current=chats.last();
}

void UserAccount::openChat(QString name)
{
    for(auto &c : chats){
        if(c->getName()==name){
            current=c;
            textedit->clear();
            for(auto &m :c->getMessages()){
                textedit->append(m->get_full_message());
            }
        }
    }
}

void UserAccount::closeEvent(QCloseEvent *event) {
    delete this->socket;
    is_logged=0;
    QWidget::closeEvent(event);
}

bool UserAccount::login(){
    Username = QInputDialog::getText(this, tr("Enter Username"), tr("Username:"), QLineEdit::Normal);
    if(!Username.isEmpty()){
        QByteArray arrBlock;
        QDataStream out(&arrBlock, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_6_7);
        MessageChat mess(Username, " has logged in system!");
        mess.settypeM("CODE_0001_VER_SYS");
        ChatUsers chat;
        out<<quint16(0)<<chat<<mess;

        out.device()->seek(0);
        out<<quint16(arrBlock.size()-sizeof(quint16));

        socket->write(arrBlock);
        label->setText("<H1>" + Username + "</H1>");
        return 1;
    }
    //this->setWindowTitle("Tissue Client: "+Username);
    is_logged=1;

    //qDebug() << "Username set and label updated:" << Username;
    return 0;
}

void UserAccount::slotItemClicked(QListWidgetItem *item) {
    if (item) {
        QString selectedItem = item->text();
        openChat(selectedItem);
    }
}






