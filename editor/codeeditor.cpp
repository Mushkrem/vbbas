#include "codeeditor.h"
#include "csyntaxhighlighter.h"

#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QFont>
#include <QFontMetrics>

CodeEditor::CodeEditor(QWidget *parent, const QString &title)
    : QDialog(parent)
{
    setWindowTitle(title.isEmpty() ? tr("Code Editor") : title);
    setWindowIcon(QIcon(":/icons/light/code.svg"));

    QVBoxLayout *layout = new QVBoxLayout(this);

    m_textEdit = new QPlainTextEdit(this);

    QFont font("Cascadia Mono", 10);
    font.setStyleHint(QFont::Monospace);
    m_textEdit->setFont(font);
    m_textEdit->setTabStopDistance(QFontMetricsF(font).horizontalAdvance(' ') * 4);
    m_textEdit->setLineWrapMode(QPlainTextEdit::NoWrap);

    layout->addWidget(m_textEdit);

    m_highlighter = new CSyntaxHighlighter(m_textEdit->document());

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttonBox);
}

void CodeEditor::setCode(const QString &code)
{
    m_textEdit->setPlainText(code);
}

QString CodeEditor::code() const
{
    return m_textEdit->toPlainText();
}
