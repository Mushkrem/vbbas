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

private:
    QPlainTextEdit *m_textEdit;
    CSyntaxHighlighter *m_highlighter;
};


#endif // CODEEDITOR_H
