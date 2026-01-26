#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>
#include <QLineEdit>
#include <QMouseEvent>

class TitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit TitleBar(QWidget *parent = nullptr);

    QString title() const { return m_title->text(); };
    void setTitle(const QString &title);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

signals:
    void titleChanged(const QString &title);

private slots:
    void updateWidth();
    void onEditingFinished();

private:
    void enterEditMode();
    void exitEditMode();

    QLineEdit *m_title;
    QWidget *m_titleWrapper;
    bool m_isEditing = false;
};

#endif // TITLEBAR_H
