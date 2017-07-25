#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H


#include <exception>
using std::exception;
#include <string>
using std::string;
#include <ostream>
using std::ostream;


class DocumentWasntOpened : public exception {
    string msg;

public:
    DocumentWasntOpened(const string& _msg) : exception(), msg(_msg) {}
    ~DocumentWasntOpened() override {}

    friend ostream& operator << (ostream& os, const DocumentWasntOpened& doc) {
        return os << doc.msg;
    }
};



class DPIWasntInit : public exception {
    string msg;

public:
    DPIWasntInit(const string& _msg) : exception(), msg(_msg) {}
    ~DPIWasntInit() override {}

    friend ostream& operator << (ostream& os, const DPIWasntInit& error) {
        return os << error.msg;
    }
};


class PageOutOfRange : public exception {
    string msg;

public:
    PageOutOfRange(const string& _msg) : exception(), msg(_msg) {}
    ~PageOutOfRange() override {}

    friend ostream& operator << (ostream& os, const PageOutOfRange& error) {
        return os << error.msg;
    }
};

#endif // EXCEPTIONS_H
