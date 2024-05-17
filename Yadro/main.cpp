#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <queue>

struct Table {
    int table_Id;
    bool is_Occupied;
    std::string start_time;
    int total_minutes_occupied;
    int earnings;
};

// Функция для вычисления разницы во времени в минутах
int timeDifferenceInMinutes(const std::string& start, const std::string& end) {
    int start_hours = std::stoi(start.substr(0, 2));
    int start_minutes = std::stoi(start.substr(3, 2));
    int end_hours = std::stoi(end.substr(0, 2));
    int end_minutes = std::stoi(end.substr(3, 2));
    return (end_hours * 60 + end_minutes) - (start_hours * 60 + start_minutes);
}

// Функция для форматирования времени из минут в формат "часы:минуты"
std::string formatTime(int total_minutes) {
    int hours = total_minutes / 60;
    int minutes = total_minutes % 60;
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << hours << ":"
        << std::setw(2) << std::setfill('0') << minutes;
    return oss.str();
}

int main() {
    std::string filename;
    std::cout << "Введите имя файла: ";
    std::cin >> filename;
    
    std::ifstream inputFile(filename);
    if (!inputFile) {
        std::cerr << "Не удалось открыть файл" << std::endl;
        return 1;
    }
    
    int number_Table;
    inputFile >> number_Table;
    inputFile.ignore();
    
    std::string start_Time, end_Time;
    inputFile >> start_Time >> end_Time;
    inputFile.ignore();
    
    int cost_Table;
    inputFile >> cost_Table;
    inputFile.ignore();
    
    std::vector<Table> tables(number_Table);
    for (int i = 0; i < number_Table; ++i) {
        tables[i].table_Id = i + 1;
        tables[i].is_Occupied = false;
        tables[i].total_minutes_occupied = 0;
        tables[i].earnings = 0;
    }
    
    std::unordered_set<std::string> clientsInClub; // Множество для хранения имен клиентов в клубе
    std::unordered_map<std::string, int> clientsAtTable; // Мапа для хранения клиентов за столами
    std::queue<std::string> waitingQueue; // Очередь ожидания
    
    std::string line;
    std::cout << start_Time << std::endl;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        std::string client_Time;
        int client_Id;
        std::string client_Name;
        iss >> client_Time >> client_Id >> client_Name;
        
        if (client_Id == 1) { // Клиент пришел
            if (clientsInClub.find(client_Name) != clientsInClub.end()) {
                std::cout << client_Time << " " << client_Id << " " << client_Name << std::endl;
                std::cerr << client_Time << " 13 YouShallNotPass" << std::endl;
            } else if (client_Time < start_Time || client_Time > end_Time) {
                std::cout << client_Time << " " << client_Id << " " << client_Name << std::endl;
                std::cerr << client_Time << " 13 NotOpenYet" << std::endl;
            } else {
                clientsInClub.insert(client_Name);
                std::cout << client_Time << " 1 " << client_Name << std::endl;
            }
        } else if (client_Id == 2) { // Клиент сел за стол
            int table_Id;
            iss >> table_Id;
            if (clientsInClub.find(client_Name) == clientsInClub.end()) {
                std::cout << client_Time << " " << client_Id << " " << client_Name << std::endl;
                std::cerr << client_Time << " 13 ClientUnknown" << std::endl;
            } else if (tables[table_Id - 1].is_Occupied && clientsAtTable[client_Name] != table_Id) {
                std::cout << client_Time << " 2 " << client_Name << std::endl;
                std::cerr << client_Time << " 13 PlaceIsBusy" << std::endl;
            } else {
                if (clientsAtTable.find(client_Name) != clientsAtTable.end()) {
                    int previous_table_id = clientsAtTable[client_Name];
                    tables[previous_table_id - 1].is_Occupied = false;
                    int duration = timeDifferenceInMinutes(tables[previous_table_id - 1].start_time, client_Time);
                    tables[previous_table_id - 1].total_minutes_occupied += duration;
                    tables[previous_table_id - 1].earnings += (duration / 60 + (duration % 60 != 0)) * cost_Table;
                }
                tables[table_Id - 1].is_Occupied = true;
                tables[table_Id - 1].start_time = client_Time;
                clientsAtTable[client_Name] = table_Id;
                std::cout << client_Time << " 2 " << client_Name << " " << table_Id << std::endl;
            }
        } else if (client_Id == 3) { // Клиент ожидает
            bool hasFreeTable = false;
            for (const auto& table : tables) {
                if (!table.is_Occupied) {
                    hasFreeTable = true;
                    break;
                }
            }
            if (hasFreeTable) {
                std::cout << client_Time << " " << client_Id << " " << client_Name << std::endl;
                std::cerr << client_Time << " 13 ICanWaitNoLonger!" << std::endl;
            } else if (waitingQueue.size() >= number_Table) {
                std::cerr << client_Time << " 11 " << client_Name << std::endl;
            } else {
                waitingQueue.push(client_Name);
                std::cout << client_Time << " 3 " << client_Name << std::endl;
            }
        } else if (client_Id == 4) { // Клиент ушел
            if (clientsInClub.find(client_Name) == clientsInClub.end()) {
                std::cout << client_Time << " " << client_Id << " " << client_Name << std::endl;
                std::cerr << client_Time << " 13 ClientUnknown" << std::endl;
            } else {
                if (clientsAtTable.find(client_Name) != clientsAtTable.end()) {
                    int table_Id = clientsAtTable[client_Name];
                    tables[table_Id - 1].is_Occupied = false;
                    int duration = timeDifferenceInMinutes(tables[table_Id - 1].start_time, client_Time);
                    tables[table_Id - 1].total_minutes_occupied += duration;
                    tables[table_Id - 1].earnings += (duration / 60 + (duration % 60 != 0)) * cost_Table;
                    clientsAtTable.erase(client_Name);
                    
                    if (!waitingQueue.empty()) {
                        std::string nextClient = waitingQueue.front();
                        waitingQueue.pop();
                        tables[table_Id - 1].is_Occupied = true;
                        tables[table_Id - 1].start_time = client_Time;
                        clientsAtTable[nextClient] = table_Id;
                        std::cout << client_Time << " 2 " << nextClient << " " << table_Id << std::endl;
                    }
                }
                std::cout << client_Time << " 4 " << client_Name << std::endl;
                clientsInClub.erase(client_Name);
            }
        }
    }
    
    // Закрытие клуба - вывод всех оставшихся клиентов и заработка за столы
    for (const auto& client : clientsAtTable) {
        int table_Id = client.second;
        tables[table_Id - 1].is_Occupied = false;
        int duration = timeDifferenceInMinutes(tables[table_Id - 1].start_time, end_Time);
        tables[table_Id - 1].total_minutes_occupied += duration;
        tables[table_Id - 1].earnings += (duration / 60 + (duration % 60 != 0)) * cost_Table;
        std::cerr << end_Time << " 11 " << client.first << std::endl;
    }
    std::cout << end_Time << std::endl;
    for (const auto& table : tables) {
        std::string formattedTime = formatTime(table.total_minutes_occupied);
        std::cout << table.table_Id << " " << table.earnings << " " << formattedTime << std::endl;
    }
    
    inputFile.close();
    
    return 0;
}
