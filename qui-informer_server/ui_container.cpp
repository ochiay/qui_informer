#include "ui_container.hxx"

#include <QtGlobal>

#include <QLabel>

#include <QFileInfo>
#include <QUrl>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QPixmap>

#include <QRandomGenerator>

#include <QJsonArray>

#include <QVideoWidget>
#include <QMediaPlayer>

UIContainer::UIContainer(QWidget *parent) : QWidget(parent)
  {
    qDebug() << "ui widget created";
    // json init

    json_obj["labels"]   = QJsonArray();
    json_obj["videos"]   = QJsonArray();
    json_obj["pictures"] = QJsonArray();
    m_json.setObject(json_obj);
  }

/*
 * create json-format message for sending to client
 */

void UIContainer::generate_text()
  {
    QLabel *label = generate_base_widget<QLabel>();

    label->setText(QString("other new label with coords: %1 %2")
                   .arg(label->x()).arg(label->y()));
    label->adjustSize();

    // add info to json doc
    QJsonArray array = json_obj["labels"].toArray();
    QJsonObject obj_label;
    obj_label["font"] = label->font().toString();
    obj_label["text"] = label->text();

    add_coord(obj_label, *label);
    array.append(obj_label);
    json_obj["labels"] = array;
    m_json.setObject(json_obj);

    QString str = m_json.toJson();
    emit json_updated(str);
  }
//------------------------------------------------------------------------------
void UIContainer::generate_video()
  {
    //! program'closing with crash due to something's there
    //! probably destoyng VideoWidget goes first than mediaplayer
    QString filename= "./test.mp4";

    VideoWidget *video = generate_base_widget<VideoWidget> ();
    QMediaPlayer *m_player = new QMediaPlayer(this);
    m_player->setMedia(QUrl::fromLocalFile(filename));
    m_player->setVideoOutput(video);
    m_player->play();

    // add info to json doc

    QJsonArray array = json_obj["videos"].toArray();
    QJsonObject obj_label;
    obj_label["filename"] = QFileInfo(filename).absoluteFilePath();

    add_coord(obj_label, *video);
    array.append(obj_label);
    json_obj["videos"] = array;
    m_json.setObject(json_obj);

    // deprecated: m_video.push_back(std::make_tuple(w_video, filename));

    QString str = m_json.toJson();
    emit json_updated(str);
  }
//------------------------------------------------------------------------------
void UIContainer::generate_picture()
  {
    QString filename = "./test.png";

    QGraphicsView *view = generate_base_widget<QGraphicsView>();
    QGraphicsScene *scene = new QGraphicsScene;
    view->setScene(scene);
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap(filename));
    // deprecated: m_picture.push_back(std::make_tuple(view, filename));
    scene->addItem(item);

    QJsonArray array = json_obj["pictures"].toArray();
    QJsonObject obj_label;
    obj_label["filename"] = QFileInfo(filename).absoluteFilePath();

    add_coord(obj_label, *view);
    array.append(obj_label);
    json_obj["pictures"] = array;
    m_json.setObject(json_obj);

    //! almost same as generate_video

    QString str = m_json.toJson();
    emit json_updated(str);
  }

//------------------------------------------------------------------------------
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

  }
//------------------------------------------------------------------------------
void UIContainer::add_coord(QJsonObject& _obj, QWidget&widget)
  {
    _obj["x"]      = widget.x();
    _obj["y"]      = widget.y();
    _obj["width"]  = widget.width();
    _obj["height"] = widget.height();
  };
