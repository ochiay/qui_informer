#include "mainwindow.hxx"
#include "ui_mainwindow.h"

#include <QUdpSocket>
#include <QNetworkDatagram>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
  {
    ui->setupUi(this);

    m_udp_socket = new QUdpSocket(this);
    m_udp_socket->bind(QHostAddress::LocalHost, 7755); //! port number should be taking out to config file in both programs

    connect(ui->btn_request_ui, &QPushButton::clicked, this, &MainWindow::show_ui);
    connect(m_udp_socket, &QUdpSocket::readyRead, this, &MainWindow::read_datagram);
  }

MainWindow::~MainWindow()
  {
    delete ui;
  }

void MainWindow::show_ui()
  {
    QByteArray data = "send me ui";
    m_udp_socket->writeDatagram(data, QHostAddress::LocalHost, 7748);


  }

void MainWindow::read_datagram()
  {
    QNetworkDatagram datagram { m_udp_socket->receiveDatagram() };
    ui->textEdit->setText(datagram.data());
  }

