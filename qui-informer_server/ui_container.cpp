#include "ui_container.hxx"
#include <QLabel>
#include <QPixmap>
#include <QVideoWidget>
#include <QRandomGenerator>


#include <tuple>

UIContainer::UIContainer(QWidget *parent) : QWidget(parent)
  {
    qDebug() << "ui widget created";

  }

QString UIContainer::ui_data()
  {
    QString result = "{\n";
    // text
    result.append("\"labels\" : [\n");
    QList <QLabel*> labels = this->findChildren<QLabel *>();
    foreach(auto label, labels)
      {
        result.append("{\n");
        result.append(QString("\t\"text\" : \"%1\"\n").arg(label->text()));
        result.append(QString("\t\"font\" : \"%1\"\n").arg(label->font().toString()));
        result.append("}\n");
      }

    QList <QVideoWidget *> videos = this->findChildren<QVideoWidget *>();
    foreach(auto video, videos)
      {
        result.append("{\n");
        result.append(QString("\t\"video\" : \"%1\"\n").arg(0));
        //result.append(QString("\t\"font\" : \"%1\"\n").arg(video->mediaObject());
        result.append("}\n");
      }

    result.append("]\n");
    // video
    // picture
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

  }

void UIContainer::generate_picture()
  {

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
