#ifndef OUTPUTDOCK_H
#define OUTPUTDOCK_H

#include "dockbase.h"

class OutputDock : public DockBase
{
    Q_OBJECT

public:
    explicit OutputDock(QWidget *parent = nullptr);

protected:
    QWidget* createContent() override;

};

#endif // OUTPUTDOCK_H
