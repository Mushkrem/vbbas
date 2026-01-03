#include "editactions.h"

#include <QFileDialog>
#include <QMessageBox>

EditActions::EditActions(QWidget *parentWindow, QObject *parent)
    : QObject{parent}, m_parentWindow(parentWindow)
{
    // *** Edit ***
    // Undo
    UndoAction = new QAction(tr("&Undo"), this);
    UndoAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::EditUndo));
    UndoAction->setShortcut(QKeySequence::fromString("Ctrl+Z"));
    connect(UndoAction, &QAction::triggered, this, &EditActions::onUndoTriggered);

    // Redo
    RedoAction = new QAction(tr("&Redo"), this);
    RedoAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::EditRedo));
    RedoAction->setShortcut(QKeySequence::fromString("Ctrl+Y"));
    connect(RedoAction, &QAction::triggered, this, &EditActions::onRedoTriggered);

    // Cut
    CutAction = new QAction(tr("&Cut"), this);
    CutAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::EditCut));
    CutAction->setShortcut(QKeySequence::fromString("Ctrl+X"));
    connect(CutAction, &QAction::triggered, this, &EditActions::onCutTriggered);

    // Copy
    CopyAction = new QAction(tr("&Copy"), this);
    CopyAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::EditCopy));
    CopyAction->setShortcut(QKeySequence::fromString("Ctrl+C"));
    connect(CopyAction, &QAction::triggered, this, &EditActions::onCopyTriggered);

    // Paste
    PasteAction = new QAction(tr("&Paste"), this);
    PasteAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::EditPaste));
    PasteAction->setShortcut(QKeySequence::fromString("Ctrl+V"));
    connect(PasteAction, &QAction::triggered, this, &EditActions::onPasteTriggered);

    // Delete
    DeleteAction = new QAction(tr("&Delete"), this);
    DeleteAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::EditDelete));
    DeleteAction->setShortcut(QKeySequence::fromString("Del"));
    connect(DeleteAction, &QAction::triggered, this, &EditActions::onDeleteTriggered);

    // SelectAll
    SelectAllAction = new QAction(tr("&Select All"), this);
    SelectAllAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::EditSelectAll));
    SelectAllAction->setShortcut(QKeySequence::fromString("Ctrl+A"));
    connect(SelectAllAction, &QAction::triggered, this, &EditActions::onSelectAllTriggered);

    // Find
    FindAction = new QAction(tr("&Find"), this);
    FindAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::EditFind));
    FindAction->setShortcut(QKeySequence::fromString("Ctrl+F"));
    connect(FindAction, &QAction::hovered, this, &EditActions::onFindTriggered);

    // Replace
    ReplaceAction = new QAction(tr("&Replace..."), this);
    ReplaceAction->setShortcut(QKeySequence::fromString("Alt+F4"));
    connect(ReplaceAction, &QAction::hovered, this, &EditActions::onReplaceTriggered);

    // Properties
    PropertiesAction = new QAction(tr("&Properties"), this);
    PropertiesAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::DocumentProperties));
    connect(PropertiesAction, &QAction::hovered, this, &EditActions::onPropertiesTriggered);
}

void EditActions::populateMenu(QMenu *menu) {
    menu->addAction(UndoAction);
    menu->addAction(RedoAction);

    menu->addSeparator();
    menu->addAction(CutAction);
    menu->addAction(CopyAction);
    menu->addAction(PasteAction);
    menu->addAction(DeleteAction);
    menu->addAction(SelectAllAction);

    menu->addSeparator();
    menu->addAction(FindAction);
    menu->addAction(ReplaceAction);

    menu->addSeparator();
    menu->addAction(PropertiesAction);
}

QToolBar* EditActions::createToolBar(QWidget *parent) {
    QToolBar *toolbar = new QToolBar(tr("Edit Toolbar"), parent);
    toolbar->setObjectName("edittoolbar");
    toolbar->setIconSize(QSize(16, 16));
    toolbar->setFloatable(false);
    toolbar->setMovable(true);

    toolbar->addAction(CutAction);
    toolbar->addAction(CopyAction);
    toolbar->addAction(PasteAction);

    toolbar->addSeparator();
    toolbar->addAction(UndoAction);
    toolbar->addAction(RedoAction);

    return toolbar;
}

void EditActions::onUndoTriggered() {
    emit undoRequested();
}

void EditActions::onRedoTriggered() {
    emit redoRequested();
}

void EditActions::onCutTriggered() {
    emit cutRequested();
}

void EditActions::onCopyTriggered() {
    emit copyRequested();
}

void EditActions::onPasteTriggered() {
    emit pasteRequested();
}

void EditActions::onDeleteTriggered() {
    emit deleteRequested();
}

void EditActions::onSelectAllTriggered() {
    emit selectAllRequested();
}

void EditActions::onFindTriggered() {
    emit findRequested();
}
void EditActions::onReplaceTriggered() {
    emit replaceRequested();
}

void EditActions::onPropertiesTriggered() {
    emit propertiesRequested();
}
