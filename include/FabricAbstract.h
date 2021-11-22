#pragma once

#include <QtCore>

class CreatorAbstract;

class FabricAbstract: public QObject
{
    Q_OBJECT

public:
    FabricAbstract(QObject* parent = nullptr);

    virtual const QString& fid() const;
    virtual bool isCompatible(CreatorAbstract* creator) const;

    bool addCreator(CreatorAbstract* creator);
    void removeCreator(CreatorAbstract* creator);
    void removeCreator(const QString& cid);
    const QList<QString> creators() const;

signals:
    void sChanged();

protected:
    QMap<QString, CreatorAbstract*> m_creators;
};
