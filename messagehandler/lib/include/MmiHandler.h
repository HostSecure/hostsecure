#ifndef MMIHANDLER_HPP
#define MMIHANDLER_HPP

#include "MqttInterface.h"

namespace MessageHandler::Gateway
{
    class MmiHandler : public MessageHandler::MqttInterface
    {
        Q_OBJECT

    public:
        explicit MmiHandler(QString&, quint16&);
        void get_all_edges();

    public slots:
        void heartbeat_received();
    };
}

#endif // MMIHANDLER_HPP
