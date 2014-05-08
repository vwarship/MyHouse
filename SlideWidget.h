#ifndef SLIDEWIDGET_H
#define SLIDEWIDGET_H

#include <QLabel>

class SlideWidget : public QLabel
{
    Q_OBJECT
public:
    explicit SlideWidget(QWidget *parent = 0);
    void setCurrentSlide(const QImage& image);
    
signals:
    void firstSlide();
    void lastSlide();
    void prevSlide();
    void nextSlide();
    void copyImage();

public slots:

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);

private:
    void stopPlay();

};

#endif // SLIDEWIDGET_H
