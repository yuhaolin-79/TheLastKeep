#include "cardmanager.h"

cardmanager::cardmanager(QObject *parent)
    : QAbstractItemModel(parent)
{}

QVariant cardmanager::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
}

QModelIndex cardmanager::index(int row, int column, const QModelIndex &parent) const
{
    // FIXME: Implement me!
}

QModelIndex cardmanager::parent(const QModelIndex &index) const
{
    // FIXME: Implement me!
}

int cardmanager::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

int cardmanager::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

QVariant cardmanager::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    return QVariant();
}
