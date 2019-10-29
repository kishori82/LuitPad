#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <vector>
#include <sstream>

class TableModel : public QAbstractTableModel
{
public:
  TableModel();

  int rowCount(const QModelIndex& parent) const;
  int columnCount(const QModelIndex& parent) const;
  QVariant data(const QModelIndex& index, int role) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;

protected:
  std::vector<std::vector<float> > Columns;
};



#endif // TABLEMODEL_H
