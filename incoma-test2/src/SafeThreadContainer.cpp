#include "SafeThreadContainer.h"

void
SafeThreadContainer::writeData(const QString& name, const QString& value)
{
    QWriteLocker locker(&this->guard);
    this->data[name] = value;
}

QString
SafeThreadContainer::readData(const QString& name) const
{
    QReadLocker locker(&this->guard);
    auto it = this->data.find(name);
    if (it != this->data.end()) {
        return it->second;
    }

    return QString();
}
