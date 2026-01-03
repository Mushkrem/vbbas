#include "tasksdock.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QFrame>
#include <QTableWidget>
#include <QHeaderView>

TasksDock::TasksDock(QWidget *parent)
    : DockBase("Tasks", parent) {}

QWidget* TasksDock::createContent()
{
    auto *w = new QWidget(this);
    w->setMinimumWidth(150);
    auto *layout = new QVBoxLayout(w);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(6);

    auto *compileAllBtn = new QPushButton("▶ Compile Design", w);
    compileAllBtn->setObjectName("compilebutton");
    compileAllBtn->setMinimumHeight(32);
    layout->addWidget(compileAllBtn);

    connect(compileAllBtn, &QPushButton::clicked, this, [this]() {
        emit taskStarted("Compile Design");
    });

    auto *line = new QFrame(w);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    layout->addWidget(line);

    auto *taskTable = new QTableWidget(w);
    taskTable->setObjectName("taskTable");
    taskTable->setColumnCount(3);
    taskTable->setHorizontalHeaderLabels({"Task", "Time", ""});
    taskTable->horizontalHeader()->setStretchLastSection(false);
    taskTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    taskTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    taskTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    taskTable->setColumnWidth(1, 60);
    taskTable->setColumnWidth(2, 60);
    taskTable->verticalHeader()->setVisible(false);
    taskTable->setSelectionMode(QAbstractItemView::NoSelection);
    taskTable->setShowGrid(false);
    taskTable->setFocusPolicy(Qt::NoFocus);

    QStringList tasks = {
        "Diagram Analysis",
        "Code Generation",
        "Interpreter",
        "Flow Simulation",
    };

    taskTable->setRowCount(tasks.size());

    for (int i = 0; i < tasks.size(); ++i) {
        // Task name
        auto *nameItem = new QTableWidgetItem(tasks[i]);
        nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
        taskTable->setItem(i, 0, nameItem);

        // Time
        auto *timeItem = new QTableWidgetItem("--");
        timeItem->setFlags(timeItem->flags() & ~Qt::ItemIsEditable);
        timeItem->setTextAlignment(Qt::AlignCenter);
        taskTable->setItem(i, 1, timeItem);

        // Run button
        auto *runBtn = new QPushButton("▶", w);
        runBtn->setMaximumSize(50, 24);
        taskTable->setCellWidget(i, 2, runBtn);

        connect(runBtn, &QPushButton::clicked, this, [this, tasks, i]() {
            emit taskStarted(tasks[i]);
        });
    }

    layout->addWidget(taskTable);

    return w;
}
