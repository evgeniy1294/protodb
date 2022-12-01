#include "Sequence.h"
#include "SequenceFormatter.h"

#include <QSharedPointer>
#include <QRegularExpression>

namespace protodb  {

const QString Sequence::DefaultFormatId = "HEX";
QStringList Sequence::m_supported_formats = {"HEX", "ASCII"};
QList< QSharedPointer<SequenceFormatter> > Sequence::m_formatters = {};

void Sequence::addFormatter(QSharedPointer<SequenceFormatter>& formatter)
{
    auto name = formatter->name();
    if ( !m_supported_formats.contains(name) ) {
        m_supported_formats.push_back(name);
        m_formatters.push_back(formatter);
    }

    return;
}

void Sequence::setDslString(const QString &dsl) {
    m_dsl_string = dsl;
    m_cached = false;

    format();
}

void Sequence::setFormatId(QString id) {
    if (m_format_id == id) return;

    m_format_id = id;
    m_cached = false;

    format();
}

// ([0-9a-fA-F]{2}\b)*
// ([[:xdigit:]]{2}\b)+   - найдет все байты
// [^0-9a-fA-F\s] - найдет все неподходящие символы
bool Sequence::format() {
    if (m_cached) return true;

    m_bytes.clear();

    if (m_format_id == "ASCII") {
        m_bytes = m_dsl_string.toLatin1();
    }
    else if (m_format_id == "HEX") {
        static QRegularExpression re("[^0-9a-fA-F,(0x)\r\\s\n\t\f\v]");

        QRegularExpressionMatch match = re.match(m_dsl_string);
        if ( !match.hasMatch() ) {
            m_bytes = QByteArray::fromHex( m_dsl_string.toLatin1() );
        }
    }
    else {
        for(auto& fmt: Sequence::m_formatters) {
            if (fmt->name() == m_format_id) {
                m_bytes = fmt->format(m_dsl_string);
            }
        }
    }

    m_cached = !m_bytes.isEmpty();
    return m_cached;
}

} // namespace protodb
