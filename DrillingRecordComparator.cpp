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
        double num1 = item1.getNum(column - 2);
        double num2 = item2.getNum(column - 2);
        if (num1 < num2)
        {
            return -1;
        }
        else if (num1 > num2)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}