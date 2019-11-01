#include "widget.h"
#include "ui_widget.h"
#include <QHostAddress>
#include <QtMqtt/QMqttClient>
#include<QtDebug>
#include<QMessageBox>
#include <QtNetwork/QTcpSocket>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    ,m_baiduVop("7YSNAv24emdNrXGlBsZT6O8z", "iZiuuv78lFt7MI6nOOHZsxSBww9Kfb9K")
{

    ui->setupUi(this);

    pixmap=new QPixmap();
    this->mysocket=NULL;
    ui->openvideo->setEnabled(true);
    ui->closevideo->setEnabled(false);
    ui->videolabel->setMinimumSize(480,320);
    ui->videolabel->setMaximumSize(480,320);


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

        QString str1=value.mid(99,3);
        QString str2=value.mid(113,3);
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
        else if("好冷"==str1)
        {
            addtemp();
        }
        else if("好热"==str1)
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

void Widget::on_openvideo_clicked()
{
    if(this->mysocket == NULL)
    {
     this->mysocket = new QTcpSocket();
     this->mysocket->connectToHost("192.168.0.4",8888);//TCP建立连接
    //发送信号者为socket这个实例，发送的信号是connected(),接收者为Form,槽函数为msconnected()
     connect(this->mysocket,SIGNAL(connected()),this,SLOT(msconnected()));
     connect(this->mysocket,SIGNAL(disconnected()),this,SLOT(msdisconnected()));
     //发送信号者为socket这个实例，发送的信号是error(QAbstractSocket::SocketError),接收者为Form,槽函数为error()
     //connect(this->socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(error()));
     ui->openvideo->setEnabled(false);//开启视频后让此按钮变灰
     ui->closevideo->setEnabled(true);//已经开启视频后让关闭视频按钮变黑（true）

    }
    else
    {
        ui->openvideo->setEnabled(false);
        ui->closevideo->setEnabled(true);
        timer->start(100);
    }
}

void Widget::msconnected()
{
    qDebug() << "connected";
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(updatepic()));
    timer->start(100);
}

void Widget::msdisconnected()
{
    if(timer->isActive())
        timer->stop();
    ui->openvideo->setEnabled(true);//按下关闭视频按钮后让开启视频按钮变黑（true）
    ui->closevideo->setEnabled(false);//按下关闭视频按钮后让关闭按钮变灰（false）
    this->mysocket->close();
    this->mysocket = NULL;
}

void Widget::updatepic()
{
    int ret;
    char *request = "pic";      //请求命令
    char response[20];          //服务器发给QT的回复
    char *len;                  //
    unsigned int piclen;        //图片长度
    static char picbuf[1024 * 1024];   //存放图片


    ret = this->mysocket->write(request, strlen(request));
    if (ret != strlen(request)) {
        qDebug() << "send request failed";
        timer->stop();
        this->mysocket->close();
    }
    this->mysocket->flush();    //刷新socket
    this->mysocket->waitForReadyRead(30000);

    memset(response, 0, sizeof(response));

    ret = this->mysocket->read(response, sizeof(response));
    if (ret != sizeof(response)) {
        qDebug() << "recv response failed";
        timer->stop();
        this->mysocket->close();
        this->mysocket = NULL;
    }
    len = strstr(response, "len");
    if (len == NULL) {
        qDebug() << "response header is error";
        timer->stop();
        this->mysocket->close();
        this->mysocket = NULL;
    }
    *len = '\0';
    //从response中读取图片长度
    piclen = atoi(response);

    qDebug() << "piclen: " << piclen;

    int total = 0;
    //循环读取pic信息
    while (total < piclen) {
        ret = this->mysocket->read(picbuf+total, piclen-total);
        if (ret < 0) {
            qDebug() << "recv pic failed" << ret;
            timer->stop();
            this->mysocket->close();
            this->mysocket = NULL;
            return;
        } else if (ret == 0) {
            this->mysocket->waitForReadyRead(30000);
            continue;
        } else
            total += ret;

        qDebug() << "total: " << total;
    }

    pixmap->loadFromData((const uchar *)picbuf, piclen, "JPEG");
    ui->videolabel->setPixmap(*pixmap);
    update();

}

void Widget::on_closevideo_clicked()
{
    qDebug() << "total:----11--------------- ";
    if(timer->isActive())
        timer->stop();
   qDebug() << "total:------------------- ";
    ui->openvideo->setEnabled(true);//按下关闭视频按钮后让开启视频按钮变黑（true）
    ui->closevideo->setEnabled(false);//按下关闭视频按钮后让关闭按钮变灰（false）
    ui->videolabel->clear();
}
