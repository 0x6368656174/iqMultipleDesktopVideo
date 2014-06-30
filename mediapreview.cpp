#include "mediapreview.h"
#include <QDebug>

MediaPreview::MediaPreview(QWidget *parent) :
    QGraphicsView(parent),
    _aspectRatio(1),
    _realSceneSize(QRect(0, 0, 800, 600))
{
    setOptimizationFlags(QGraphicsView::DontSavePainterState);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    viewport()->setFocusPolicy(Qt::NoFocus);
    setFocusPolicy(Qt::StrongFocus);
    QSizePolicy sizePol = sizePolicy();
    sizePol.setHeightForWidth(true);
    setSizePolicy(sizePol);
}

int MediaPreview::heightForWidth(int width) const
{
    if (_aspectRatio <= 0)
        return width;
    return width / _aspectRatio;
}

void MediaPreview::setScene(QGraphicsScene *scene)
{
    QGraphicsView::setScene(scene);
}

void MediaPreview::setRealSceneSize(QRect &size)
{
    _realSceneSize = QRect(0, 0, size.width(), size.height());
    setSceneRect(_realSceneSize);
    adjustSceneSize();
}

void MediaPreview::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    adjustSceneSize();
}

void MediaPreview::adjustSceneSize()
{
    int sceneWidth = _realSceneSize.width();
    int sceneHeight = _realSceneSize.height();
    qreal sceneAspectRatio = sceneWidth / sceneHeight;
    qreal sceneScaleCoof = 1;
    if (sceneAspectRatio < _aspectRatio)
    {
        sceneScaleCoof = (qreal)(height() - 5) / sceneHeight;
    } else
    {
        sceneScaleCoof = (qreal)(width() - 5) / sceneWidth;
    }
    QTransform tm = transform();
    tm.reset();
    tm.scale(sceneScaleCoof, sceneScaleCoof);
    setTransform(tm);
}
