#pragma once

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QUuid>

class DslProcessor;

class Sequence {

public:
  friend class DslProcessor;

  Sequence()
   : m_uuid(QUuid::createUuid())
   , m_name()
   , m_dsl_string()
   , m_cached(false)
   , m_bytes()
   , m_active(false)
   , m_description()
   , m_binded_name()
   , m_period(0) {  }

  Sequence(const Sequence& seq)
    : m_uuid(seq.m_uuid)
    , m_name(seq.m_name)
    , m_dsl_string(seq.m_dsl_string)
    , m_cached(seq.m_cached)
    , m_bytes(seq.m_bytes)
    , m_active(seq.m_active)
    , m_description(seq.m_description)
    , m_binded_name(seq.m_binded_name)
    , m_period(seq.m_period) {  }

  const QUuid& uuid() const { return m_uuid; };

  const QString& name() const { return m_name; }
  void setName(const QString& a_name) { m_name = a_name; }

  const QString& dslString() const { return m_dsl_string; };
  void setDslString(const QString& dsl) { m_dsl_string = dsl; }

  const QString& description() const { return m_description; }
  void setDescription(const QString& description) { m_description = description; }

  const QString& bindedName() const { return m_binded_name; }
  void setBindedName(const QString& name) { m_binded_name = name; }

  bool active() const { return m_active; }
  void setActive(bool a_active) { m_active = a_active; }

  quint32 period() const { return m_period;}
  void setPeriod( quint32 period ) { m_period = period; }



  bool operator==(const Sequence& rsh) {return m_uuid == rsh.m_uuid;}

private:
  bool m_cached;
  QByteArray m_bytes;
  QUuid m_binded_uuid;

private:
  QUuid m_uuid;

  bool m_active;
  quint32 m_period;
  QString m_name;
  QString m_dsl_string;
  QString m_description;
  QString m_binded_name;
};
