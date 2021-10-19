#pragma once

#include <QDialog>
#include <QPointer>

class QDialogButtonBox;
class QDataWidgetMapper;
class QAbstractButton;
class QLabel;

class SqTableDialog: public QDialog
{
    Q_OBJECT

public:
    SqTableDialog( QWidget* aParent = nullptr );
    ~SqTableDialog() = default;

    QDataWidgetMapper* getMapper() const;
    void setMapper(QDataWidgetMapper* aMapper);

    void selectRow(int aRow);
    int getRow() const;

private slots:
    void onDialogBtnClicked(QAbstractButton* aBtn);
    void onBackBtnClicked();
    void onPrevBtnClicked();
    void onNextBtnClicked();
    void onFrontBtnClicked();
    void onAddBtnClicked();


private:
    void createGui();
    void createConnections();

    int mSelectedRow;
    QLabel* itemLabel;
    QDataWidgetMapper* mMapper;
    QDialogButtonBox* mButtonBox;
};
