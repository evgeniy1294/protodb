#pragma once

#include <QWidget>

class QButtonGroup;

namespace protodb {

class SequenceFormatSelectionWidget: public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QString selectedFormat READ selectedFormat WRITE setFormatSelection NOTIFY selectedFormatChanged)

public:
    SequenceFormatSelectionWidget(QObject* parent = nullptr);
   ~SequenceFormatSelectionWidget() = default;

    void setFormatSelection(const QString& fmt);
    QString selectedFormat() const;

signals:
    void selectedFormatChanged(QString fmt);

private:
    void create_gui();
    void connect_signals();

private:
    QButtonGroup* m_btn_group;
    QString m_selected_format;
};

} // namespace protodb
