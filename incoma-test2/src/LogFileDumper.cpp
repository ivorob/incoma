#include <QDebug>
#include <QMessageBox>
#include "LogFileDumper.h"

LogFileDumper::LogFileDumper(QString filename, QWidget *widget)
    : QAbstractItemView(widget),
      file(filename)
{
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        throw std::runtime_error(QString("Cannot open file %1").arg(filename).toStdString());
    }
}

QRect
LogFileDumper::visualRect(const QModelIndex &index) const
{
    return QRect();
}

void
LogFileDumper::scrollTo(const QModelIndex &index, QAbstractItemView::ScrollHint hint)
{
}

QModelIndex
LogFileDumper::indexAt(const QPoint &point) const
{
    return QModelIndex();
}

int
LogFileDumper::horizontalOffset() const
{
    return int();
}

int
LogFileDumper::verticalOffset() const
{
    return int();
}

bool
LogFileDumper::isIndexHidden(const QModelIndex &index) const
{
    return bool();
}

void
LogFileDumper::setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command)
{
}

QRegion
LogFileDumper::visualRegionForSelection(const QItemSelection &selection) const
{
    return QRegion();
}

QModelIndex
LogFileDumper::moveCursor(QAbstractItemView::CursorAction cursorAction, Qt::KeyboardModifiers modifiers)
{
    return QModelIndex();
}

void
LogFileDumper::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    if (!topLeft.isValid()) {
        return;
    }

    QTextStream out(&this->file);
    for (QModelIndex currentIndex = topLeft;
         currentIndex.isValid() && currentIndex.row() <= bottomRight.row();
         currentIndex = currentIndex.siblingAtRow(currentIndex.row() + 1))
    {
        QVariant data = currentIndex.data();
        if (data.isValid()) {
            out << currentIndex.data().toString() << "\n";
        }
    }
}
