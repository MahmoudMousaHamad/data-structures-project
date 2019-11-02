#include "Hasher.h"
#include "DrillingRecord.h"
#include "DrillingRecordHasher.h"

unsigned long DrillingRecordHasher::hash(const DrillingRecord& item) const
{
    unsigned long item_hash_value = 0;
    std::string timestamp = item.getString(1);
    for (unsigned int i = 0; i < 8; ++i)
    {
        item_hash_value += int(timestamp[i]);
    }
    return item_hash_value;
}