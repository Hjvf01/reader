#ifndef CONNECTORS_H
#define CONNECTORS_H

#include <vector>
using std::vector;

#include <QtCore/QObject>

template<typename Sender, typename Receiver> class Connector {

    Sender* sender = nullptr;
    Receiver* receiver = nullptr;

public:
    Connector(Sender sender, Receiver receiver);
    ~Connector();

    void connect(vector<auto>, vector<auto>);
    void disconnect(void);
};




template<typename Sender, typename Receiver>
Connector::Connector(Sender sender, Receiver receiver) {
    this->sender = sender;
    this->receiver = receiver;
}

#endif // CONNECTORS_H
