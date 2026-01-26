#include "titlebar.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QMouseEvent>

TitleBar::TitleBar(QWidget *parent)
    : QWidget(parent)
{
    setFixedHeight(32);

    setContentsMargins(0, 0, 0, 0);

    auto layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_titleWrapper = new QWidget(this);
    m_titleWrapper->setObjectName("titleWrapper");
    m_titleWrapper->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto wrapperLayout = new QHBoxLayout(m_titleWrapper);
    wrapperLayout->setContentsMargins(5, 0, 0, 0);
    wrapperLayout->setSpacing(0);

    QIcon icon = QIcon::fromTheme("format-justify-left");
    auto *iconLabel = new QLabel;
    iconLabel->setPixmap(icon.pixmap(16, 16));
    iconLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    iconLabel->setFixedSize(16, 16);
    wrapperLayout->addWidget(iconLabel, Qt::AlignLeft);

    m_title = new QLineEdit(m_titleWrapper);
    m_title->setReadOnly(true);
    m_title->setFocusPolicy(Qt::NoFocus);
    m_title->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_title->setTextMargins(0, 0, 0, 0);
    m_title->setFrame(false);
    m_title->setCursor(Qt::ArrowCursor);
    m_title->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    m_title->installEventFilter(this);

    wrapperLayout->addWidget(m_title, 0, Qt::AlignLeft);
    m_titleWrapper->installEventFilter(this);

    connect(m_title, &QLineEdit::textEdited,
            this, &TitleBar::titleChanged);
    connect(m_title, &QLineEdit::textChanged,
            this, &TitleBar::updateWidth);
    connect(m_title, &QLineEdit::editingFinished,
            this, &TitleBar::onEditingFinished);

    layout->addWidget(m_titleWrapper);
}

void TitleBar::setTitle(const QString &title) {
    if (title == "...") {
        m_title->setText("Unnamed block");
        return;
    }
    m_title->setText(title);
    updateWidth();
    emit titleChanged(m_title->text());
}

void TitleBar::updateWidth() {
    QFontMetrics fm(m_title->font());
    int textWidth = fm.horizontalAdvance(m_title->text());

    int cursorPadding = 20;
    int minWidth = 60;
    int totalWidth = qMax(minWidth, textWidth + cursorPadding);

    m_title->setFixedWidth(totalWidth);
}

void TitleBar::enterEditMode() {
    if (m_isEditing) return;

    m_isEditing = true;
    m_title->setReadOnly(false);
    m_title->setFocusPolicy(Qt::StrongFocus);
    m_title->setCursor(Qt::IBeamCursor);
    m_title->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    m_title->setFocus();
    m_title->selectAll();
}

void TitleBar::exitEditMode() {
    if (!m_isEditing) return;

    m_isEditing = false;
    m_title->setReadOnly(true);
    m_title->setFocusPolicy(Qt::NoFocus);
    m_title->setCursor(Qt::ArrowCursor);
    m_title->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    m_title->deselect();
    m_title->clearFocus();
}

void TitleBar::onEditingFinished() {
    exitEditMode();
}

bool TitleBar::eventFilter(QObject *watched, QEvent *event) {
    // Handle double-click on wrapper (since title has TransparentForMouseEvents when not editing)
    if (watched == m_titleWrapper && !m_isEditing) {
        if (event->type() == QEvent::MouseButtonDblClick) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                enterEditMode();
                return true;  // Event handled
            }
        }
    }

    // When in edit mode, handle double-click on the line edit itself
    if (watched == m_title && m_isEditing) {
        if (event->type() == QEvent::MouseButtonDblClick) {
            return false;
        }
    }

    return QWidget::eventFilter(watched, event);
}
