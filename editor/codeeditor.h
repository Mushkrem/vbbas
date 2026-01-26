#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QDialog>
#include <QPlainTextEdit>

class CSyntaxHighlighter;

class CodeEditor : public QDialog
{
    Q_OBJECT

public:
    explicit CodeEditor(QWidget *parent = nullptr, const QString &title = nullptr);

    void setCode(const QString &code);
    QString code() const;

    void setTitle(const QString &title) { m_title = title; };
    QString title() const { return m_title; };

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

protected slots:
    void onTitleChange(const QString &title);

private:
    QPoint m_dragStart;

    QPlainTextEdit *m_textEdit;
    CSyntaxHighlighter *m_highlighter;

    void applyDropShadowEffect(QWidget* widget);

    QString m_title;
};


#endif // CODEEDITOR_H
