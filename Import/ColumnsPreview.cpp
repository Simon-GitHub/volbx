#include "ColumnsPreview.h"

#include <QDebug>
#include <QTableWidgetItem>

#include "Datasets/DatasetDefinition.h"

ColumnsPreview::ColumnsPreview(QWidget* parent) :
    QTableWidget(parent)
{
    setSelectionBehavior(QAbstractItemView::SelectColumns);

    connect(this,
            &QTableWidget::itemSelectionChanged,
            this,
            &ColumnsPreview::onItemSelectionChanged);
}

void ColumnsPreview::setDatasetDefinitionSampleInfo(const DatasetDefinition& datasetDefinition)
{
    clear();
    setColumnCount(0);
    setRowCount(0);

    int columns = datasetDefinition.columnCount();
    setColumnCount(columns);

    QStringList labels;
    labels.reserve(columns);
    for (int i = 0; i < columns; ++i)
    {
        labels.append(datasetDefinition.getColumnName(i));
    }

    setHorizontalHeaderLabels(labels);

    const QVector<QVector<QVariant> >* sampleData = datasetDefinition.getSampleData();

    int rows = sampleData->size();
    setRowCount(rows);

    Q_ASSERT(columns == sampleData->at(0).size());

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < columns; ++j)
        {
            QTableWidgetItem* item = new QTableWidgetItem(sampleData->at(i).at(j).toString());
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            setItem(i, j, item);
        }
    }
}

void ColumnsPreview::clearDataAndDisable()
{
    setEnabled(false);
    clear();
    setColumnCount(0);
    setRowCount(0);
}

void ColumnsPreview::selectCurrentColumn(int column)
{
    QTableWidgetItem* itemToSelect = item(0, column);
    if (nullptr != itemToSelect && !itemToSelect->isSelected())
    {
        selectColumn(column);
    }
}

void ColumnsPreview::onItemSelectionChanged()
{
    QList<QTableWidgetItem*> selectedItemsList = selectedItems();

    if (selectedItemsList.isEmpty())
    {
        return;
    }

    QTableWidgetItem* selectedItem = selectedItems().first();
    if (nullptr != selectedItem)
    {
        Q_EMIT currentColumnNeedSync(selectedItem->column());
    }
}
