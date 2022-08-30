#include "ssidebar.h"

SSideBar::SSideBar(QWidget *parent) : QWidget{parent} {
    layout = new QBoxLayout(QBoxLayout::TopToBottom);
    layout->setSpacing(0);
    layout->setContentsMargins(9,8,9,9);
    layout->addStretch();
    setLayout(layout);
    addEntry(":res/icons/dark/settings/general32.png", "General");
    addEntry(":res/icons/dark/settings/general32.png", "Image View");
    addEntry(":res/icons/dark/settings/appearance32.png", "Theme");
    addEntry(":res/icons/dark/settings/shortcuts32.png", "Controls");
    addEntry(":res/icons/dark/settings/terminal32.png", "Scripts");
    addEntry(":res/icons/dark/settings/advanced32.png", "Advanced");
    addEntry(":res/icons/dark/settings/about32.png", "About");
}

void SSideBar::addEntry(QString icon, QString name) {
    SSideBarItem *entry = new SSideBarItem(icon, name);
    layout->insertWidget(entries.count(), entry);
    entries.append(entry);
    if(entries.count() == 1)
        selectEntry(0);
}

void SSideBar::selectEntry(int idx) {
    if(idx >= 0 && idx < entries.count()) {
        foreach(auto entry, entries)
            entry->setHighlighted(false);
        entries[idx]->setHighlighted(true);
        emit entrySelected(idx);
    }
}

void SSideBar::mousePressEvent(QMouseEvent *event) {
    event->accept();
    if(!event->buttons() & Qt::LeftButton)
        return;
    selectEntryAt(event->pos());

}

void SSideBar::mouseMoveEvent(QMouseEvent *event) {
    event->accept();
    if(!event->buttons() & Qt::LeftButton)
        return;
    selectEntryAt(event->pos());
}

void SSideBar::selectEntryAt(QPoint pos) {
    int newSelection = -1;
    for(int i = 0; i < entries.count(); i++) {
        if(entries[i]->geometry().contains(pos)) {
            if(!entries[i]->highlighted())
                newSelection = i;
            break;
        }
    }
    selectEntry(newSelection);
}

void SSideBar::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event)
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

// -------------------------------------------------------------------

SSideBarItem::SSideBarItem(QString icon, QString name, QWidget *parent) : QWidget{parent} {
    iconWidget.setColor(QColor(70,70,70));
    iconWidget.setIconPath(icon);
    textLabel.setText(name);
    layout = new QBoxLayout(QBoxLayout::LeftToRight);
    layout->setContentsMargins(6,5,6,5);
    layout->addWidget(&iconWidget);
    layout->addWidget(&textLabel);
    layout->addStretch();
    setLayout(layout);
    setMouseTracking(true);
}

void SSideBarItem::setHighlighted(bool mode) {
    mHighlighted = mode;
    this->setProperty("checked", mHighlighted);
    style()->unpolish(this);
    style()->polish(this);
}

bool SSideBarItem::highlighted() {
    return mHighlighted;
}

void SSideBarItem::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event)
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
