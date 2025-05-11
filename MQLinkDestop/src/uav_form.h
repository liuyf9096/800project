#ifndef UAV_FORM_H
#define UAV_FORM_H

#include <QWidget>

namespace Ui {
class UAVForm;
}

class UAVForm : public QWidget
{
    Q_OBJECT

public:
    explicit UAVForm(QWidget *parent = nullptr);
    ~UAVForm();

    void setIpAddress(QString ip);
    void setDeviceId(QString id);

private:
    Ui::UAVForm *ui;
};

#endif // UAV_FORM_H
