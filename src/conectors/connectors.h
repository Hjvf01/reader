#ifndef CONNECTORS_H
#define CONNECTORS_H

#include <vector>
using std::vector;
#include <cassert>

#include <QtCore/QObject>
#include <QtCore/QDebug>


using Index = unsigned int;


template <typename Sender, typename Receiver> class BaseConnector {
public:
    virtual void disconnect(void) const = 0;
};


template <typename Sender, typename Receiver>
class One2One : public BaseConnector<Sender, Receiver> {

    Sender* sender = nullptr;
    Receiver* receiver = nullptr;

public:
    One2One() {}

    One2One(Sender* sender, Receiver* receiver) {
        this->sender = sender;
        this->receiver = receiver;
    }

    void set(Sender* sender, Receiver* receiver) {
        if(this->sender == nullptr && this->receiver == nullptr) {
            this->sender = sender;
            this->receiver = receiver;
        }
    }

    void connect(const vector<auto> _signals, const vector<auto> _slots) const {
        assert(_signals.size() == _slots.size());

        Index len = _signals.size();
        for(Index i = 0; i < len; i++) {
            QObject::connect(sender, _signals[i], receiver, _slots[i]);
        }
    }

    void disconnect(void) const override {
        QObject::disconnect(sender, 0, receiver, 0);
    }

    ~One2One() {
        disconnect();
    }

};


template <typename Sender, typename Receiver>
class One2Many : public BaseConnector<Sender, Receiver> {

    Sender* sender = nullptr;
    vector<Receiver*> receivers;

public:
    One2Many() {}

    One2Many(Sender* sender, vector<Receiver*> receivers) {
        this->sender = sender;
        for(Receiver* receiver: receivers)
            this->receivers.push_back(receiver);
    }

    void set(Sender* sender, vector<Receiver*> receivers) {
        if(sender == nullptr && this->receivers.size() == 0) {
            this->sender = sender;
            for(Receiver* receiver: receivers)
                this->receivers.push_back(receivers);
        }
    }

    void connect(const vector<auto> _signals, const vector<auto> _slots) {
        assert(_signals.size() == _slots.size());
        assert(receivers.size() == _signals.size());

        Index len = _signals.size();
        for(Index i = 0; i < len; i++) {
            QObject::connect(sender, _signals[i], receivers[i], _slots[i]);
        }
    }

    void disconnect() const override {
        for(Receiver* receiver: receivers)
            QObject::disconnect(sender, nullptr, receiver, nullptr);
    }

    ~One2Many() {
        disconnect();
    }

};


template <typename Sender, typename Receiver>
class Many2Many : public BaseConnector<Sender, Receiver> {

    vector<Sender*> senders;
    vector<Receiver*> receivers;

public:
    Many2Many() {}

    Many2Many(vector<Sender*> senders, vector<Receiver*> receivers) {
        assert(senders.size() == receivers.size());

        for(Sender* sender: senders) this->senders.push_back(sender);
        for(Receiver* receiver: receivers) this->receivers.push_back(receiver);

        assert(this->senders.size() == this->receivers.size());
    }

    void set(vector<Sender*> senders, vector<Receiver*> receivers) {
        assert(senders.size() == receivers.size());

        if(this->senders.size() == 0 && this->receivers.size() == 0) {
            Index len = senders.size();
            for(Index i = 0; i < len; i++) {
                this->senders.push_back(senders[i]);
                this->receivers.push_back(receivers[i]);
            }
        }
    }

    void connect(const vector<auto> _signals, const vector<auto> _slots) {
        assert(_signals.size() == senders.size());
        assert(_slots.size() == receivers.size());

        Index len = _signals.size();
        for(Index i = 0; i < len; i++) {
            QObject::connect(senders[i], _signals[i], receivers[i], _slots[i]);
        }
    }

    void disconnect() const override {
        Index len = senders.size();
        for(Index i = 0; i < len; i++)
            QObject::disconnect(senders[i], nullptr, receivers[i], nullptr);
    }

    ~Many2Many() {
        disconnect();
    }

};


template <typename Sender, typename Receiver>
class Many2One : public BaseConnector<Sender, Receiver> {

    vector<Sender*> senders;
    Receiver* receiver = nullptr;

public:
    Many2One() {}

    Many2One(vector<Sender*> senders, Receiver* receiver) {
        this->receiver = receiver;

        for(Sender* sender: senders) this->senders.push_back(sender);
    }

    void set(vector<Sender*> senders, Receiver* receiver) {
        if(this->receiver == nullptr && this->senders.size() == 0) {
            this->receiver = receiver;
            for(Sender* sender: senders) this->senders.push_back(sender);
        }
    }

    void connect(const vector<auto> _signals, const vector<auto> _slots) {
        assert(_signals.size() == _slots.size());
        assert(_signals.size() == senders.size());

        Index len = _signals.size();
        for(Index i = 0; i < len; i++)
            QObject::connect(senders[i], _signals[i], receiver, _slots[i]);
    }

    void disconnect() const override {
        for(Sender* sender: senders)
            QObject::disconnect(sender, nullptr, receiver, nullptr);
    }


    ~Many2One() {
        disconnect();
    }

};

#endif // CONNECTORS_H
