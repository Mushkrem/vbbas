#ifndef NAVIGATORDOCK_H
#define NAVIGATORDOCK_H

#include "dockbase.h"

class NavigatorDock : public DockBase
{
    Q_OBJECT

public:
    explicit NavigatorDock(QWidget *parent = nullptr);

protected:
    QWidget* createContent() override;

};

#endif // NAVIGATORDOCK_H
