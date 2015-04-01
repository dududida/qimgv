#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QImageReader>
#include <QColor>
#include <QPalette>
#include <QTimer>
#include <QThread>
#include <QtConcurrent>
#include <QFuture>
#include <QDebug>
#include <QErrorMessage>
#include <vector>
#include "image.h"
#include "mapoverlay.h"
#include "settings.h"
#include <time.h>

#define FLT_EPSILON 1.19209290E-07F

#define AVG(a,b)  ( ((((a)^(b)) & 0xfefefefeUL) >> 1) + ((a)&(b)) )

enum ImageFitMode
{
    NORMAL,
    WIDTH,
    ALL,
    FREE
};

class ImageViewer : public QWidget
{
    Q_OBJECT

public:
    ImageViewer();
    ImageViewer(QWidget* parent);
    ~ImageViewer();
    Image* getCurrentImage() const;
    bool isDisplaying() const;

signals:
    void sendDoubleClick();
    void sendRightDoubleClick();
    void imageChanged();

public slots:
    void displayImage(Image* image);
    void freeImage();
    void slotFitNormal();
    void slotFitWidth();
    void slotFitAll();
    void slotZoomIn();
    void slotZoomOut();

private slots:
    void onAnimation();
    void resizeImage();

protected:
    virtual void paintEvent(QPaintEvent* event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void resizeEvent(QResizeEvent* event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);

private:
    Image* currentImage;
    QImage image;
    QTimer *resizeTimer;
    QRectF drawingRect;
    QPoint mouseMoveStartPos;

    QColor bgColor;

    MapOverlay *mapOverlay;

    bool isDisplayingFlag;
    bool errorFlag;

    float currentScale;
    float maxScale; // zoom OUT
    float minScale; // zoom IN
    float scaleStep;
    QPointF fixedZoomPoint;

    ImageFitMode imageFitMode;
    void initMap();
    void setScale(float scale);
    void updateMaxScale();
    void scaleAround(QPointF p, float oldScale);
    void fitDefault();
    void fitNormal();
    void fitWidth();
    void fitAll();
    void imageAlign();
    void centerImage();
    void updateMap();
    void fixAlignHorizontal();
    void fixAlignVertical();
    float scale() const;
    bool imageIsScaled() const;
    void stopAnimation();
    void startAnimation();
    QImage halfSized(const QImage &source);
    void downScaleImage();
    void slowScaleFrom(QImage*);
};

#endif // IMAGEVIEWER_H