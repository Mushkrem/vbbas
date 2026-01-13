#ifndef OBJECTACTIONS_H
#define OBJECTACTIONS_H

#include <QToolBar>
#include <QObject>
#include <QAction>
#include <QMenu>

class ObjectActions : public QObject
{
    Q_OBJECT
public:
    explicit ObjectActions(QWidget *parentWindow, QObject *parent = nullptr);

    QAction* startObjectAction;
    QAction* stopObjectAction;
    QAction* eventObjectAction;

    QAction* statementObjectAction;
    QAction* conditionalObjectAction;
    QAction* interactionObjectAction;

    void populateMenu(QMenu *menu);
    QToolBar* createToolBar(QWidget *parent);

    //temporarily here
    enum class ObjectType {
        Start,
        Stop,
        Event,
        Statement,
        Conditional,
        Interaction
    };
    Q_ENUM(ObjectType)

signals:
    void objectRequested(ObjectType type);

private slots:
    void onObjectTriggered();

private:
    QAction *createObjectAction(const QString &text, ObjectType type);
    QWidget *m_parentWindow;
};

#endif // OBJECTACTIONS_H
