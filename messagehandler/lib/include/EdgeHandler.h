#ifndef EDGEHANDLER_HPP
#define EDGEHANDLER_HPP

#include "MqttInterface.h"

namespace MessageHandler::Gateway
{
    class EdgeHandler : public MessageHandler::MqttInterface
    {
        Q_OBJECT

    public:
        explicit EdgeHandler(QString&, quint16&);
        void register_new_edge(const QString &);


    public slots:
        void new_message(QMqttMessage) override;
        void heartbeat_received();
        void newDevicePresence(QString device_id, QString device_serial, uint target, uint event,
                               QString interface, QString m_hardwareAddress);

    signals:
        void newEdgeServiceRule(QString device_id, QString device_serial, uint target, QString interface);
//void on_get_edges();
    };
}

#endif // EDGEHANDLER_HPP
