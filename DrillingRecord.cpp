#include <iomanip>
#include "DrillingRecord.h"
#include "ResizableArray.h"


std::ostream& operator<< (std::ostream& os, const DrillingRecord& record) {
    std::string seperator = ";";
    DrillingRecord recordToPrint = record;
    for (unsigned int i = 0; i < 2; ++i) {
        os << recordToPrint.getString(i) << seperator;
    }

    for (unsigned int i = 0; i < 15; ++i) {
        os << std::fixed << std::setprecision(2) << recordToPrint.getNum(i);
        os << seperator;
    }

    os << std::fixed << std::setprecision(2) << recordToPrint.getNum(15);

    return os;
}

DrillingRecord::DrillingRecord() {}

void DrillingRecord::addNum(double num) {
	nums[numCtr] = num;
	numCtr++;
}

void DrillingRecord::addString(std::string string) {
	strings[this->strCtr] = string;
	strCtr++;
}

double DrillingRecord::getNum(unsigned int index) const {
	return nums[index];
}

std::string DrillingRecord::getString(unsigned int index) const {
	return strings[index];
}