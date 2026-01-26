#include "titlebar.h"
#include "codeeditor.h"
#include "csyntaxhighlighter.h"

#include <QFont>
#include <QFile>
#include <QVBoxLayout>
#include <QFontMetrics>
#include <QDialogButtonBox>
#include <QGraphicsDropShadowEffect>

CodeEditor::CodeEditor(QWidget *parent, const QString &title)
    : QDialog(parent)
{
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setTitle(title);

    // to-do: resizeable window

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    QWidget *container = new QWidget(this);
    container->setObjectName("container");

    // Can't add to .css files because codeeditor is not a child of registered objects in the stylingutils class.
    QFile file(":/resources/qss/codeeditor.css");
    if(file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        container->setStyleSheet(file.readAll());
        file.close();
    }

    applyDropShadowEffect(container);

    mainLayout->addWidget(container);

    QVBoxLayout *layout = new QVBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 5);

    auto titleBar = new TitleBar(this);
    titleBar->setTitle(title);
    titleBar->setObjectName("titleBar");
    titleBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    layout->addWidget(titleBar);

    m_textEdit = new QPlainTextEdit(this);
    QFont font("Cascadia Code", 13);
    // Swear I had a problem with any font rendering inside this QPlainTextEdit absolutely ugly.
    // They were are skewed, blurry pixelated and everything in between
    // Then I copied something from stackoverflow?? And it started working?
    // But when I removed it, it still worked??
    m_textEdit->setFont(font);
    m_textEdit->setTabStopDistance(QFontMetricsF(font).horizontalAdvance(' ') * 4);
    font.setHintingPreference(QFont::PreferNoHinting);
    font.setStyleStrategy(QFont::PreferAntialias);
    m_textEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
    m_textEdit->setFocus();

    qDebug() << "Actual font:" << m_textEdit->font().family();


    connect(titleBar, &TitleBar::titleChanged,
            this, &CodeEditor::onTitleChange);

    layout->addWidget(m_textEdit);

    m_highlighter = new CSyntaxHighlighter(m_textEdit->document());
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setContentsMargins(0, 0, 10, 5);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttonBox);
}

void CodeEditor::onTitleChange(const QString &title) {
    setTitle(title);
}

void CodeEditor::setCode(const QString &code)
{
    m_textEdit->setPlainText(code);
}

QString CodeEditor::code() const
{
    return m_textEdit->toPlainText();
}

void CodeEditor::mousePressEvent(QMouseEvent *event) {
    m_dragStart = event->globalPosition().toPoint();
}

void CodeEditor::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        QPoint deltaMove = event->globalPosition().toPoint() - m_dragStart;
        window()->move(window()->pos() + deltaMove);
        m_dragStart = event->globalPosition().toPoint();
    }
}

void CodeEditor::applyDropShadowEffect(QWidget* widget) {
    QGraphicsDropShadowEffect* dropShadowEffect = new QGraphicsDropShadowEffect(widget);
    dropShadowEffect->setColor(QColor(0, 0, 0, 50));
    dropShadowEffect->setBlurRadius(38.0);
    dropShadowEffect->setOffset(0, 0);

    widget->setGraphicsEffect(dropShadowEffect);
}
