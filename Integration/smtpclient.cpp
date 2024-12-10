#include "smtpclient.h"
#include <QDebug>
#include <QByteArray>
#include <QFileInfo>
#include <QFile>
QString mailer::smtpServer = "smtp.gmail.com";
//QString mailer::smtpServer = "smtp.cme.tn";
int mailer::smtpPort = 465; // Port sécurisé pour SSL
QString mailer::username = "mmymmy512@gmail.com"; // Votre adresse email
QString mailer::password = "mmymmy1925*";  // Mot de passe d'application
/*QString mailer::username = "yassine.mlaouah2005@gmail.com"; // Votre adresse email
QString mailer::password = "1925Bardo2023+1925@";*/
/*QString mailer::username = "yassine.mlaouah@cme.tn"; // Votre adresse email
QString mailer::password = "Bardo2023+";*/
mailer::mailer() {}

/*int mailer::sendEmail(const QString &dist, const QString &obj, const QString &bdy) {
    qDebug() << "SSL Library Info:";
    qDebug() << "sslLibraryBuildVersionString: " << QSslSocket::sslLibraryBuildVersionString();
    qDebug() << "sslLibraryVersionNumber: " << QSslSocket::sslLibraryVersionNumber();
    qDebug() << "supportsSsl: " << QSslSocket::supportsSsl();

    QSslSocket socket;

    // Connect to the SMTP server
    socket.connectToHostEncrypted(smtpServer, smtpPort);
    if (!socket.waitForConnected()) {
        qDebug() << "Error connecting to the server:" << socket.errorString();
        return -1;
    }

    if (!socket.waitForReadyRead()) {
        qDebug() << "Error waiting for server response:" << socket.errorString();
        return -1;
    }

    qDebug() << "Connected to the SMTP server.";

    // Send commands to the SMTP server
    auto sendCommand = [&](const QString &command) -> bool {
        socket.write(command.toUtf8());
        if (!socket.waitForBytesWritten() || !socket.waitForReadyRead()) {
            qDebug() << "Error during command:" << command << ", Error:" << socket.errorString();
            return false;
        }
        return true;
    };

    if (!sendCommand("HELO localhost\r\n")) return -1;
    if (!sendCommand("AUTH LOGIN\r\n")) return -1;
    if (!sendCommand(QByteArray(username.toUtf8()).toBase64() + "\r\n")) return -1;
    if (!sendCommand(QByteArray(password.toUtf8()).toBase64() + "\r\n")) return -1;
    if (!sendCommand("MAIL FROM:<" + username + ">\r\n")) return -1;
    if (!sendCommand("RCPT TO:<" + dist + ">\r\n")) return -1;
    if (!sendCommand("DATA\r\n")) return -1;

    // Send the email content
    QString emailContent =
        "From: " + username + "\r\n" +
        "To: " + dist + "\r\n" +
        "Subject: " + obj + "\r\n" +
        "\r\n" +
        bdy + "\r\n.\r\n";

    if (!sendCommand(emailContent)) return -1;

    // Quit the session
    sendCommand("QUIT\r\n");
    socket.close();

    qDebug() << "Email sent successfully.";
    return 0;
}*/
int mailer::sendEmail(const QString &dist, const QString &obj, const QString &bdy, const QStringList &attachments) {
    qDebug() << "SSL Library Info:";
    qDebug() << "sslLibraryBuildVersionString: " << QSslSocket::sslLibraryBuildVersionString();
    qDebug() << "sslLibraryVersionNumber: " << QSslSocket::sslLibraryVersionNumber();
    qDebug() << "supportsSsl: " << QSslSocket::supportsSsl();

    QSslSocket socket;

    // Connect to the SMTP server
    socket.connectToHostEncrypted(smtpServer, smtpPort);
    if (!socket.waitForConnected()) {
        qDebug() << "Error connecting to the server:" << socket.errorString();
        return -1;
    }

    if (!socket.waitForReadyRead()) {
        qDebug() << "Error waiting for server response:" << socket.errorString();
        return -1;
    }

    qDebug() << "Connected to the SMTP server.";

    // Send commands to the SMTP server
    auto sendCommand = [&](const QString &command) -> bool {
        socket.write(command.toUtf8());
        if (!socket.waitForBytesWritten() || !socket.waitForReadyRead()) {
            qDebug() << "Error during command:" << command << ", Error:" << socket.errorString();
            return false;
        }
        return true;
    };

    if (!sendCommand("HELO localhost\r\n")) return -1;
    if (!sendCommand("AUTH LOGIN\r\n")) return -1;
    if (!sendCommand(QByteArray(username.toUtf8()).toBase64() + "\r\n")) return -1;
    if (!sendCommand(QByteArray(password.toUtf8()).toBase64() + "\r\n")) return -1;
    if (!sendCommand("MAIL FROM:<" + username + ">\r\n")) return -1;
    if (!sendCommand("RCPT TO:<" + dist + ">\r\n")) return -1;
    if (!sendCommand("DATA\r\n")) return -1;

    // Define MIME boundary
    QString boundary = "----=_Boundary_12345";

    // Start the email content with headers
    QString emailContent =
        "From: " + username + "\r\n" +
        "To: " + dist + "\r\n" +
        "Subject: " + obj + "\r\n" +
        "MIME-Version: 1.0\r\n" +
        "Content-Type: multipart/mixed; boundary=\"" + boundary + "\"\r\n\r\n";

    // Add the main email body
    emailContent += "--" + boundary + "\r\n";
    emailContent += "Content-Type: text/plain; charset=\"utf-8\"\r\n";
    emailContent += "Content-Transfer-Encoding: 7bit\r\n\r\n";
    emailContent += bdy + "\r\n\r\n";

    // Add attachments
    for (const QString &filePath : attachments) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray fileData = file.readAll().toBase64();
            QString fileName = QFileInfo(filePath).fileName();

            emailContent += "--" + boundary + "\r\n";
            emailContent += "Content-Type: application/octet-stream\r\n";
            emailContent += "Content-Disposition: attachment; filename=\"" + fileName + "\"\r\n";
            emailContent += "Content-Transfer-Encoding: base64\r\n\r\n";
            emailContent += fileData + "\r\n\r\n";
        } else {
            qDebug() << "Error opening file for attachment:" << filePath;
        }
    }

    // End the MIME message
    emailContent += "--" + boundary + "--\r\n.\r\n";

    // Send the email content
    if (!sendCommand(emailContent)) return -1;

    // Quit the session
    sendCommand("QUIT\r\n");
    socket.close();

    qDebug() << "Email sent successfully with attachments.";
    return 0;
}
