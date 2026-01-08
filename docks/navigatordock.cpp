#include "navigatordock.h"

#include <QVBoxLayout>
#include <QPushButton>

NavigatorDock::NavigatorDock(QWidget *parent)
    : DockBase("Navigator", parent) {}

QWidget* NavigatorDock::createContent()
{
    auto *w = new QWidget(this);
    w->setMinimumWidth(150);
    auto *layout = new QVBoxLayout(w);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(6);

    auto *compileAllBtn = new QPushButton("▶ Compile Design", w);
    compileAllBtn->setObjectName("compilebutton");
    compileAllBtn->setMinimumHeight(32);
    layout->addWidget(compileAllBtn);

    return w;
}
