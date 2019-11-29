#pragma once

#include <unordered_map>
#include <QReadWriteLock>
#include <QString>
#include <QHash>

namespace std {

template<>
struct hash<QString> {
    size_t operator()(const QString& value) const {
        return static_cast<size_t>(qHash(value));
    }
};

}

class SafeThreadContainer {
public:
    void writeData(const QString& name, const QString& value);
    QString readData(const QString& name) const noexcept;
private:
    mutable QReadWriteLock guard;
    std::unordered_map<QString, QString> data;
};
