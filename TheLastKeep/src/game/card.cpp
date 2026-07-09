#include "game/card.h"

card::card(QObject *parent)
    : QAbstractItemModel(parent)
{}

QVariant card::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
    return QVariant();
}

QModelIndex card::index(int row, int column, const QModelIndex &parent) const
{
    // FIXME: Implement me!
    return QModelIndex();
}

QModelIndex card::parent(const QModelIndex &index) const
{
    // FIXME: Implement me!
    return QModelIndex();
}

int card::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
    return 0;
}

int card::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
    return 0;
}

QVariant card::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    return QVariant();
}
