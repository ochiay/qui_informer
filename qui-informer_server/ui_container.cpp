#include "ui_container.hxx"
#include <QLabel>
#include <QPixmap>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QRandomGenerator>
#include <QUrl>

#include <tuple>

UIContainer::UIContainer(QWidget *parent) : QWidget(parent)
  {
    qDebug() << "ui widget created";

  }

/*
 * create json-format message for sending to client
 */
QString UIContainer::ui_data()
  {

    //! there could be special QJsonDocument or some kind of, but there isn't it
    QString result = "{\n";

    // labels

    QList <QLabel*> labels = this->findChildren<QLabel *>();
    result.append("\t\"labels\" : {\n");
    result.append(QString("\t\t\"count\" : %1,\n").arg(labels.count()));
    result.append("\t\t\"values\" : [\n");

    foreach(auto label, labels)
      {
        result.append("\t\t{\n");
        result.append(QString("\t\t\t\"text\" : \"%1\",\n").arg(label->text()));
        result.append(QString("\t\t\t\"font\" : \"%1\",\n").arg(label->font().toString()));
        result.append("\t\t}\n");
      }
    result.append("\t\t]\n");
    result.append("\t},\n");

    //! video and picture block can be taking out at common code
    // video

    result.append("\t\"videos\" : {\n");
    result.append(QString("\t\tcount : %1,\n").arg(m_video.count()));
    result.append("\t\tvalues : [\n");
    foreach(auto video, m_video)
      {
        result.append("\t\t{\n");

        result.append(QString("\t\t\t\"x\" : %1,\n").arg((std::get<0>(video))->x()));
        result.append(QString("\t\t\t\"y\" : %1,\n").arg((std::get<0>(video))->y()));
        result.append(QString("\t\t\t\"video file name\" : \"%1\",\n").arg(std::get<1>(video)));

        result.append("\t\t},\n");
      }
    result.append("\t\t]\n");
    result.append("\t},\n");

    // pictures

    result.append("\t\"pictures\" : {\n");
    result.append(QString("\t\t\"count\" : %1,\n").arg(m_picture.count()));
    result.append("\t\tvalues : [\n");
    foreach(auto picture, m_picture)
      {
        result.append("\t\t{\n");

        result.append(QString("\t\t\t\"x\" : %1,\n").arg((std::get<0>(picture))->x()));
        result.append(QString("\t\t\t\"y\" : %1,\n").arg((std::get<0>(picture))->y()));
        result.append(QString("\t\t\t\"picture file name\" : \"%1\",\n").arg(std::get<1>(picture)));

        result.append("\t\t},\n");
      }
    result.append("\t\t]\n");
    result.append("\t}\n");


    return result + "}\n";
  }

void UIContainer::generate_text()
  {
    QLabel *label = generate_base_widget<QLabel>();


    label->setText(QString("other new label with coords: %1 %2")
                   .arg(label->x()).arg(label->y()));
    label->adjustSize();
  }

void UIContainer::generate_video()
  {
    QVideoWidget *video = generate_base_widget<QVideoWidget> ();

    QMediaPlayer *m_player = new QMediaPlayer(this);
    QString filename= ".\\test.mp4";
    m_player->setMedia(QUrl::fromLocalFile(filename));
    m_player->setVideoOutput(video);
    m_player->play();

    m_video.push_back(std::make_tuple(video, filename));
    qDebug() << video->width() << ' ' << video->height();
  }

void UIContainer::generate_picture()
  {
    QGraphicsView *view = generate_base_widget<QGraphicsView>();
    QGraphicsScene *scene = new QGraphicsScene;
    view->setScene(scene);

    QString filename = ".\\test.png";

    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap(filename));

    m_picture.push_back(std::make_tuple(view, filename));
    scene->addItem(item);
  }

std::tuple<int,int,int,int> UIContainer::child_widget_size()
  {
    int max_width = this->size().width();
    int max_height = this->size().height();

    int pos_x  = QRandomGenerator::system()->bounded(max_width);
    int pos_y  = QRandomGenerator::system()->bounded(max_height);
    int width  = QRandomGenerator::system()->bounded(max_width - pos_x);
    int height = QRandomGenerator::system()->bounded(max_height - pos_y);

    qDebug() << pos_x << pos_y << width << height;
    return std::make_tuple(pos_x, pos_y, width, height);

  };
