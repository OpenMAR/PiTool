#ifndef RPC_SERVER_H
#define RPC_SERVER_H
#include <QWebSocketServer>
#include <QWebSocket>

typedef bool(__stdcall *RpcServer_Callback)(QWebSocket *,QString,QJsonObject param);

class RpcServer:QObject
{
    Q_OBJECT
public:
    explicit RpcServer(QObject *parent = 0);
    ~RpcServer();
    void setCallback(RpcServer_Callback callback);
    void sendMessage(QString process,QString param);
    void sendCallback(QWebSocket *pClient,QString process,QString result,QString value);
    void sendCallback(QWebSocket *pClient,QString process,bool result,QString message="");

private slots:
    void newConnection();
    void closed();
    void textMessageReceived(QString message);
    void processBinaryMessage(QByteArray message);
    void socketDisconnected();

private:
    void processMessage(QWebSocket *pSocket,QString message);
    QWebSocketServer *m_pServer;
    QList<QWebSocket*> mClientList;
    RpcServer_Callback mCallback;
};

#endif // MY_APPLICATION_H
