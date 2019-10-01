#include "Comparator.h"
#include "DrillingRecordComparator.h"

DrillingRecordComparator::DrillingRecordComparator(unsigned int _column)
{
    column = _column;
}

int DrillingRecordComparator::compare(const DrillingRecord& item1, const DrillingRecord& item2) const
{
    if (column == 0 || column  == 1)
    {
        return item1.getString(column).compare(item2.getString(column));
    }
    else
    {
        return (int) (item1.getNum(column - 2) - item2.getNum(column - 2));
    }
}