#pragma once

#include <QAbstractItemView>
#include <QFile>

class LogFileDumper : public QAbstractItemView {
    Q_OBJECT
public:
    LogFileDumper(QString filename, QWidget *widget = nullptr);
private slots:
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>()) override;
private:
    QRect visualRect(const QModelIndex &index) const override;
    void scrollTo(const QModelIndex &index, QAbstractItemView::ScrollHint hint = QAbstractItemView::EnsureVisible) override;
    QModelIndex indexAt(const QPoint &point) const override;
    int horizontalOffset() const override;
    int verticalOffset() const override;
    bool isIndexHidden(const QModelIndex &index) const override;
    void setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command) override;
    QRegion visualRegionForSelection(const QItemSelection &selection) const override;
    QModelIndex moveCursor(QAbstractItemView::CursorAction cursorAction, Qt::KeyboardModifiers modifiers) override;
private:
    QFile file;
};
