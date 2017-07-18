#include "network.h"


BaseWebWorker::BaseWebWorker() {
    config = QSslConfiguration::defaultConfiguration();
    config.setProtocol(QSsl::TlsV1_2);
    request.setSslConfiguration(config);
}

BaseWebWorker::~BaseWebWorker() {}


bool BaseWebWorker::jsonHasError(const QJsonParseError& error) const {
    if(error.error == JsonError::NoError)
        return false;
    else {
        emit errorSignal(error.errorString());
        return true;
    }
}


bool BaseWebWorker::replyHasError(QNetworkReply* reply) const {
    if(reply->error() == NetworkError::NoError)
        return false;
     else {
        emit errorSignal(reply->errorString());
        return true;
     }
}
