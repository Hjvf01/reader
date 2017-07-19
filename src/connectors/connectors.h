#ifndef CONNECTORS_H
#define CONNECTORS_H

#include <vector>
using std::vector;
#include <cassert>

#include <QtCore/QObject>
#include <QtCore/QDebug>
#include <Qt>


template <typename Sender, typename Type, typename... Rest>
using Signals = vector<void (Sender::*)(Type, Rest...)>;

template <typename Receiver, typename Type, typename... Rest>
using Slots = vector<void (Receiver::*)(Type, Rest...)>;


using Connection = Qt::ConnectionType;


using Index = unsigned int;


template <typename Sender, typename Receiver> class BaseConnector {
public:

    template <typename Type, typename... Rest>
    using _Signals = Signals<Sender, Type, Rest...>;

    template <typename Type, typename... Rest>
    using _Slots = Slots<Receiver, Type, Rest...>;

    using VoidSignals = vector<void (Sender::*)(void)>;
    using VoidSlots = vector<void (Receiver::*)(void)>;




    virtual void disconnect(void) const = 0;

    virtual void connect(
        const vector<void (Sender::*)(void)>&,
        const vector<void (Receiver::*)(void)>&,
        Connection type = Qt::AutoConnection
    ) const = 0;

    virtual ~BaseConnector() {}
};


template <typename Sender, typename Receiver>
class One2One : public BaseConnector<Sender, Receiver> {
    Sender* sender;
    Receiver* receiver;

public:
    One2One() {
        sender = nullptr;
        receiver = nullptr;
    }

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


    template <typename Type, typename... Rest>  void connect(
            const vector<void (Sender::*)(Type, Rest...)>& _signals,
            const vector<void (Receiver::*)(Type, Rest...)>& _slots,
            Connection type = Qt::AutoConnection
    ) const {
        assert(_signals.size() == _slots.size());

        Index len = _signals.size();
        for(Index i = 0; i < len; i++)
            QObject::connect(sender, _signals[i], receiver, _slots[i], type);
    }


    void connect(
            const vector<void (Sender::*)(void)>& _signals,
            const vector<void (Receiver::*)(void)>& _slots,
            Connection type = Qt::AutoConnection
    ) const override {
        assert(_signals.size() == _slots.size());

        size_t len = _signals.size();
        for(size_t i = 0; i < len; i++)
            QObject::connect(sender, _signals[i], receiver, _slots[i], type);
    }


    void disconnect(void) const override {
        QObject::disconnect(sender, 0, receiver, 0);
    }


    ~One2One() override {
        //disconnect();
    }
};


template <typename Sender, typename Receiver>
class One2Many : public BaseConnector<Sender, Receiver> {
    Sender* sender;
    vector<Receiver*> receivers;

public:
    One2Many() { sender = nullptr; }


    One2Many(Sender* sender, const vector<Receiver*>& receivers) {
        this->sender = sender;
        for(Receiver* receiver: receivers)
            this->receivers.push_back(receiver);
    }


    void set(Sender* sender, const vector<Receiver*>& receivers) {
        if(sender == nullptr && this->receivers.size() == 0) {
            this->sender = sender;
            for(Receiver* receiver: receivers)
                this->receivers.push_back(receiver);
        }
    }


    template <typename Type, typename... Rest> void connect(
            const vector<void (Sender::*)(Type, Rest...)>& _signals,
            const vector<void (Receiver::*)(Type, Rest...)>& _slots,
            Qt::ConnectionType type = Qt::AutoConnection
    ) const {
        assert(_signals.size() == _slots.size());
        assert(receivers.size() == _signals.size());

        Index len = _signals.size();
        for(Index i = 0; i < len; i++)
            QObject::connect(
                sender, _signals[i], receivers[i], _slots[i], type
            );
    }


    void connect(
            const vector<void (Sender::*)(void)>& _signals,
            const vector<void (Receiver::*)(void)>& _slots,
            Qt::ConnectionType type = Qt::AutoConnection
    ) const override {
        assert(_signals.size() == _slots.size());
        assert(receivers.size() == _signals.size());

        Index len = _signals.size();
        for(Index i = 0; i < len; i++)
            QObject::connect(
                sender, _signals[i], receivers[i], _slots[i], type
            );
    }


    void disconnect() const override {
        for(Receiver* receiver: receivers)
            QObject::disconnect(sender, nullptr, receiver, nullptr);
    }


    ~One2Many() override {
        //disconnect();
    }
};


template <typename Sender, typename Receiver>
class Many2Many : public BaseConnector<Sender, Receiver> {
    using CRSenders = const vector<Sender*>;
    using CRReceivers = const vector<Receiver*>;

    vector<Sender*> senders;
    vector<Receiver*> receivers;

public:
    Many2Many() {}


    Many2Many(CRSenders senders, CRReceivers receivers) {
        assert(senders.size() == receivers.size());

        for(Sender* sender: senders)
            this->senders.push_back(sender);
        for(Receiver* receiver: receivers)
            this->receivers.push_back(receiver);

        assert(this->senders.size() == this->receivers.size());
    }


    void set(CRSenders senders, CRReceivers receivers) {
        assert(senders.size() == receivers.size());

        if(this->senders.size() == 0 && this->receivers.size() == 0) {
            Index len = senders.size();
            for(Index i = 0; i < len; i++) {
                this->senders.push_back(senders[i]);
                this->receivers.push_back(receivers[i]);
            }
        }
    }


    template <typename Type, typename... Rest> void connect(
            const vector<void (Sender::*)(Type, Rest...)>& _signals,
            const vector<void (Receiver::*)(Type, Rest...)>& _slots,
            Qt::ConnectionType type = Qt::AutoConnection
    ) const {
        assert(_signals.size() == senders.size());
        assert(_slots.size() == receivers.size());

        Index len = _signals.size();
        for(Index i = 0; i < len; i++) {
            QObject::connect(
                senders[i], _signals[i], receivers[i], _slots[i], type
            );
        }
    }


    void connect(
            const vector<void (Sender::*)(void)>& _signals,
            const vector<void (Receiver::*)(void)>& _slots,
            Qt::ConnectionType type = Qt::AutoConnection
    ) const override {
        assert(_signals.size() == senders.size());
        assert(_slots.size() == receivers.size());

        Index len = _signals.size();
        for(Index i = 0; i < len; i++) {
            QObject::connect(
                senders[i], _signals[i], receivers[i], _slots[i], type
            );
        }
    }


    void disconnect() const override {
        Index len = senders.size();
        for(Index i = 0; i < len; i++)
            QObject::disconnect(senders[i], nullptr, receivers[i], nullptr);
    }

    ~Many2Many() override {
        //disconnect();
    }
};


template <typename Sender, typename Receiver>
class Many2One : public BaseConnector<Sender, Receiver> {
    vector<Sender*> senders;
    Receiver* receiver;

public:
    Many2One() {
        receiver = nullptr;
    }


    Many2One(const vector<Sender*>& senders, Receiver* receiver) {
        this->receiver = receiver;

        for(Sender* sender: senders)
            this->senders.push_back(sender);
    }


    void set(const vector<Sender*>& senders, Receiver* receiver) {
        if(this->receiver == nullptr && this->senders.size() == 0) {
            this->receiver = receiver;

            for(Sender* sender: senders)
                this->senders.push_back(sender);

            assert(this->senders.size() == senders.size());
        }
        else
            assert(this->senders.size() == 0);
    }


    template<typename Type, typename... Rest> void connect(
            const vector<void (Sender::*)(Type, Rest...)>& _signals,
            const vector<void (Receiver::*)(Type, Rest...)>& _slots,
            Qt::ConnectionType type = Qt::AutoConnection
    ) const {
        assert(_signals.size() == _slots.size());
        assert(_signals.size() == senders.size());

        Index len = _signals.size();
        for(Index i = 0; i < len; i++) {
            QObject::connect(
                senders[i], _signals[i], receiver, _slots[i], type
            );
        }
    }


    void connect(
            const vector<void (Sender::*)(void)>& _signals,
            const vector<void (Receiver::*)(void)>& _slots,
            Qt::ConnectionType type = Qt::AutoConnection
    ) const override {
        assert(_signals.size() == _slots.size());
        assert(_signals.size() == senders.size());

        Index len = _signals.size();
        for(Index i = 0; i < len; i++)
            QObject::connect(
                senders[i], _signals[i], receiver, _slots[i], type
            );
    }


    void disconnect() const override {
        for(Sender* sender: senders)
            QObject::disconnect(sender, nullptr, receiver, nullptr);
    }


    ~Many2One() override {
        //disconnect();
    }
};

#endif // CONNECTORS_H
