#include "messagechat.h"

MessageChat::MessageChat(QString Username, QString message){
    this->time=QTime::currentTime();
    this->Username=Username;
    this->message=message;
    this->chatID=0;
    this->typeM="message";
}
MessageChat::MessageChat(){

}
void MessageChat::setID(int ID){
    this->chatID=ID;
}
void MessageChat::settypeM(QString type){
    this->typeM=type;
}
QString MessageChat::get_message(){
    return this->message;
}
QString MessageChat::get_type(){
    return this->typeM;
}
QString MessageChat::get_full_message(){
    return time.toString()+" "+Username+": "+message;
}

QString MessageChat::get_name()
{
    return this->Username;
}
