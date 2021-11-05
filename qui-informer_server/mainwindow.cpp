#include "mainwindow.hxx"
#include "ui_mainwindow.h"
#include "ui_container.hxx"
#include <QPushButton>
#include <QToolButton>
#include <QDebug>
#include <QNetworkDatagram>
#include <QJsonDocument>
#include <QUdpSocket>


MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
  {
    ui->setupUi(this);

    m_udp_socket = new QUdpSocket(this);

    m_udp_socket->bind(QHostAddress::LocalHost, 7748);
    connect(m_udp_socket, &QUdpSocket::readyRead, this, &MainWindow::get_request);

    connect(ui->btn_generate_text, &QPushButton::clicked, ui->ui_container, &UIContainer::generate_text);
    connect(ui->btn_generate_video, &QPushButton::clicked, ui->ui_container, &UIContainer::generate_video);
    connect(ui->btn_generate_picture, &QPushButton::clicked, ui->ui_container, &UIContainer::generate_picture);

    connect(ui->ui_container, &UIContainer::json_updated, ui->text_json, &QPlainTextEdit::setPlainText);
  }

MainWindow::~MainWindow()
  {
    delete ui;
  }

void MainWindow::send_datagram()
  {
    QByteArray buffer;

   //! QString as_json = ui->ui_container->ui_data();

    m_udp_socket->writeDatagram(buffer, QHostAddress::LocalHost, 7755);
  }

void MainWindow::get_request()
  {
    QNetworkDatagram datagram { m_udp_socket->receiveDatagram() };

    if (datagram.data() == "send me ui")
      {
        //!QByteArray data = ui->ui_container->ui_data().toUtf8();
        //!m_udp_socket->writeDatagram(data, QHostAddress::LocalHost, 7755);
      }

  }



