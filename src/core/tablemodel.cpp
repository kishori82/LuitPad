/************************************************************************
# MIT License

Copyright (c) 2020 Kishori M. Konwar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#include "tablemodel.h"

TableModel::TableModel() : QAbstractTableModel() {
  std::vector<float> column1;
  column1.push_back(10);
  column1.push_back(20);
  column1.push_back(30);
  column1.push_back(40);

  Columns.push_back(column1);

  std::vector<float> column2;
  column2.push_back(50);
  column2.push_back(60);
  column2.push_back(70);
  column2.push_back(80);

  Columns.push_back(column2);
}

int TableModel::rowCount(const QModelIndex &parent) const {
  return Columns[0].size();
}

int TableModel::columnCount(const QModelIndex &parent) const {
  return Columns.size();
}

QVariant TableModel::data(const QModelIndex &index, int role) const {
  if (role == Qt::DisplayRole) {
    return Columns[index.column()][index.row()];
  }
  return QVariant::Invalid;
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation,
                                int role) const {

  if (role == Qt::DisplayRole) {
    std::stringstream ss;
    if (orientation == Qt::Horizontal) {
      ss << "H_" << section;
      return QString(ss.str().c_str());
    } else if (orientation == Qt::Vertical) {
      ss << "V_" << section;
      return QString(ss.str().c_str());
    }
  }

  return QVariant::Invalid;
}
