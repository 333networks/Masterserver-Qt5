#include "../beaconserver.h"

void BeaconServer::onUdpRead()
{
    while ( _udpSocket.hasPendingDatagrams() )
    {
        // get sender and payload
        QNetworkDatagram    datagram        = _udpSocket.receiveDatagram();
        QString             senderAddress   = QHostAddress( datagram.senderAddress().toIPv4Address() ).toString();
        int                 senderPort      = datagram.senderPort();
        QString             receiveBuffer   = datagram.data();
        receiveBuffer = receiveBuffer.toLatin1();

        // shorthand label
        QString senderAddressLabel = QStringLiteral("%1:%2").arg(senderAddress, QString::number(senderPort));
        _coreObject->Log.logEvent("udp", QStringLiteral("%1 sent '%2'").arg(senderAddressLabel, receiveBuffer ) );

        // ignore empty data packets (query port forwarded to a game port)
        if (receiveBuffer.length() <= 0)
            continue;

        // determine protocol and response based on the first character (backslash, byte value, ... )
        unsigned short protocol_chooser = receiveBuffer.at(0).unicode();
        if (protocol_chooser != 92)
            continue;

        // Protocol is GameSpy v0 (\key\value format) used by Unreal/UT, Postal 2, Rune, Deus Ex, Serious Sam, others
        processHeartbeatGamespy0(datagram, senderAddress, senderPort, receiveBuffer);

    } // while pending datagrams
}
