/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "BytecodeCharsEncoder.h"

#include <protodb/utils/JsonBaseUtils.h>
#include <nlohmann/json.hpp>

// Okteta core
#include <Okteta/AbstractByteArrayModel.hpp>
#include <Okteta/Codecs/USASCIICharCodec.hpp>
#include <Okteta/Character.hpp>
#include <Okteta/CharCodec.hpp>

namespace protodb {

const QString BytecodeCharsEncoder::CodecNameConfigKey      = QStringLiteral("CodecName");
const QString BytecodeCharsEncoder::UndefinedCharConfigKey  = QStringLiteral("UndefinedChar");
const QString BytecodeCharsEncoder::SubstituteCharConfigKey = QStringLiteral("SubstituteChar");

BytecodeCharsEncoder::BytecodeCharsEncoder(QObject* parent)
    : BytecodeEncoderAbstract(tr("name of the encoding target", "Characters"), QStringLiteral("text/plain"), QString(), parent)
    , m_codec_name(Okteta::USASCIICharCodec::codecName())
    , m_undefined_char(QLatin1Char('?'))
    , m_substitute_char(QLatin1Char('.'))
{

}

QString BytecodeCharsEncoder::encodeData(const QByteArray& data) const
{
    QString result;

    Okteta::CharCodec* charCodec = Okteta::CharCodec::createCodec(m_codec_name);
    const QChar tabChar = QLatin1Char('\t');
    const QChar returnChar = QLatin1Char('\n');

    for (int i = 0; i < data.size(); i++) {
        const Okteta::Character byteChar = charCodec->decode(data[i]);

        const QChar streamChar = byteChar.isUndefined() ? m_undefined_char :
                                    (!byteChar.isPrint() || byteChar == tabChar || byteChar == returnChar) ?
                                                                                    m_substitute_char :
                                                                                    (QChar)byteChar;
        result.append(streamChar);
    }

    // clean up
    delete charCodec;

    return result;
}

void BytecodeCharsEncoder::setConfig(const nlohmann::json& cfg)
{
    bool changed = false;

    auto codec_name = cfg.value(CodecNameConfigKey.toStdString(), Okteta::USASCIICharCodec::codecName());
    if (codec_name != m_codec_name) {
        m_codec_name = codec_name;
        changed = true;
    }

    auto undefined_char = QLatin1Char(cfg.value(UndefinedCharConfigKey.toStdString(), char('?')));
    if (undefined_char != m_undefined_char) {
        m_undefined_char = undefined_char;
        changed = true;
    }

    auto substitute_char = QLatin1Char(cfg.value(SubstituteCharConfigKey.toStdString(), char('.')));
    if (substitute_char != m_substitute_char) {
        m_substitute_char = substitute_char;
        changed = true;
    }

    if (changed) {
        emit configChanged();
    }
}

void BytecodeCharsEncoder::config(nlohmann::json& cfg) const
{
    cfg[CodecNameConfigKey.toStdString()] = m_codec_name;
    cfg[UndefinedCharConfigKey.toStdString()] = m_undefined_char;
    cfg[SubstituteCharConfigKey.toStdString()] = m_substitute_char;
}

void BytecodeCharsEncoder::setCodecName(const QString& codec)
{
    if (m_codec_name != codec) {
        m_codec_name = codec;
        emit configChanged();
    }
}

QString BytecodeCharsEncoder::codecName() const
{
    return m_codec_name;
}

void BytecodeCharsEncoder::setUndefinedChar(QChar undefinedChar)
{
    if (m_undefined_char != undefinedChar) {
        m_undefined_char = undefinedChar;
        emit configChanged();
    }
}

QChar BytecodeCharsEncoder::undefinedChar() const
{
    return m_undefined_char;
}

void BytecodeCharsEncoder::setSubstituteChar(QChar substituteChar)
{
    if (m_substitute_char != substituteChar) {
        m_substitute_char = substituteChar;
        emit configChanged();
    }
}

QChar BytecodeCharsEncoder::substituteChar() const
{
    return m_substitute_char;
}

} // namespace protodb
