#include "mainwindow.hxx"

#include <QUdpSocket>
#include <QNetworkDatagram>

#include <QJsonDocument>
#include <QJsonArray>
#include <QLabel>

#include <QGraphicsView>
#include <QGraphicsPixmapItem>

#include <QMediaPlayer>
#include <QVideoWidget>

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

/* get json data and build interface with it */
/*! but first delete previous widgets */
/*! and without assertions */
void MainWindow::read_datagram()
  {
    // https://www.weiy.city/2020/08/how-to-write-and-read-json-file-by-qt/

    // get json data
    QNetworkDatagram datagram { m_udp_socket->receiveDatagram() };
    ui->textEdit->setText(datagram.data());

    // delete media
    QList <QMediaPlayer *> medias = this->findChildren<QMediaPlayer *>();
    for (auto m: medias)
      {
        m->stop();
        delete m;
      }
    medias.clear();

    // delete previous widgets
    QList <QWidget *> widgets_for_delete = ui->ui_container->findChildren<QWidget *>();
    for (auto w: widgets_for_delete)
      delete w;
    widgets_for_delete.clear();

    QList <QGraphicsScene *> scenes = this->findChildren<QGraphicsScene *>();
    for (auto s: scenes)
      delete s;
    scenes.clear();

    // make new
    QJsonParseError json_err;
    QJsonDocument ui_doc = QJsonDocument::fromJson(datagram.data(), &json_err);

    QJsonObject ui_obj = ui_doc.object();

    /*! duplicated code as in server:ui_container.cpp:generate_...*/
    /*! here should be various asserts and checks */
    // videos
    auto videos = ui_obj.take( "videos" ).toArray();
    for (auto video: videos)
      {
        VideoWidget *w_video = generate_base_widget<VideoWidget>(video);
        auto filename = video.toObject().take("filename").toString();
        /*! create video obj      * */

        QMediaPlayer *m_player = new QMediaPlayer(this);
        m_player->setMedia(QUrl::fromLocalFile(filename));
        m_player->setVideoOutput(w_video);
        m_player->play();
      }

    // labels
    auto labels = ui_obj.take( "labels" ).toArray();
    for (auto label: labels)
      {
        QLabel *w_label = generate_base_widget<QLabel>(label);
        auto font_str = label.toObject().take("font").toString();
        auto text = label.toObject().take("text").toString();

        w_label->setText(text);
        QFont font; font.fromString(font_str);
        w_label->setFont(font);
      }

    // pictures
    auto pictures = ui_obj.take( "pictures" ).toArray();
    for (auto picture: pictures)
      {
        auto filename = picture.toObject().take("filename").toString();

        QGraphicsView *view = generate_base_widget<QGraphicsView>(picture);
        QGraphicsScene *scene = new QGraphicsScene(this);
        view->setScene(scene);
        QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap(filename));
        scene->addItem(item);
      }

  }

