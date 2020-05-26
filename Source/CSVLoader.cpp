/*
  ==============================================================================

    CSVLoader.cpp
    Created: 10 May 2020 8:33:13pm
    Author:  YN

  ==============================================================================
*/

#include "CSVLoader.h"
#include "FileUtilities.h"

CSVLoader::CSVLoader()
{
    File csvFile = FileUtilities::getFromResources("final_annotations_files.csv");
    FileInputStream inputStream (csvFile);
    embyo_ids = StringArray();
    int line_idx = 0;
    while (! inputStream.isExhausted()) // [3]
    {
        String line = inputStream.readNextLine();
        StringArray broken = StringArray::fromTokens(line, ",", "");
        if(line_idx==0){
            data_labels = broken;
        }
        else {
            String id = broken[NAME_COLUMN_INDEX];
            embyo_ids.add(id);
            csvMap[id] = broken;
        }
        line_idx++;
    }
}

//gets all names of each data row. column of data rows secified by
//static constant CSVLoader::NAME_COLUMN_INDEX
StringArray& CSVLoader::getNames(){
    return embyo_ids;
}

//returns entire row of data - note that data row is not labelled.
//couple with the CSVLoader::dataLabels
StringArray& CSVLoader::getEntry(String& entryName){
    return csvMap[entryName];
}

//get the t events as a vecor of ints. Add -1 if it
//doesn't have an entry.
std::vector<int> CSVLoader::getTs(String& entryName){
    std::vector<int> result = std::vector<int>(8, -1);
    std::vector<String> events {"t2", "t3", "t4", "t5", "t6", "t7", "t8", "t9"};
    
    for (int i = 0; i < events.size(); i++){
        StringArray& csvRow = getEntry(entryName);
        String value = csvRow[data_labels.indexOf(events[i])];
        if (value.length() > 0) {
            int int_val = value.getIntValue();
            result[i] = int_val;
        }
    }
    
    //std::cout << entryName << "\n";
    for (int i = 0; i < events.size(); i++){
        //std::cout << result[i] << "\n";
    }
    
    return result;
}
