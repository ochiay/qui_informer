#include "mainwindow.hxx"

#include <QUdpSocket>
#include <QNetworkDatagram>

#include <QJsonDocument>
#include <QJsonArray>
#include <QLabel>

#include <QGraphicsView>
#include <QGraphicsPixmapItem>

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
    // https://www.weiy.city/2020/08/how-to-write-and-read-json-file-by-qt/
    QNetworkDatagram datagram { m_udp_socket->receiveDatagram() };

    ui->textEdit->setText(datagram.data());
    QJsonParseError json_err;
    QJsonDocument ui_doc = QJsonDocument::fromJson(datagram.data(), &json_err);

    QJsonObject ui_obj = ui_doc.object();
    //! hardcoded key captions
    auto videos = ui_obj.take( "videos" ).toArray();
    auto labels = ui_obj.take( "labels" ).toArray();
    auto pictures = ui_obj.take( "pictures" ).toArray();

    /*! duplicated code as in server:ui_container.cpp:generate_...*/
    /*! here should be various asserts and checks */
    // videos
    for (auto video: videos)
      {
        VideoWidget *w_video = generate_base_widget<VideoWidget>(video);
        auto filename = video.toObject().take("filename").toString();
        /*! create video obj      * */
#if QT_VERSION > QT_VERSION_CHECK(5, 15 ,2)
        QMediaPlayer *m_player = new QMediaPlayer(this);
        m_player->setMedia(QUrl::fromLocalFile(filename));
        m_player->setVideoOutput(w_video);
        m_player->play();
#endif
      }

    // labels
    for (auto label: labels)
      {
        QLabel *w_label = generate_base_widget<QLabel>(label);
        auto font = label.toObject().take("font").toString();
        auto text = label.toObject().take("text").toString();

        w_label->setText(text);
        w_label->setFont(QFont(font));
      }

    // pictures
    for (auto picture: pictures)
      {
        auto filename = picture.toObject().take("filename").toString();

        QGraphicsView *view = generate_base_widget<QGraphicsView>(picture);
        QGraphicsScene *scene = new QGraphicsScene;
        view->setScene(scene);
        QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap(filename));
        scene->addItem(item);
      }

  }

