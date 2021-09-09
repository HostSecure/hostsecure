#pragma once
#include <QObject>


class DataStore : public QObject
{
    Q_OBJECT
public:
    explicit DataStore( QObject* a_parent = nullptr );

signals:

};
