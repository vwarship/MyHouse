#include "SlideWidget.h"
#include <QtGui>
#include "ImageUtil.h"

SlideWidget::SlideWidget(QWidget *parent) : QLabel(parent)
{
    setScaledContents(true);
    hide();
}

void SlideWidget::setCurrentSlide(const QImage& image)
{
    const QSize thumbnailSize(width(), height());
    setPixmap(ImageUtil::scaleImage(QPixmap::fromImage(image), thumbnailSize));
}

void SlideWidget::mouseDoubleClickEvent(QMouseEvent *)
{
    stopPlay();
}

void SlideWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Escape:
        stopPlay();
        break;
    case Qt::Key_Home:
        emit firstSlide();
        break;
    case Qt::Key_End:
        emit lastSlide();
        break;
    case Qt::Key_Left:
        emit prevSlide();
        break;
    case Qt::Key_Right:
        emit nextSlide();
        break;
    }

    if ((event->key() == Qt::Key_C) && (event->modifiers() & Qt::ControlModifier))
        emit copyImage();
}

void SlideWidget::stopPlay()
{
    setWindowFlags(Qt::SubWindow);
    showNormal();
    hide();
}
