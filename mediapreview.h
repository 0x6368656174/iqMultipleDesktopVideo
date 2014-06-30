#ifndef MEDIAPREVIEW_H
#define MEDIAPREVIEW_H

#include <QGraphicsView>

class MediaPreview : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MediaPreview(QWidget *parent = 0);
    
    inline void setAspectRatio(const qreal aspectRatio) {_aspectRatio = aspectRatio;}

    void setScene(QGraphicsScene *scene);

    int heightForWidth(int width) const;

    void setRealSceneSize(QRect &size);

protected:
    void resizeEvent(QResizeEvent *event);

private:
    qreal _aspectRatio;

    QRect _realSceneSize;

    void adjustSceneSize();
};

#endif // MEDIAPREVIEW_H
