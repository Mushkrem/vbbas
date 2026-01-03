#ifndef DOCKBASE_H
#define DOCKBASE_H

#include <QDockWidget>

class DockBase : public QDockWidget {
    Q_OBJECT
public:
    explicit DockBase(const QString &title, QWidget *parent = nullptr);

protected:
    void showEvent(QShowEvent *event) override;
    virtual QWidget* createContent() = 0;

private:
    bool m_initialized = false;
};

#endif // DOCKBASE_H
