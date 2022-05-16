#include "imagetransform.h"

#include <QPainter>
#include <utility>
//这是一个类一个类一个类
ImageTransform::ImageTransform() {}
ImageTransform::ImageTransform(const QPointF &pos) {}

const QImage &ImageTransform::getImage() { return this->image; }
bool ImageTransform::setImage(const char *path) {
  bool succeeded = true;
  if (path != nullptr)
    succeeded = this->image.load(path);
  else
    this->image = QImage{};
  updateImageRect();
  return succeeded;
}
bool ImageTransform::setImage(const QString &path) {
  bool succeeded = this->image.load(path);
  //这里的path就是文件名
  //Loads an image from the file with the given fileName.
  //Returns true if the image was successfully loaded;
  //otherwise invalidates the image and returns false
  updateImageRect();
  return succeeded;
}

void ImageTransform::setOffset(const QPointF &offset) {
  this->offset = offset;//offset具有x和y轴方向的偏移量
  updateImageRect();
}
void ImageTransform::setAlignment(Qt::Alignment alignment) {
  this->alignment = alignment;//设置图片的位置？
  updateImageRect();
}

void ImageTransform::updateImageRect() {
  this->prepareGeometryChange();
  //使scene提前知道item要改变了
  imageRect = this->image.rect();
  //返回包裹着这个图形的矩形
  imageRect.translate(this->offset);
  //这个是将imageRect移动offset的偏移量
  QPointF alignmentPoint;
  //接下来是水平和竖直方向的调整
  switch (this->alignment & Qt::AlignHorizontal_Mask) {
  //说明alignment是AlignHorizontal_Mask中的一个
    case Qt::AlignLeft:
      alignmentPoint.setX(0);
      break;
    case Qt::AlignHCenter:
      alignmentPoint.setX(this->image.width() / 2);
      break;
    case Qt::AlignRight:
      alignmentPoint.setX(this->image.width());
      break;
  }
  switch (this->alignment & Qt::AlignVertical_Mask) {
    case Qt::AlignTop:
      alignmentPoint.setY(0);
      break;
    case Qt::AlignVCenter:
      alignmentPoint.setY(this->image.height() / 2);
      break;
    case Qt::AlignBottom:
      alignmentPoint.setY(this->image.height());
      break;
  }
  imageRect.translate(-alignmentPoint);
}

QRectF ImageTransform::boundingRect() const { return this->imageRect; }
void ImageTransform::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget) {
  if (this->image.isNull()) return;
  Q_UNUSED(option);//suppress the warning from the compiler
  Q_UNUSED(widget);
  painter->drawImage(this->boundingRect(), this->image);
}
