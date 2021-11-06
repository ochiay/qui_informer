#include "mainwindow.hxx"

#include <QUdpSocket>
#include <QNetworkDatagram>

#include <QJsonDocument>
#include <QJsonArray>
#include <QLabel>

#include <QGraphicsView>
#include <QGraphicsItem>
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

    // delete graphic items

    //delete_all<QGraphicsView>(this);

    // doesnt work delete_all<QGraphicsPixmapItem>(this); instead:
    // без этого куска крашится, когда удаляет graphicviews в delete_all<...>
    //for(auto item: gr_items) delete item;
    delete_all<QGraphicsScene>(ui->ui_container);
    for(auto item: e_noe_views) delete item;
    e_noe_views.clear();


    // delete media
    delete_all<QMediaPlayer>(this);
    // delete previous widgets
    delete_all<QWidget>(ui->ui_container);

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
        QGraphicsScene *scene = new QGraphicsScene(ui->ui_container);
        view->setScene(scene);
        QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap(filename));
        e_noe_scenes.push_back(scene);
        e_noe_views.push_back(view);
        scene->addItem(item);

      }

  }

