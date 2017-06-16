/*
 * This is sort-of a main controller of application.
 * It creates and initializes all components, then sets up gui and actions.
 * Most of communication between components go through here.
 *
 */

#include "core2.h"

Core2::Core2()
    : QObject(),
      imageLoader(NULL),
      dirManager(NULL),
      cache(NULL),
      scaler(NULL),
      currentImageAnimated(NULL),
      currentVideo(NULL),
      mCurrentIndex(0),
      mPreviousIndex(0),
      mImageCount(0),
      infiniteScrolling(false)
{
    qRegisterMetaType<ScalerRequest>("ScalerRequest");
    initGui();
    initComponents();
    connectComponents();
    initActions();
    readSettings();
    connect(settings, SIGNAL(settingsChanged()), this, SLOT(readSettings()));
}

void Core2::readSettings() {
    infiniteScrolling = settings->infiniteScrolling();
    fitMode = settings->imageFitMode();

}

void Core2::showGui() {
    if(mw && !mw->isVisible())
        mw->show();
}

// create MainWindow and all widgets
void Core2::initGui() {
    imageViewer = new ImageViewer();
    videoPlayer = new VideoPlayerGL();
    viewerWidget = new ViewerWidget(imageViewer, videoPlayer);
    mw = new MainWindow2(viewerWidget);
    thumbnailPanelWidget = new ThumbnailStrip();
    mw->setPanelWidget(thumbnailPanelWidget);
    mw->hide();
}

void Core2::initComponents() {
    loadingTimer = new QTimer();
    loadingTimer->setSingleShot(true);
    loadingTimer->setInterval(500); // TODO: test on slower pc & adjust timeout
    dirManager = new DirectoryManager();
    cache = new ImageCache();
    imageLoader = new NewLoader(dirManager, cache);
    scaler = new Scaler();
}

void Core2::connectComponents() {
    connect(loadingTimer, SIGNAL(timeout()), this, SLOT(onLoadingTimeout()));
    connect(imageLoader, SIGNAL(loadStarted()),
            this, SLOT(onLoadStarted()));
    connect(imageLoader, SIGNAL(loadFinished(Image *, int)),
            this, SLOT(onLoadFinished(Image *, int)));
    //connect(cache, SIGNAL(initialized(int)), this, SIGNAL(cacheInitialized(int)), Qt::DirectConnection);
    //connect(dirManager, SIGNAL(fileRemoved(int)), cache, SLOT(removeAt(int)), Qt::DirectConnection);
    //connect(cache, SIGNAL(itemRemoved(int)), this, SIGNAL(itemRemoved(int)), Qt::DirectConnection);
    connect(dirManager, SIGNAL(directorySortingChanged()), this, SLOT(initCache()));

    connect(mw, SIGNAL(opened(QString)), this, SLOT(loadImageBlocking(QString)));

    // thumbnails stuff
    connect(cache, SIGNAL(initialized(int)), thumbnailPanelWidget, SLOT(fillPanel(int)));
    connect(thumbnailPanelWidget, SIGNAL(thumbnailRequested(int, int)),
            imageLoader, SLOT(generateThumbnailFor(int, int)), Qt::UniqueConnection);
    connect(imageLoader, SIGNAL(thumbnailReady(int, Thumbnail*)),
            thumbnailPanelWidget, SLOT(setThumbnail(int, Thumbnail*)));
    connect(thumbnailPanelWidget, SIGNAL(thumbnailClicked(int)), this, SLOT(openByIndex(int)));
    connect(this, SIGNAL(imageIndexChanged(int)), thumbnailPanelWidget, SLOT(highlightThumbnail(int)));
    connect(imageViewer, SIGNAL(scalingRequested(QSize)), this, SLOT(scalingRequest(QSize)));
    connect(scaler, SIGNAL(scalingFinished(QPixmap*,ScalerRequest)), this, SLOT(onScalingFinished(QPixmap*,ScalerRequest)));
}

void Core2::initActions() {
    connect(actionManager, SIGNAL(nextImage()), this, SLOT(slotNextImage()));
    connect(actionManager, SIGNAL(prevImage()), this, SLOT(slotPrevImage()));
    connect(actionManager, SIGNAL(fitAll()), imageViewer, SLOT(setFitAll()));
    connect(actionManager, SIGNAL(fitWidth()), imageViewer, SLOT(setFitWidth()));
    connect(actionManager, SIGNAL(fitNormal()), imageViewer, SLOT(setFitOriginal()));

    connect(actionManager, SIGNAL(fitAll()), mw, SLOT(showMessageFitAll()));
    connect(actionManager, SIGNAL(fitWidth()), mw, SLOT(showMessageFitWidth()));
    connect(actionManager, SIGNAL(fitNormal()), mw, SLOT(showMessageFitOriginal()));

    //connect(actionManager, SIGNAL(toggleFitMode()), this, SLOT(switchFitMode()));
    connect(actionManager, SIGNAL(toggleFullscreen()), mw, SLOT(triggerFullscreen()));
    connect(actionManager, SIGNAL(zoomIn()), imageViewer, SLOT(slotZoomIn()));
    connect(actionManager, SIGNAL(zoomOut()), imageViewer, SLOT(slotZoomOut()));
    connect(actionManager, SIGNAL(scrollUp()), imageViewer, SLOT(scrollUp()));
    connect(actionManager, SIGNAL(scrollDown()), imageViewer, SLOT(scrollDown()));
    //connect(actionManager, SIGNAL(resize()), this, SLOT(slotResizeDialog()));
    connect(actionManager, SIGNAL(rotateLeft()), this, SLOT(rotateLeft()));
    connect(actionManager, SIGNAL(rotateRight()), this, SLOT(rotateRight()));
    connect(actionManager, SIGNAL(openSettings()), mw, SLOT(showSettings()));
    //connect(actionManager, SIGNAL(crop()), this, SLOT(slotCrop()));
    //connect(actionManager, SIGNAL(setWallpaper()), this, SLOT(slotSelectWallpaper()));
    connect(actionManager, SIGNAL(open()), mw, SLOT(showOpenDialog()));
    connect(actionManager, SIGNAL(save()), mw, SLOT(showSaveDialog()));
    connect(actionManager, SIGNAL(exit()), mw, SLOT(close()));
    //connect(actionManager, SIGNAL(removeFile()), core, SLOT(removeFile()));
}

Image *Core2::currentImage() {
    return cache->imageAt(mCurrentIndex);
}

void Core2::rotateLeft() {
    rotateByDegrees(-90);
}

void Core2::rotateRight() {
    rotateByDegrees(90);
}

///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////
void Core2::scalingRequest(QSize size) {
    if(currentImage()) {
        scaler->requestScaled(ScalerRequest(currentImage(), size, QString::number(size.width())));
    }
}

void Core2::onScalingFinished(QPixmap *scaled, ScalerRequest req) {
    if(currentImage()) {
        //delete scaled;
        imageViewer->updateImage(scaled);
    } else {
        delete scaled;
    }
}

void Core2::rotateByDegrees(int degrees) {
    if(currentImage() != NULL) {
        currentImage()->rotate(degrees);
        ImageStatic *staticImage;
        if((staticImage = dynamic_cast<ImageStatic *>(currentImage())) != NULL) {
            viewerWidget->showImage(currentImage()->getPixmap());
        }
        else if ((currentVideo = dynamic_cast<Video *>(currentImage())) != NULL) {
            //emit videoAltered(currentVideo->getClip());
        }
        updateInfoString();
    }
}

void Core2::initCache() {
        // construct file path list
        QStringList filePaths;
        for(int i = 0; i < dirManager->fileCount(); i++) {
            filePaths << dirManager->filePathAt(i);
        }
        cache->init(dirManager->currentDirectory(), &filePaths);
}

void Core2::stopPlayback() {
    viewerWidget->stopPlayback();
}

void Core2::loadImage(QString filePath, bool blocking) {
    if(dirManager->isImage(filePath)) {
        ImageInfo *info = new ImageInfo(filePath);
        int index = dirManager->indexOf(info->fileName());
        if(index == -1) {
            dirManager->setDirectory(info->directoryPath());
            index = dirManager->indexOf(info->fileName());
            if(index == -1) {
                qDebug() << "Core: could not open file. This is a bug.";
                return;
            }
        }
        mCurrentIndex = index;
        mImageCount = dirManager->fileCount();
        if(cache->currentDirectory() != dirManager->currentDirectory()) {
            this->initCache();
        }
        //stopPlayback();
        if(blocking)
            imageLoader->openBlocking(mCurrentIndex);
        else
            imageLoader->open(mCurrentIndex);
    } else {
        qDebug() << "Core: invalid/missing file.";
    }
}

void Core2::loadImage(QString filePath) {
    loadImage(filePath, false);
}

void Core2::loadImageBlocking(QString filePath) {
    loadImage(filePath, true);
}

void Core2::openByIndex(int index) {
    if(index >=0 && index < dirManager->fileCount()) {
        mCurrentIndex = index;
        //stopPlayback();
        imageLoader->open(index);
    }
    else
        qDebug() << "Core::loadImageByIndex - argument out of range.";
}

void Core2::slotNextImage() {
    if(dirManager->containsImages()) {
        int index = mCurrentIndex + 1;
        if(index >= dirManager->fileCount()) {
            if(infiniteScrolling) {
                index = 0;
            }
            else {
                mw->showMessageDirectoryEnd();
                return;
            }
        }
        //stopPlayback();
        mCurrentIndex = index;
        imageLoader->open(index);
        if(dirManager->checkRange(index + 1))
            imageLoader->preload(index + 1);
    }
}

void Core2::slotPrevImage() {
    if(dirManager->containsImages()) {
        int index = mCurrentIndex - 1;
        if(index < 0) {
            if(infiniteScrolling) {
                index = dirManager->fileCount() - 1;
            }
            else {
                mw->showMessageDirectoryStart();
                return;
            }
        }
        //stopAnimation();
        mCurrentIndex = index;
        imageLoader->open(index);
        if(dirManager->checkRange(index - 1))
            imageLoader->preload(index - 1);
    }
}

void Core2::onLoadStarted() {
    updateInfoString();
    loadingTimer->start();
}

void Core2::onLoadingTimeout() {
    // TODO: show loading message over MainWindow
}

void Core2::onLoadFinished(Image *img, int index) {
    mutex.lock();
    //emit signalUnsetImage();
    loadingTimer->stop();
    currentImageAnimated = NULL;
    currentVideo = NULL;
    mPreviousIndex = index;
    if(img) {
        if((currentImageAnimated = dynamic_cast<ImageAnimated *>(img)) != NULL) {
            viewerWidget->showAnimation(currentImageAnimated->getMovie());
        } else if((currentVideo = dynamic_cast<Video *>(img)) != NULL) {
            showGui(); // workaround for mpv. If we play video while mainwindow is hidden we get black screen.
            viewerWidget->showVideo(currentVideo->getClip());
        } else {
            // static image
            viewerWidget->showImage(img->getPixmap());
        }
        emit imageIndexChanged(index);
        updateInfoString();
    }
    mutex.unlock();
}

void Core2::updateInfoString() {
    Image* img = currentImage();
    QString infoString = "";
    infoString.append("[ " +
                      QString::number(mCurrentIndex + 1) +
                      "/" +
                      QString::number(dirManager->fileCount()) +
                      " ]   ");
    if(img) {
        QString name, fullName = img->info()->fileName();
        if(fullName.size()>95) {
            name = fullName.left(95);
            name.append(" (...) ");
            name.append(fullName.right(12));
        } else {
            name = fullName;
        }
        infoString.append(name + "  ");
        infoString.append("(" +
                          QString::number(img->width()) +
                          "x" +
                          QString::number(img->height()) +
                          "  ");
        infoString.append(QString::number(img->info()->fileSize()) + " KB)");
    }
    mw->setInfoString(infoString);
}