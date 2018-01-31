#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>      
#include <boost/filesystem.hpp>
#include <map>

using namespace std;
namespace fs = boost::filesystem;

int main() {
    multimap<string, string> ext;
    multimap<string, string>::iterator map_iter;
    string log_name = "hiddenfiles_log.txt";
    char* r_filename = &log_name[0u];
    fstream result(r_filename, fstream::out);
    fs::path path = ".";
    fs::directory_iterator end;
    string res = "";
    string out = "";

    ext = {
        {".rar", "52 61 72 21 1A 07 00"},
        {".rar", "52 61 72 21 1A 07 01 00"},
        {".zip", "50 4B 03 04"},
        {".zip", "50 4B 05 06"},
        {".zip", "50 4B 07 08"},
    };

    for (fs::directory_iterator iter(path); iter != end; ++iter) {
        if (is_regular_file(iter->path()) && (iter->path().filename().string() != "hiddenfilefinder.exe") &&
            (iter->path().filename().string() != "hiddenfiles_log.txt")) {
            string file_name = iter->path().filename().string();
            char* filename = &file_name[0u];
            unsigned long addr = 0;
            fstream file(filename, fstream::binary | fstream::in);
            out = "possible ";
            stringstream ss;
            res = "";
            while (file.good()) {
                char buf;
                file.get(buf);
               ss << hex << setfill('0') << " " << setw(2) << (unsigned int)(unsigned char)buf; // cast into unsigned char to avoid sign extension
            }
            string temp = ss.str();

            for (map_iter = ext.begin(); map_iter != ext.end(); map_iter++) {
                std::transform(temp.begin(), temp.end(), temp.begin(), ::toupper);
                if (temp.find(map_iter->second) != string::npos) {
                    res = map_iter->first;
                    if (res != iter->path().extension().string()) {
                        if (out.find(res) == string::npos) out += map_iter->first + " | ";
                    }
                }
            }
            if (out == "possible ") out = "not found";
            std::cout << file_name + " -> " + out << endl;
            result << file_name + " -> " + out << endl;
        }
    }
    system("pause");
    std::cout << "Log saved at " + log_name;
    return 0;
}