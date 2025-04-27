#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "zmq_messager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ZmqMessager::GetInstance();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_client_connect_clicked()
{
    ZmqMessager::GetInstance()->client->connect();
}

void MainWindow::on_btn_client_send_clicked()
{
    ZmqMessager::GetInstance()->client->sendMessage(ui->lineEdit_client->text());
}

void MainWindow::on_btn_server_bind_clicked()
{
    // ZmqMessager::GetInstance()->server->bindAddress();
}

void MainWindow::on_btn_server_send_clicked()
{
    ZmqMessager::GetInstance()->server->sendMessage("", ui->lineEdit_server->text());
}

