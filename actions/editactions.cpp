#include "editactions.h"

#include <QFileDialog>
#include <QMessageBox>

EditActions::EditActions(QWidget *parentWindow, QObject *parent)
    : QObject{parent}, m_parentWindow(parentWindow)
{
    // *** Edit ***
    // Undo
    undoAction = new QAction(tr("&Undo"), this);
    undoAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::EditUndo));
    undoAction->setShortcut(QKeySequence::fromString("Ctrl+Z"));
    connect(undoAction, &QAction::triggered, this, &EditActions::onUndoTriggered);

    // Redo
    redoAction = new QAction(tr("&Redo"), this);
    redoAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::EditRedo));
    redoAction->setShortcut(QKeySequence::fromString("Ctrl+Y"));
    connect(redoAction, &QAction::triggered, this, &EditActions::onRedoTriggered);

    // Cut
    cutAction = new QAction(tr("&Cut"), this);
    cutAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::EditCut));
    cutAction->setShortcut(QKeySequence::fromString("Ctrl+X"));
    connect(cutAction, &QAction::triggered, this, &EditActions::onCutTriggered);

    // Copy
    copyAction = new QAction(tr("&Copy"), this);
    copyAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::EditCopy));
    copyAction->setShortcut(QKeySequence::fromString("Ctrl+C"));
    connect(copyAction, &QAction::triggered, this, &EditActions::onCopyTriggered);

    // Paste
    pasteAction = new QAction(tr("&Paste"), this);
    pasteAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::EditPaste));
    pasteAction->setShortcut(QKeySequence::fromString("Ctrl+V"));
    connect(pasteAction, &QAction::triggered, this, &EditActions::onPasteTriggered);

    // Delete
    deleteAction = new QAction(tr("&Delete"), this);
    deleteAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::EditDelete));
    deleteAction->setShortcut(QKeySequence::fromString("Del"));
    connect(deleteAction, &QAction::triggered, this, &EditActions::onDeleteTriggered);

    // SelectAll
    selectAllAction = new QAction(tr("&Select All"), this);
    selectAllAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::EditSelectAll));
    selectAllAction->setShortcut(QKeySequence::fromString("Ctrl+A"));
    connect(selectAllAction, &QAction::triggered, this, &EditActions::onSelectAllTriggered);

    // Find
    findAction = new QAction(tr("&Find"), this);
    findAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::EditFind));
    findAction->setShortcut(QKeySequence::fromString("Ctrl+F"));
    connect(findAction, &QAction::hovered, this, &EditActions::onFindTriggered);

    // Replace
    replaceAction = new QAction(tr("&Replace..."), this);
    replaceAction->setShortcut(QKeySequence::fromString("Alt+F4"));
    connect(replaceAction, &QAction::hovered, this, &EditActions::onReplaceTriggered);

    // Properties
    propertiesAction = new QAction(tr("&Properties"), this);
    propertiesAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::DocumentProperties));
    connect(propertiesAction, &QAction::hovered, this, &EditActions::onPropertiesTriggered);
}

void EditActions::populateMenu(QMenu *menu) {
    menu->addAction(undoAction);
    menu->addAction(redoAction);

    menu->addSeparator();
    menu->addAction(cutAction);
    menu->addAction(copyAction);
    menu->addAction(pasteAction);
    menu->addAction(deleteAction);
    menu->addAction(selectAllAction);

    menu->addSeparator();
    menu->addAction(findAction);
    menu->addAction(replaceAction);

    menu->addSeparator();
    menu->addAction(propertiesAction);
}

QToolBar* EditActions::createToolBar(QWidget *parent) {
    QToolBar *toolbar = new QToolBar(tr("Edit Toolbar"), parent);
    toolbar->setObjectName("edittoolbar");
    toolbar->setIconSize(QSize(16, 16));
    toolbar->setFloatable(false);
    toolbar->setMovable(true);

    toolbar->addAction(cutAction);
    toolbar->addAction(copyAction);
    toolbar->addAction(pasteAction);

    toolbar->addSeparator();
    toolbar->addAction(undoAction);
    toolbar->addAction(redoAction);

    return toolbar;
}

void EditActions::setDocumentInfo(IDocumentInfo *documentInfo) {
    m_documentInfo = documentInfo;
    updateActionStates();
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

void EditActions::updateActionStates()
{
    if (!m_documentInfo) return;

    bool hasAnythingSelected = m_documentInfo->hasAnythingSelected();

    cutAction->setEnabled(hasAnythingSelected);
    copyAction->setEnabled(hasAnythingSelected);
    pasteAction->setEnabled(hasAnythingSelected);
    deleteAction->setEnabled(hasAnythingSelected);
    propertiesAction->setEnabled(hasAnythingSelected);
}
