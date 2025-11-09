#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include <cstring>
#include <cstdlib>

using namespace std;

void listFiles(const string &path) {
    DIR *dir;
    struct dirent *entry;
    struct stat info;

    if ((dir = opendir(path.c_str())) == NULL) {
        perror("opendir");
        return;
    }

    cout << "\nFiles in directory: " << path << endl;
    cout << "---------------------------------------------" << endl;

    while ((entry = readdir(dir)) != NULL) {
        string fullPath = path + "/" + entry->d_name;
        stat(fullPath.c_str(), &info);

        if (S_ISDIR(info.st_mode))
            cout << "[DIR]  " << entry->d_name << endl;
        else
            cout << "       " << entry->d_name << endl;
    }

    closedir(dir);
}

void createFile(const string &filename) {
    ofstream file(filename);
    if (file)
        cout << "File created successfully: " << filename << endl;
    else
        cout << "Error creating file!" << endl;
}

void deleteFile(const string &filename) {
    if (remove(filename.c_str()) == 0)
        cout << "File deleted successfully!" << endl;
    else
        perror("Error deleting file");
}

void copyFile(const string &src, const string &dest) {
    ifstream in(src, ios::binary);
    ofstream out(dest, ios::binary);

    if (!in || !out) {
        cout << "Error copying file!" << endl;
        return;
    }

    out << in.rdbuf();
    cout << "File copied successfully!" << endl;
}

void moveFile(const string &src, const string &dest) {
    if (rename(src.c_str(), dest.c_str()) == 0)
        cout << "File moved successfully!" << endl;
    else
        perror("Error moving file");
}

void searchFile(const string &path, const string &target) {
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(path.c_str())) == NULL)
        return;

    while ((entry = readdir(dir)) != NULL) {
        string name = entry->d_name;
        if (name == "." || name == "..") continue;

        string fullPath = path + "/" + name;

        if (name.find(target) != string::npos)
            cout << "Found: " << fullPath << endl;

        struct stat info;
        stat(fullPath.c_str(), &info);

        if (S_ISDIR(info.st_mode))
            searchFile(fullPath, target);
    }
    closedir(dir);
}

void changePermissions(const string &filename, mode_t mode) {
    if (chmod(filename.c_str(), mode) == 0)
        cout << "Permissions changed successfully!" << endl;
    else
        perror("chmod");
}

int main() {
    string path = ".";
    int choice;
    string file1, file2, name;

    do {
        cout << "\n=== Linux File Explorer ===\n";
        cout << "1. List Files\n";
        cout << "2. Create File\n";
        cout << "3. Delete File\n";
        cout << "4. Copy File\n";
        cout << "5. Move File\n";
        cout << "6. Search File\n";
        cout << "7. Change File Permissions\n";
        cout << "8. Change Directory\n";
        cout << "9. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                listFiles(path);
                break;
            case 2:
                cout << "Enter filename to create: ";
                cin >> file1;
                createFile(path + "/" + file1);
                break;
            case 3:
                cout << "Enter filename to delete: ";
                cin >> file1;
                deleteFile(path + "/" + file1);
                break;
            case 4:
                cout << "Enter source file: ";
                cin >> file1;
                cout << "Enter destination file: ";
                cin >> file2;
                copyFile(path + "/" + file1, path + "/" + file2);
                break;
            case 5:
                cout << "Enter source file: ";
                cin >> file1;
                cout << "Enter destination file: ";
                cin >> file2;
                moveFile(path + "/" + file1, path + "/" + file2);
                break;
            case 6:
                cout << "Enter filename to search: ";
                cin >> name;
                searchFile(path, name);
                break;
            case 7:
                cout << "Enter filename to change permissions: ";
                cin >> file1;
                int mode;
                cout << "Enter new permission mode (e.g., 0644): ";
                cin >> oct >> mode;
                changePermissions(path + "/" + file1, mode);
                break;
            case 8:
                cout << "Enter new directory path: ";
                cin >> path;
                if (chdir(path.c_str()) == 0)
                    cout << "Changed directory to " << path << endl;
                else
                    perror("chdir");
                break;
            case 9:
                cout << "Exiting File Explorer..." << endl;
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
    } while (choice != 9);

    return 0;
}