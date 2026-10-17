#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class BrowserHistory;
void displayHistory(BrowserHistory& browser);

class Node {
    string url;
    Node* prev;
    Node* next;

public:
    Node(string u) {
        url = u;
        prev = nullptr;
        next = nullptr;
    }

    friend class BrowserHistory;
    friend void displayHistory(BrowserHistory& browser);
};

class BrowserHistory {
    Node* head;
    Node* current;
    int count;
    const int LIMIT = 10;

    void deleteForwardHistory() {
        if (current == nullptr)
            return;

        Node* temp = current->next;

        while (temp != nullptr) {
            Node* nextNode = temp->next;
            delete temp;
            count--;
            temp = nextNode;
        }

        current->next = nullptr;
    }

    void addPage(string url) {
        if (current == nullptr) {
            head = new Node(url);
            current = head;
        } else {
            deleteForwardHistory();

            Node* newNode = new Node(url);
            current->next = newNode;
            newNode->prev = current;
            current = newNode;
        }

        count++;

        if (count > LIMIT) {
            Node* temp = head;
            head = head->next;
            head->prev = nullptr;
            delete temp;
            count--;
        }
    }

    void deleteAllPages() {
        Node* temp = head;

        while (temp != nullptr) {
            Node* nextNode = temp->next;
            delete temp;
            temp = nextNode;
        }

        head = nullptr;
        current = nullptr;
        count = 0;
    }

public:
    BrowserHistory() {
        head = nullptr;
        current = nullptr;
        count = 0;
    }

    ~BrowserHistory() {
        deleteAllPages();
    }

    void visitPage(string url) {
        addPage(url);
        cout << "Page visited: " << url << "\n";
    }

    void goBack() {
        if (current != nullptr && current->prev != nullptr) {
            current = current->prev;
            cout << "Current page: " << current->url << "\n";
        } else {
            cout << "Beginning of history reached.\n";
        }
    }

    void goForward() {
        if (current != nullptr && current->next != nullptr) {
            current = current->next;
            cout << "Current page: " << current->url << "\n";
        } else {
            cout << "End of history reached.\n";
        }
    }

    void showCurrentPage() {
        if (current == nullptr)
            cout << "No page is currently open.\n";
        else
            cout << "Current page: " << current->url << "\n";
    }

    void searchHistory(string url) {
        Node* temp = head;

        while (temp != nullptr) {
            if (temp->url == url) {
                cout << "URL found: " << url << "\n";
                return;
            }

            temp = temp->next;
        }

        cout << "URL not found.\n";
    }

    void clearHistory() {
        deleteAllPages();

        ofstream file("history.txt", ios::trunc);
        if (!file) {
            cout << "History cleared from memory, but saved file could not be cleared.\n";
            return;
        }

        file.close();
        cout << "Browser history cleared successfully.\n";
    }

    void saveHistory() {
        ofstream file("history.txt");

        if (!file) {
            cout << "Unable to save history.\n";
            return;
        }

        Node* temp = head;

        while (temp != nullptr) {
            file << temp->url << "\n";
            temp = temp->next;
        }

        file.close();
        cout << "History saved successfully.\n";
    }

    void loadHistory() {
        ifstream file("history.txt");

        if (!file) {
            cout << "History file not found.\n";
            return;
        }

        deleteAllPages();

        string url;

        while (getline(file, url)) {
            if (!url.empty())
                addPage(url);
        }

        file.close();

        if (head == nullptr)
            cout << "History file is empty.\n";
        else
            cout << "History loaded successfully.\n";
    }

    friend void displayHistory(BrowserHistory& browser);
};

void displayHistory(BrowserHistory& browser) {
    Node* temp = browser.head;

    if (temp == nullptr) {
        cout << "Browser history is empty.\n";
        cout << "-------------------------------------\n";
        return;
    }

    while (temp != nullptr) {
        cout << temp->url;

        if (temp->next != nullptr)
            cout << "  <---->  ";

        temp = temp->next;
    }

    cout << "\n-------------------------------------\n";
}

int main() {
    BrowserHistory browser;
    int choice;
    string url;

    do {
        cout << "\nBrowser History Menu\n";
        cout << "--------------------\n";
        cout << "1. Visit New Page\n";
        cout << "2. Go Back\n";
        cout << "3. Go Forward\n";
        cout << "4. Show Current Page\n";
        cout << "5. Display History\n";
        cout << "6. Search URL\n";
        cout << "7. Clear History\n";
        cout << "8. Save History\n";
        cout << "9. Load History\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input.\n";
            break;
        }

        switch (choice) {
            case 1:
                cout << "Enter URL: ";
                cin >> url;
                browser.visitPage(url);
                break;

            case 2:
                browser.goBack();
                break;

            case 3:
                browser.goForward();
                break;

            case 4:
                browser.showCurrentPage();
                break;

            case 5:
                displayHistory(browser);
                break;

            case 6:
                cout << "Enter URL to search: ";
                cin >> url;
                browser.searchHistory(url);
                break;

            case 7:
                browser.clearHistory();
                break;

            case 8:
                browser.saveHistory();
                break;

            case 9:
                browser.loadHistory();
                break;

            case 0:
                cout << "Exiting program.\n";
                break;

            default:
                cout << "Invalid choice.\n";
        }

    } while (choice != 0);

    return 0;
}
