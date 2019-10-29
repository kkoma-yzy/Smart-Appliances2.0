#include "widget.h"
#include "ui_widget.h"
#include <QHostAddress>
#include <QtMqtt/QMqttClient>
#include<QtDebug>
#include<QMessageBox>



Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    ,m_baiduVop("7YSNAv24emdNrXGlBsZT6O8z", "iZiuuv78lFt7MI6nOOHZsxSBww9Kfb9K")
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
    connect(m_client, &QMqttClient::messageReceived, this, [this](const QByteArray &message, const QMqttTopicName &topic) {
        const QString content = QDateTime::currentDateTime().toString(Qt::DefaultLocaleShortDate)
                    + " 主题: "
                    + topic.name()
                    + " 消息: "
                    + message
                    + "\n";
        //ui->editLog->insertPlainText(content);
        QString value=QString(message);
        qDebug()<<value;

        QString str1=value.mid(99,2);
        QString str2=value.mid(113,2);
        qDebug()<<str1;
        qDebug()<<str2;
        ui->temp->setText(str1);
        ui->shidu->setText(str2);
});


    /*connect(ui->pushButton, &QPushButton::clicked, [=](const bool &checked)
    {
        if(checked)
        {
            ui->pushButton->setText("Stop");
            this->startInput();
        }
        else
        {
            ui->pushButton->setText("Start");
            this->inputFinish();
        }
    });*/

    qDebug() << "Refresh token:" << m_baiduVop.refreshToken();


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
    if( "register ok" == QString(secv_arr).section(",",0,0))

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

            /*QString str="$baidu/iot/shadow/led/update/snapshot";
            auto topic1 = str;
            auto subscription1 = m_client->subscribe(topic1,0);
            if (!subscription1) {
                QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not subscribe. Is there a valid connection?"));
                return;
            }*/
            ui->stackedWidget->setCurrentWidget(ui->page2);


        }
        return ;
    }

}

void Widget::on_registerbtn_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->registerpage);

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



void Widget::on_wenshidubtn_clicked()
{
    QString str="$baidu/iot/shadow/led/update/snapshot";
    auto topic1 = str;
    auto subscription1 = m_client->subscribe(topic1,0);
    if (!subscription1) {
        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not subscribe. Is there a valid connection?"));
        return;
    }

}

void Widget::openlight()
{
    QString str1="$baidu/iot/shadow/led/update";
    QString str="{\"desired\": {\"ledswitch\": 1}}";
    qDebug()<<"开灯";
    //qDebug()<<ui->lineEditTopic->text();
    if (m_client->publish(str1,
                          str.toUtf8(),
                          0,
                          false)==-1)
        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not publish message"));
}

void Widget::closelight()
{
    QString str1="$baidu/iot/shadow/led/update";
    QString str="{\"desired\": {\"ledswitch\": 2}}";
    qDebug()<<"关灯";
    if (m_client->publish(str1,
                          str.toUtf8(),
                         0,
                         false)==-1)
        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not publish message"));
}

void Widget::openair()
{
    QString str1="$baidu/iot/shadow/led/update";
    QString str="{\"desired\": {\"airpower\": 1}}";
    qDebug()<<"开空调";

    if (m_client->publish(str1,
                          str.toUtf8(),
                          0,
                          false)==-1)
        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not publish message"));
}

void Widget::closeair()
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

void Widget::addtemp()
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

void Widget::jiantemp()
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
void Widget::on_ledbtn_clicked()
{
    openlight();

}

void Widget::on_ledclosebtn_clicked()
{
    closelight();



}
void Widget::on_air_open_btn_clicked()
{
    openair();

}

void Widget::on_air_close_btn_clicked()
{
    closeair();

}

void Widget::on_addbtn_clicked()
{
    addtemp();


}

void Widget::on_jianbtn_clicked()
{
    jiantemp();


}


void Widget::startInput(void)
{
    m_baiduVop.start();
}

void Widget::inputFinish(void)
{
    const auto &&data = m_baiduVop.finish();

    if(data.first)
    {
        ui->label101->setText(data.second);
        qDebug()<<data.second;
        QString str1=data.second;
        if("开灯" == str1)
        {
           openlight();
        }
        else if("关灯"==str1)
        {
            closelight();
        }
        else if("开空调"==str1)
        {
            openair();
        }
        else if("关空调"==str1)
        {
            closeair();
        }
        else if("加一度"==str1)
        {
            addtemp();
        }
        else if("减一度"==str1)
        {
            jiantemp();
        }
    }
    else
    {
        ui->label101->setText("识别错误");
    }
}




void Widget::on_pushButton_clicked()
{
    if(ui->pushButton->text()=="Start")
    {
        ui->pushButton->setText("Stop");
        this->startInput();
    }
    else
    {
        ui->pushButton->setText("Start");
        this->inputFinish();
    }
}
