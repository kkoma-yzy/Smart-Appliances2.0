#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QMqttClient>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);

    ~Widget();

private slots:
    void on_loginbtn_clicked();
    void connectedslot();
    void readdataslot();

    void on_registerbtn_clicked();

    void on_ledbtn_clicked();

    void on_ledclosebtn_clicked();


    void on_registerbtn1_clicked();

    void on_backbtn_clicked();

    void on_air_open_btn_clicked();

    void on_air_close_btn_clicked();

    void on_addbtn_clicked();

    void on_jianbtn_clicked();

private:
    Ui::Widget *ui;
    QTcpSocket *socket;
    QMqttClient *m_client;
    QString sermsg;
    QString mqtt_serv_name;
    QString mqtt_serv_pwd;
    QByteArray secv_arr;

};
#endif // WIDGET_H
