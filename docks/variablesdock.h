#ifndef VARIABLESDOCK_H
#define VARIABLESDOCK_H

#include "dockbase.h"

class VariablesDock : public DockBase
{
    Q_OBJECT

public:
    explicit VariablesDock(QWidget *parent = nullptr);

protected:
    QWidget* createContent() override;

};

#endif // VARIABLESDOCK_H
