#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <cmath>
#include <random>
#include <map>

using namespace std;

class CSVProcessor {
public:
    vector<string> split(const string& s, char delimiter);
    vector<vector<string>> readCSV(const string& filename);
    void countNullsForColumn(const vector<vector<string>>& csvData, int columnIndex);
    void countNulls(const vector<vector<string>>& csvData);
    void fillNulls(vector<vector<string>>& csvData, int columnIndex, const string& newValue);
    vector<double> convertColumnToDouble(const vector<string>& column);

    template<typename T>
    vector<vector<double>> convertToDouble(const vector<vector<T>>& data);

    template<typename T>
    void displayCSV(const vector<vector<T>>& csvData);

    template<typename T>
    vector<T> extractColumn(vector<vector<T>>& csvData, int columnIndex);

    template<typename T>
    void removeColumn(vector<vector<T>>& csvData, int columnIndex);

    template<typename T>
    void addColumn(vector<vector<T>>& csvData, const vector<T>& newColumn);

    template<typename T>
    void addColumns(vector<vector<T>>& csvData, const vector<vector<T>>& newMatrix);

    template<typename T>
    vector<double> countData(const vector<T>& data);

    vector<vector<string>> appendCSV(vector<vector<string>> csvData1, vector<vector<string>>csvData2);

};

// Non-template member function definitions

vector<string> CSVProcessor::split(const string& s, char delimiter) {
    vector<string> tokens;
    stringstream ss(s);
    string token;
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

vector<vector<string>> CSVProcessor::readCSV(const string& filename) {
    vector<vector<string>> csvData;
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening file." << endl;
        return csvData;
    }

    // Skip the first line
    string firstLine;
    getline(file, firstLine);

    // Read the CSV data
    string line;
    while (getline(file, line)) {
        vector<string> row = split(line, ',');
        csvData.push_back(row);
    }

    file.close();

    return csvData;
}

void CSVProcessor::countNullsForColumn(const vector<vector<string>>& csvData, int columnIndex) {
    if (columnIndex >= csvData[0].size()) {
        cout << "Invalid column index." << endl;
        return;
    }

    int nullCount = 0;
    for (auto& row : csvData) {
        if (row.size() - 1 < columnIndex) {
            nullCount++;
        }
        else {
            if (row[columnIndex].empty()) {
                nullCount++;
            }
        }
    }
    cout << "Column " << columnIndex << ": " << nullCount << " null values" << endl;
}

void CSVProcessor::countNulls(const vector<vector<string>>& csvData) {
    int numColumns = csvData[0].size();

    for (int i = 0; i < numColumns; ++i) {
        countNullsForColumn(csvData, i);
    }
}

void CSVProcessor::fillNulls(vector<vector<string>>& csvData, int columnIndex, const string& newValue) {
    if (columnIndex >= csvData[0].size()) {
        cout << "Invalid column index." << endl;
        return;
    }

    for (auto& row : csvData) {
        if (row.size() - 1 < columnIndex) {
            row.resize(columnIndex + 1);
            row[columnIndex] = newValue;
        }
        else if (row[columnIndex].empty()) {
            row[columnIndex] = newValue;
        }
    }
    cout << "Null values in column " << columnIndex << " filled with: " << newValue << endl;
}

// Template member function definitions

template<typename T>
vector<vector<double>> CSVProcessor::convertToDouble(const vector<vector<T>>& data) {
    vector<vector<double>> doubleData;
    for (const auto& row : data) {
        vector<double> doubleRow;
        for (const auto& cell : row) {
            double value = stod(cell);
            doubleRow.push_back(value);
        }
        doubleData.push_back(doubleRow);
    }
    return doubleData;
}

template<typename T>
void CSVProcessor::displayCSV(const vector<vector<T>>& csvData) {
    const int maxColumns = 12; // Maximum number of columns to display at a time
    const int maxRows = 10;    // Maximum number of rows to display at a time
    const int colWidth = 15;   // Set a fixed column width for better alignment

    if (csvData.empty()) {
        cout << "Empty CSV data" << endl;
        return;
    }

    int totalColumns = csvData[0].size();
    int totalRows = csvData.size();

    for (int colStart = 0; colStart < totalColumns; colStart += maxColumns) {
        int colEnd = min(colStart + maxColumns, totalColumns);

        // Display the header for the current set of columns
        for (int col = colStart; col < colEnd; ++col) {
            cout << setw(colWidth) << "Column " << col;
        }
        cout << endl;

        // Display the data for the current set of columns
        for (int row = 0; row < min(totalRows, maxRows); ++row) {
            for (int col = colStart; col < colEnd; ++col) {
                cout << setw(colWidth) << csvData[row][col];
            }
            cout << endl;
        }

        // Print a separator line if there are more columns to display
        if (colEnd < totalColumns) {
            cout << string((colEnd - colStart) * colWidth, '-') << endl;
        }
    }
}



template<typename T>
vector<T> CSVProcessor::extractColumn(vector<vector<T>>& csvData, int columnIndex) {
    vector<T> columnData;
    for (auto& row : csvData) {
        if (row.size() > columnIndex) {
            columnData.push_back(row[columnIndex]);
        }
    }
    return columnData;
}

template<typename T>
void CSVProcessor::removeColumn(vector<vector<T>>& csvData, int columnIndex) {
    for (auto& row : csvData) {
        if (row.size() > columnIndex) {
            row.erase(row.begin() + columnIndex); // Remove the column from the row
        }
    }
}

template<typename T>
void CSVProcessor::addColumn(vector<vector<T>>& csvData, const vector<T>& newColumn) {
    if (csvData.size() != newColumn.size()) {
        cout << "Error: Number of rows in CSV data does not match number of elements in new column." << endl;
        return;
    }

    for (int i = 0; i < csvData.size(); ++i) {
        csvData[i].push_back(newColumn[i]); // Append new element to each row
    }
}

template<typename T>
void CSVProcessor::addColumns(vector<vector<T>>& csvData, const vector<vector<T>>& newMatrix) {
    if (csvData.size() != newMatrix.size()) {
        cout << "Error: Number of rows in CSV data does not match number of rows in the new matrix." << endl;
        return;
    }

    for (int col = 0; col < newMatrix[0].size(); ++col) {
        vector<T> columnData = extractColumn(newMatrix, col);
        addColumn(csvData, columnData);
    }
}


template<typename T>
vector<double> CSVProcessor::countData(const vector<T>& data) {
    map<T, double> counts;
    for (const auto& item : data) {
        counts[item]++;
    }

    vector<double> occurrences;
    for (const auto& pair : counts) {
        cout << pair.first << ": " << pair.second << " occurrences" << endl;
        occurrences.push_back(pair.second);
    }

    return occurrences;
}

vector<double> CSVProcessor::convertColumnToDouble(const vector<string>& column) {
    vector<double> doubleColumn;
    for (const auto& cell : column) {
        double value = stod(cell);
        doubleColumn.push_back(value);
    }
    return doubleColumn;
}

vector<vector<string>> CSVProcessor::appendCSV(vector<vector<string>> csvData1, vector<vector<string>>csvData2) {


    // Append csvData2 to csvData1
    csvData1.insert(csvData1.end(), csvData2.begin(), csvData2.end());

    return csvData1;
}
