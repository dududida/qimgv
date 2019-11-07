#pragma once

#include "gui/overlays/imageinfooverlay.h"

struct ImageInfoOverlayStateBuffer {
    QMap<QString, QString> info;
};

class ImageInfoOverlayProxyWrapper {
public:
    explicit ImageInfoOverlayProxyWrapper(FloatingWidgetContainer *parent = nullptr);
    ~ImageInfoOverlayProxyWrapper();
    void init();
    void show();
    void hide();

    void setExifInfo(QMap<QString, QString> info);
    bool isHidden();
private:
    FloatingWidgetContainer *container;
    ImageInfoOverlay *overlay;
    ImageInfoOverlayStateBuffer stateBuf;
};
