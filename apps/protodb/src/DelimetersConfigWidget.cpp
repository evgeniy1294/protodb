#include "DelimetersConfigWidget.h"

#include <QGridLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QLineEdit>
#include <QAbstractItemView>

using namespace protodb;

protodb::DelimetersConfigWidget::DelimetersConfigWidget(QWidget* parent)
    : QWidget(parent)
{
    createGui();
    connectSignals();

    nlohmann::json cfg;
        defaultConfig(cfg);
        setConfig(cfg);
}

void DelimetersConfigWidget::createGui()
{
    m_use_custom = new QCheckBox;
        m_use_custom->setCheckState(Qt::Unchecked);

    m_del_1 = new QComboBox();
        m_del_1->addItem("Off", kOffValue);
        m_del_1->addItem("CR", static_cast<int>('\r'));
        m_del_1->addItem("LF", static_cast<int>('\n'));
        m_del_1->addItem(";",  static_cast<int>(';'));
        m_del_1->addItem(tr("Custom"), kCustomValue);
        m_del_1->setInsertPolicy(QComboBox::InsertAtCurrent);

    m_del_2 = new QComboBox();
        m_del_2->setDisabled(true);
        m_del_2->addItem("Off", kOffValue);
        m_del_2->addItem("CR", static_cast<int>('\r'));
        m_del_2->addItem("LF", static_cast<int>('\n'));
        m_del_2->addItem(";",  static_cast<int>(';'));
        m_del_2->addItem(tr("Custom"), kCustomValue);
        m_del_2->setInsertPolicy(QComboBox::InsertAtCurrent);

    m_guard_int = new QSpinBox();
        m_guard_int->setMaximum(10000);
        m_guard_int->setSuffix(tr("ms"));
        m_guard_int->setSpecialValueText("Off");

    auto m_layout = new QGridLayout;
        m_layout->setAlignment(Qt::AlignTop | Qt::AlignLeft);

        //m_layout->addWidget(new QLabel(tr("Custom delimeter")), 0, 0, 1, 1);
        //m_layout->addWidget(m_use_custom, 0, 1, 1, 1);

        m_layout->addWidget(new QLabel(tr("Delimeters")), 1, 0, 1, 1);
        m_layout->addWidget(m_del_1, 1, 1, 1, 1);
        m_layout->addWidget(m_del_2, 1, 2, 1, 1);

        m_layout->addWidget(new QLabel(tr("Guard interval")), 2, 0, 1, 1);
        m_layout->addWidget(m_guard_int, 2, 1, 1, 1);

    setLayout(m_layout);
}

void DelimetersConfigWidget::connectSignals()
{
    connect(m_use_custom, &QCheckBox::stateChanged, this, [this](int check) {
        if (check == Qt::Checked) {
            m_del_1->setDisabled(true);
            m_del_2->setDisabled(true);
        }
        else {
            m_del_1->setEnabled(true);
            m_del_2->setEnabled(m_del_1->currentIndex() != 0);
        }
    });

    connect(m_del_1, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index){
        if (index == m_del_1->count() - 1) {
            m_del_1->setEditable(true);
            m_del_1->lineEdit()->setInputMask("Hh");
            m_del_1->lineEdit()->setCursorPosition(2);

            if (m_del_1->itemText(index) == tr("Custom")) {
                m_del_1->lineEdit()->clear();
            }
        }
        else {
            m_del_1->setEditable(false);
        }

        m_del_2->setEnabled(index != 0);
    });

    connect(m_del_2, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index){
        if (index == m_del_2->count() - 1) {
            m_del_2->setEditable(true);
            m_del_2->lineEdit()->setInputMask("Hh");
            m_del_2->lineEdit()->setCursorPosition(2);

            if (m_del_2->itemText(index) == tr("Custom")) {
                m_del_2->lineEdit()->clear();
            }
        }
        else {
            m_del_2->setEditable(false);
        }
    });
}

int DelimetersConfigWidget::getD1() const
{
    int d1 = m_del_1->currentData().toInt();
    if (m_del_1->currentIndex() == m_del_1->count() - 1) {
        bool ok = false;
        int tmp = m_del_1->currentText().toInt(&ok, 16);

        d1 = ok ? tmp : d1;
    }

    return d1;
}

int DelimetersConfigWidget::getD2() const
{
    int d2 = m_del_2->currentData().toInt();
    if (m_del_2->currentIndex() == m_del_2->count() - 1) {
        bool ok = false;
        int tmp = m_del_2->currentText().toInt(&ok, 16);

        d2 = ok ? tmp : d2;
    }

    return d2;
}

void DelimetersConfigWidget::config(nlohmann::json& json) const
{
    int d1 = getD1();
    int d2 = getD2();

    json["UseCustom"] = m_use_custom->isChecked();
    json["Delimeter1"] = d1;
    json["Delimeter2"] = d2;
    json["GuardInterval"] = m_guard_int->value();
}

void DelimetersConfigWidget::defaultConfig(nlohmann::json& json) const
{
    json["UseCustom"] = false;
    json["Delimeter1"] = -1;
    json["Delimeter2"] = -1;
    json["GuardInterval"] = 0;
}

void DelimetersConfigWidget::setConfig(const nlohmann::json& json)
{
    if (json.is_object()) {
        auto guard_int = json.value("GuardInterval", 0);
            m_guard_int->setValue(guard_int);

        auto del = json.value("Delimeter1", -1);
            auto idx = m_del_1->findData(del);
            if (idx >= 0 && idx < m_del_1->count()) {
                m_del_1->setCurrentIndex(idx);
            }
            else {
                m_del_1->setItemText(m_del_1->count()-1, QString::number(del, 16));
                m_del_1->setCurrentIndex(m_del_1->count()-1);
            }

        del = json.value("Delimeter2", -1);
            idx = m_del_2->findData(del);
            if (idx >= 0 && idx < m_del_2->count()) {
                m_del_2->setCurrentIndex(idx);
            }
            else {
                m_del_2->setItemText(m_del_2->count()-1, QString::number(del, 16));
                m_del_2->setCurrentIndex(m_del_2->count()-1);
            }

        auto use_custom = json.value("UseCustom", false);
            m_use_custom->setChecked(use_custom);
            if (m_use_custom->isChecked()) {
                m_del_1->setDisabled(true);
                m_del_2->setDisabled(true);
            }
            else {
                m_del_1->setEnabled(true);
                m_del_2->setEnabled(m_del_1->currentIndex() != 0);
            }
    }
}


