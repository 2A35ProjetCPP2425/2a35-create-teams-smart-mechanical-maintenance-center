#ifndef SMTPCLIENT_H
#define SMTPCLIENT_H

#include <QString>
#include <QtNetwork/QSslSocket>

class mailer {
public:
    mailer();
    //static int sendEmail(const QString &dist, const QString &obj, const QString &bdy);
    static int sendEmail(const QString &dist, const QString &obj, const QString &bdy, const QStringList &attachments);

private:
    static QString smtpServer;
    static int smtpPort;
    static QString username;
    static QString password;
};
#endif // SMTPCLIENT_H
