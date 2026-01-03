#include "dockbase.h"

DockBase::DockBase(const QString &title, QWidget *parent)
    : QDockWidget(title, parent)
{
    setFeatures(QDockWidget::DockWidgetMovable |
                QDockWidget::DockWidgetFloatable);
}

void DockBase::showEvent(QShowEvent *event)
{
    if (!m_initialized) {
        if (auto *old = widget())
            old->deleteLater();

        QWidget *content = createContent();
        setWidget(content);

        m_initialized = true;
    }

    QDockWidget::showEvent(event);
}
