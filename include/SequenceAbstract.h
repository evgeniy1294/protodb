#pragma once

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QUuid>

class SequenceProcessor;

class SequenceAbstract {

public:
  friend class SequenceProcessor;

  enum Type {
      Incoming,
      Outgoing
  };

  SequenceAbstract()
   : m_uuid(QUuid::createUuid())
   , m_name()
   , m_dsl_string()
   , m_cached(false)
   , m_bytes()
   , m_active(false)
   , m_description() {  }

  SequenceAbstract(const SequenceAbstract& a_seq)
    : m_uuid(a_seq.m_uuid)
    , m_name(a_seq.m_name)
    , m_dsl_string(a_seq.m_dsl_string)
    , m_cached(a_seq.m_cached)
    , m_bytes(a_seq.m_bytes)
    , m_active(a_seq.m_active)
    , m_description(a_seq.m_description) {  }

  const QString& name() const { return m_name; }
  void setName(const QString& a_name) { m_name = a_name; }

  const QString& dslString() const { return m_dsl_string; };
  void setDslString(const QString& a_dsl) { m_dsl_string = a_dsl; }

  const QString& description() const { return m_description; }
  void setDescription(const QString& a_description) { m_description = a_description; }

  const QUuid& uuid() const { return m_uuid; };
  inline Type type() const { return static_cast<Type>(mType); }

  bool active() const { return m_active; }
  void setActive(bool a_active) { m_active = a_active; }

  bool operator==(const SequenceAbstract& rsh) {return m_uuid == rsh.m_uuid;}

private:
  bool m_cached;
  QByteArray m_bytes;

private:
  QUuid m_uuid;
  ushort mType;

  bool m_active;
  QString m_name;
  QString m_dsl_string;
  QString m_description;
};
