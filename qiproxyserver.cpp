#include "qiproxyserver.h"
#include "qipipe.h"

QiProxyServer::QiProxyServer(QObject *parent) :
    QTcpServer(parent){
}

void QiProxyServer::incomingConnection(int socketId){
    //QTcpSocket *socket = new QTcpSocket();
    //socket->setSocketDescriptor(socketId);
    QiPipe *pipe = addPipe(socketId);

    connect(pipe,SIGNAL(connected(PipeData_ptr)),SLOT(onPipeConnected(PipeData_ptr)));
    connect(pipe,SIGNAL(connected(PipeData_ptr)),SIGNAL(newPipe(PipeData_ptr)));
    connect(pipe,SIGNAL(completed(PipeData_ptr)),SLOT(onPipeComplete(PipeData_ptr)));
    connect(pipe,SIGNAL(completed(PipeData_ptr)),SIGNAL(pipeUpdate(PipeData_ptr)));
    connect(pipe,SIGNAL(error(PipeData_ptr)),SLOT(onPipeError(PipeData_ptr)));
    connect(pipe,SIGNAL(error(PipeData_ptr)),SIGNAL(pipeUpdate(PipeData_ptr)));

    pipe->start();


}
void QiProxyServer::onPipeConnected(PipeData_ptr p){
    // do nothing?
    Q_UNUSED(p);
}
void QiProxyServer::onPipeComplete(PipeData_ptr p){
    removePipe(p->socketId);
}
void QiProxyServer::onPipeError(PipeData_ptr p){
    removePipe(p->socketId);
}

QiPipe* QiProxyServer::addPipe(int socketDescriptor){
    QiPipe *p = new QiPipe(socketDescriptor);//delete in removePipe(int)
    pipes.value(socketDescriptor,p);
    return p;
}

bool QiProxyServer::removePipe(int socketId){
    if(pipes.contains(socketId)){
        QiPipe *p = pipes.value(socketId);
        pipes.remove(socketId);
        p->deleteLater();
    }
    return false;
}
