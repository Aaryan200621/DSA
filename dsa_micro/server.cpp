// Browser History - simple C++ web server
// Build:  g++ -std=c++17 server.cpp -o server
// Run:    ./server      then open http://localhost:8080
// (Linux / macOS / WSL. Run it from inside the browser-history folder.)

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
using namespace std;

// ---------------- Data: doubly linked list ----------------

struct Node {
    string url;
    Node* prev = nullptr;
    Node* next = nullptr;
    Node(string u) : url(u) {}
};

class BrowserHistory {
    Node* head = nullptr;
    Node* current = nullptr;
    int count = 0;
    const int LIMIT = 10;
    chrono::steady_clock::time_point startedAt = chrono::steady_clock::now();

    void deleteForwardHistory() {
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
            head = current = new Node(url);
        } else {
            deleteForwardHistory();
            Node* newNode = new Node(url);
            current->next = newNode;
            newNode->prev = current;
            current = newNode;
        }
        count++;

        if (count > LIMIT) {            // drop the oldest page
            Node* temp = head;
            head = head->next;
            head->prev = nullptr;
            delete temp;
            count--;
        }
    }

    void deleteAllPages() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        current = nullptr;
        count = 0;
    }

    // Escape quotes and backslashes so the URL is safe inside JSON
    string esc(string s) {
        string out;
        for (char c : s) {
            if (c == '"' || c == '\\') out += '\\';
            if (c == '\n' || c == '\r') continue;
            out += c;
        }
        return out;
    }

    // Build the reply: success flag, message, and the whole list
    string reply(bool ok, string message, int found = -1) {
        string list = "";
        int index = -1, i = 0;
        for (Node* t = head; t != nullptr; t = t->next, i++) {
            if (t == current) index = i;
            if (i > 0) list += ",";
            list += "\"" + esc(t->url) + "\"";
        }
        string cur = current ? "\"" + esc(current->url) + "\"" : "null";
        bool canBack = current && current->prev;
        bool canForward = current && current->next;

        return string("{\"ok\":") + (ok ? "true" : "false") +
               ",\"message\":\"" + esc(message) + "\"" +
               ",\"current\":" + cur +
               ",\"index\":" + to_string(index) +
               ",\"found\":" + to_string(found) +
               ",\"limit\":" + to_string(LIMIT) +
               ",\"logTime\":\"" + esc(now()) + "\"" +
               ",\"activeSeconds\":" + to_string(activeSeconds()) +
               ",\"canBack\":" + (canBack ? "true" : "false") +
               ",\"canForward\":" + (canForward ? "true" : "false") +
               ",\"history\":[" + list + "]}";
    }

    string trim(string s) {
        size_t a = s.find_first_not_of(" \t\r\n");
        if (a == string::npos) return "";
        size_t b = s.find_last_not_of(" \t\r\n");
        return s.substr(a, b - a + 1);
    }

    string now() {
        time_t currentTime = time(nullptr);
        tm localTime{};
        localtime_r(&currentTime, &localTime);
        char text[20];
        strftime(text, sizeof(text), "%Y-%m-%d %H:%M:%S", &localTime);
        return text;
    }

    long activeSeconds() {
        return chrono::duration_cast<chrono::seconds>(
            chrono::steady_clock::now() - startedAt).count();
    }

public:
    ~BrowserHistory() { deleteAllPages(); }

    string state() { return reply(true, ""); }

    string visitPage(string url) {
        url = trim(url);
        if (url.empty()) return reply(false, "Enter a URL to visit.");
        addPage(url);
        return reply(true, "Page visited: " + url);
    }

    string goBack() {
        if (current && current->prev) {
            current = current->prev;
            return reply(true, "Current page: " + current->url);
        }
        return reply(false, "Beginning of history reached.");
    }

    string goForward() {
        if (current && current->next) {
            current = current->next;
            return reply(true, "Current page: " + current->url);
        }
        return reply(false, "End of history reached.");
    }

    string searchHistory(string url) {
        url = trim(url);
        int i = 0;
        for (Node* t = head; t != nullptr; t = t->next, i++)
            if (t->url == url)
                return reply(true, "URL found: " + url, i);
        return reply(false, "URL not found.");
    }

    string deletePage(string value) {
        value = trim(value);
        int index;
        try {
            index = stoi(value);
        } catch (...) {
            return reply(false, "Invalid history item.");
        }

        Node* target = head;
        for (int i = 0; target != nullptr && i < index; i++)
            target = target->next;
        if (target == nullptr || index < 0)
            return reply(false, "History item not found.");

        if (target->prev) target->prev->next = target->next;
        else head = target->next;
        if (target->next) target->next->prev = target->prev;

        if (target == current)
            current = target->next ? target->next : target->prev;
        delete target;
        count--;
        return reply(true, "History item deleted.");
    }

    string clearHistory() {
        deleteAllPages();
        ofstream("history.txt", ios::trunc);   // empty the saved file
        return reply(true, "Browser history cleared successfully.");
    }

    string saveHistory() {
        ofstream file("history.txt");
        if (!file) return reply(false, "Unable to save history.");
        for (Node* t = head; t != nullptr; t = t->next)
            file << t->url << "\n";
        return reply(true, "History saved successfully.");
    }

    string loadHistory() {
        ifstream file("history.txt");
        if (!file) return reply(false, "History file not found.");

        deleteAllPages();
        string url;
        while (getline(file, url)) {
            url = trim(url);
            if (!url.empty()) addPage(url);
        }
        if (head == nullptr) return reply(false, "History file is empty.");
        return reply(true, "History loaded successfully.");
    }
};

// ---------------- Web server ----------------

void sendResponse(int client, string type, string body, string status = "200 OK") {
    string response = "HTTP/1.1 " + status + "\r\n" +
                      "Content-Type: " + type + "\r\n" +
                      "Content-Length: " + to_string(body.size()) + "\r\n" +
                      "Connection: close\r\n\r\n" + body;
    send(client, response.c_str(), response.size(), 0);
}

// Send one of the files from the public folder
void sendFile(int client, string path) {
    if (path == "/") path = "/index.html";

    string type = "text/plain";
    if (path.find(".html") != string::npos) type = "text/html";
    if (path.find(".css")  != string::npos) type = "text/css";
    if (path.find(".js")   != string::npos) type = "application/javascript";

    ifstream file("public" + path, ios::binary);
    if (path.find("..") != string::npos || !file) {
        sendResponse(client, "text/plain", "Not found", "404 Not Found");
        return;
    }
    stringstream content;
    content << file.rdbuf();
    sendResponse(client, type, content.str());
}

int main() {
    BrowserHistory browser;
    int port = 8080;

    int server = socket(AF_INET, SOCK_STREAM, 0);
    int yes = 1;
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);   // this computer only
    addr.sin_port = htons(port);

    if (bind(server, (sockaddr*)&addr, sizeof(addr)) < 0 || listen(server, 5) < 0) {
        cout << "Could not start server on port " << port << " (is it already running?)\n";
        return 1;
    }
    cout << "Server running at http://localhost:" << port << "\n";

    while (true) {
        int client = accept(server, nullptr, nullptr);
        if (client < 0) continue;

        // 1. Read the request (headers first, then the body if there is one)
        string request;
        char buffer[4096];
        size_t headerEnd;
        while ((headerEnd = request.find("\r\n\r\n")) == string::npos) {
            int n = recv(client, buffer, sizeof(buffer), 0);
            if (n <= 0) break;
            request.append(buffer, n);
        }
        if (headerEnd == string::npos) { close(client); continue; }

        string headers = request.substr(0, headerEnd);
        string body = request.substr(headerEnd + 4);

        string lower = headers;
        transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
        size_t pos = lower.find("content-length:");
        size_t length = (pos == string::npos) ? 0 : stoul(lower.substr(pos + 15));

        while (body.size() < length) {
            int n = recv(client, buffer, sizeof(buffer), 0);
            if (n <= 0) break;
            body.append(buffer, n);
        }

        // 2. First line looks like:  POST /api/visit HTTP/1.1
        istringstream firstLine(headers);
        string method, path;
        firstLine >> method >> path;
        cout << method << " " << path << "\n";

        // 3. Decide what to answer
        string json = "application/json";
        if      (path == "/api/state")   sendResponse(client, json, browser.state());
        else if (path == "/api/visit")   sendResponse(client, json, browser.visitPage(body));
        else if (path == "/api/back")    sendResponse(client, json, browser.goBack());
        else if (path == "/api/forward") sendResponse(client, json, browser.goForward());
        else if (path == "/api/search")  sendResponse(client, json, browser.searchHistory(body));
        else if (path == "/api/delete")  sendResponse(client, json, browser.deletePage(body));
        else if (path == "/api/clear")   sendResponse(client, json, browser.clearHistory());
        else if (path == "/api/save")    sendResponse(client, json, browser.saveHistory());
        else if (path == "/api/load")    sendResponse(client, json, browser.loadHistory());
        else                             sendFile(client, path);

        close(client);
    }
}
