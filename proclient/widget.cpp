#include "widget.h"
#include "ui_widget.h"
#include <QHostAddress>
#include <QtMqtt/QMqttClient>
#include<QtDebug>
#include<QMessageBox>



Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{

    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->page_1);
    socket = new QTcpSocket(this);
    m_client = new QMqttClient(this);
    m_client->setKeepAlive(60);
    m_client->setClientId("xiaojinhui");

    socket->connectToHost(QHostAddress("106.13.178.21"),10085);
    connect(socket,SIGNAL(connected()),this,SLOT(connectedslot()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(readdataslot()));


}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_loginbtn_clicked()
{
    QByteArray arr;
    QString str="login";
    //QString n="\n";
    QString p=",";
    arr.append(str);
    arr.append(p);

    QString name=ui->lineEditname->text();
    QString pwd=ui->lineEditpwd->text();
    if(name.isEmpty()||pwd.isEmpty())
    {
        QMessageBox::warning(this,"提示","用户名或密码不能为空！请重新输入！");
        ui->lineEditname->clear();
        ui->lineEditpwd->clear();
    }

    //QString sql="select pwd from user where pwd="+name;
    //qDebug()<<sql;
    arr.append(name);
    arr.append(p);
    arr.append(pwd);
    arr.append(p);
    qDebug()<<arr;
    socket->write(arr);


}

void Widget::connectedslot()
{
    ui->labelmsg->setText("has connected");
}

void Widget::readdataslot()
{

    secv_arr.clear();
     secv_arr = socket->readAll();
    QString str1=QString(secv_arr);
    qDebug()<<str1;
    if( "register ok" == QString(secv_arr).section(",",0,0));
    {
       //QMessageBox::information(this, QLatin1String("success"), QLatin1String("register ok"));
        //ui->stackedWidget->setCurrentWidget(ui->page_1);
          sermsg = QString(secv_arr).section(",",1);
         qDebug()<<sermsg;

         m_client->setHostname(QString(secv_arr).section(",",1,1).section(":",0,0));
         m_client->setPort(1883);
         m_client->setUsername(QString(secv_arr).section(",",2,2));
         m_client->setPassword(QString(secv_arr).section(",",3,3));
         m_client->connectToHost();
         qDebug() <<m_client->state();
         qDebug() <<QMqttClient::Disconnected;
         if(1==m_client->state())
         {


             ui->stackedWidget->setCurrentWidget(ui->page2);


         }

        return ;

    }

    if ("login ok" == QString(secv_arr).section(",",0,0))
    {


        sermsg = QString(secv_arr).section(",",1);
         qDebug()<<sermsg;
        m_client->setHostname(QString(secv_arr).section(",",1,1).section(":",0,0));
        m_client->setPort(1883);
        m_client->setUsername(QString(secv_arr).section(",",2,2));
        m_client->setPassword(QString(secv_arr).section(",",3,3));
        m_client->connectToHost();
        qDebug() <<m_client->state();
        qDebug() <<QMqttClient::Disconnected;
        if(1==m_client->state())
        {


            ui->stackedWidget->setCurrentWidget(ui->page2);


        }
        return ;
    }

}

void Widget::on_registerbtn_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->registerpage);

}

void Widget::on_ledbtn_clicked()
{
    QString str1="$baidu/iot/shadow/led/update";
    QString str="{\"desired\": {\"ledswitch\": 1}}";
    qDebug()<<"asdasdasd";
    //qDebug()<<ui->lineEditTopic->text();
    if (m_client->publish(str1,
                          str.toUtf8(),
                          0,
                          false)==-1)
        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not publish message"));
}

void Widget::on_ledclosebtn_clicked()
{

    QString str1="$baidu/iot/shadow/led/update";
    QString str="{\"desired\": {\"ledswitch\": 2}}";
    qDebug()<<"12111";
    if (m_client->publish(str1,
                          str.toUtf8(),
                         0,
                         false)==-1)
        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not publish message"));


}


void Widget::on_registerbtn1_clicked()
{
    QString username=ui->register_username->text();
    QString pwd=ui->register_pwd->text();
    QString seradd=ui->seraddlineEdit->text();
    QString seruser=ui->seruserlineEdit->text();
    QString serpwd=ui->serpwdlineEdit->text();
   //String str = ui->labelname->text();
   //String str1 = ui->labelpwd->text();
    QByteArray arr;
    arr.clear();
    QString p =",";
    arr.append("register");
    arr.append(p);
    arr.append(username);
    arr.append(p);
    arr.append(pwd);
    arr.append(p);
    arr.append(seradd);
    arr.append(p);
    arr.append(seruser);
    arr.append(p);
    arr.append(serpwd);
    arr.append(p);
    qDebug()<<arr;

    socket->write(arr);

}

void Widget::on_backbtn_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_1);
}

void Widget::on_air_open_btn_clicked()
{
    QString str1="$baidu/iot/shadow/led/update";
    QString str="{\"desired\": {\"airpower\": 1}}";
    qDebug()<<"空调";

    if (m_client->publish(str1,
                          str.toUtf8(),
                          0,
                          false)==-1)
        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not publish message"));
}

void Widget::on_air_close_btn_clicked()
{
    QString str1="$baidu/iot/shadow/led/update";
    QString str="{\"desired\": {\"airpower\": 2}}";
    qDebug()<<"空调关";

    if (m_client->publish(str1,
                          str.toUtf8(),
                          0,
                          false)==-1)
        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not publish message"));
}

void Widget::on_addbtn_clicked()
{
    QString str1="$baidu/iot/shadow/led/update";
    QString str="{\"desired\": {\"airtemp\": 2}}";
    qDebug()<<"温度加";

    if (m_client->publish(str1,
                          str.toUtf8(),
                          0,
                          false)==-1)
        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not publish message"));

}

void Widget::on_jianbtn_clicked()
{
    QString str1="$baidu/iot/shadow/led/update";
    QString str="{\"desired\": {\"airtemp\": 1}}";
    qDebug()<<"温度减";

    if (m_client->publish(str1,
                          str.toUtf8(),
                          0,
                          false)==-1)
        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not publish message"));

}
