# Browser History Navigation System

A browser history management system developed using **C++**, **Doubly Linked Lists**, and **HTML, CSS, and JavaScript**. The project demonstrates how browser navigation works internally using fundamental Data Structures and Algorithms.

The application provides a web-based interface to visit pages, navigate backward and forward, search history, delete individual entries, and save or load browsing history.

## Features

- **Visit a Page:** Add a URL to browser history.
- **Go Back:** Navigate to the previously visited page.
- **Go Forward:** Navigate to the next page in history.
- **Search History:** Search for a specific URL.
- **Delete History Entry:** Remove an individual URL from history.
- **Clear History:** Delete all stored browsing history.
- **Save History:** Store browsing history in a text file.
- **Load History:** Restore browsing history from a saved file.
- **History Limit:** Maintain a maximum of 10 pages by removing the oldest entry.
- **Active Time Tracking:** Display the time elapsed since the application started.
- **Timestamp Logging:** Display the latest operation timestamp.

## Tech Stack

| Technology | Purpose |
|---|---|
| C++ | Backend logic and server implementation |
| HTML5 | Frontend structure |
| CSS3 | User interface styling |
| JavaScript | Frontend interactions and API requests |
| TCP Sockets | HTTP server communication |
| File Handling | Persistent history storage |

## Data Structures Used

### 1. Doubly Linked List

The main data structure used in this project is a **Doubly Linked List**.

Each node stores:
- `url`: The visited page URL.
- `prev`: Pointer to the previous node.
- `next`: Pointer to the next node.

```cpp
struct Node {
    string url;
    Node* prev = nullptr;
    Node* next = nullptr;

    Node(string u) : url(u) {}
};
```

The `current` pointer tracks the currently selected page.

**Why a Doubly Linked List?**

- Supports backward navigation using the `prev` pointer.
- Supports forward navigation using the `next` pointer.
- Allows efficient insertion and deletion of nodes.
- Makes browser navigation easier to implement.

### 2. File Handling

The project uses `history.txt` to store browsing history.

- Save operation writes URLs to the file.
- Load operation reads URLs from the file.
- Clear operation removes all saved history.

## Project Structure

```text
dsa_micro/
│
├── public/
│   ├── index.html
│   ├── style.css
│   └── app.js
│
├── server.cpp
├── history.txt
├── DSA_Report_Browser_History_Navigation_System.pdf
└── README.md
```

## How to Run the Project

### Prerequisites

- C++ compiler supporting C++17
- Linux, macOS, or Windows Subsystem for Linux (WSL)
- A modern web browser

### Step 1: Clone the Repository

```bash
git clone https://github.com/Aaryan200621/DSA.git
```

### Step 2: Navigate to the Project Directory

```bash
cd DSA/dsa_micro
```

### Step 3: Compile the C++ Server

```bash
g++ -std=c++17 server.cpp -o server
```

### Step 4: Start the Server

```bash
./server
```

The server will start at:

```text
http://localhost:8080
```

### Step 5: Open the Application

Open the following URL in your browser:

```text
http://localhost:8080
```

You can now use the browser history management system.

## Backend API Endpoints

The C++ server handles frontend requests through the following HTTP endpoints:

| HTTP Endpoint | Method | Description |
|---|---|---|
| `/api/state` | GET | Returns current browser history state |
| `/api/visit` | POST | Adds a new URL |
| `/api/back` | GET | Moves to the previous page |
| `/api/forward` | GET | Moves to the next page |
| `/api/search` | POST | Searches for a URL |
| `/api/delete` | POST | Deletes a history entry using its index |
| `/api/clear` | GET | Clears all browser history |
| `/api/save` | GET | Saves history to `history.txt` |
| `/api/load` | GET | Loads history from `history.txt` |

## Working Principle

1. The user enters a URL through the frontend.
2. JavaScript sends an HTTP request to the C++ server.
3. The server processes the request using the `BrowserHistory` class.
4. The doubly linked list is updated based on the requested operation.
5. The server returns the updated state in JSON format.
6. JavaScript updates the frontend to display the latest browser history.

### Browser Navigation Logic

- When a new URL is visited, a new node is added.
- When the user moves backward, the `current` pointer moves to the previous node.
- When the user moves forward, the `current` pointer moves to the next node.
- When a new page is visited after moving backward, all forward-history nodes are deleted.
- When history exceeds 10 entries, the oldest node is removed.

## Learning Outcomes

This project demonstrates practical implementation of:

- Doubly Linked Lists
- Dynamic Memory Allocation
- Insertion and Deletion Operations
- Pointer Manipulation
- File Input/Output Operations
- TCP Socket Programming
- HTTP Request and Response Handling
- Client-Server Architecture
- JSON-Based Data Exchange

## Limitations

- The server currently listens only on the local machine (`localhost`).
- The application does not open or render actual external websites. It manages URL history.
- History is maintained in memory during execution and requires the save operation for persistence.
- The application supports a maximum of 10 history entries.

## Future Improvements

- Add support for multiple browser sessions.
- Add persistent automatic history storage.
- Implement URL validation.
- Add timestamps for individual history entries.
- Support configurable history limits.
- Add secure and concurrent HTTP request handling.

## Author

**Aaryan**

GitHub: [@Aaryan200621](https://github.com/Aaryan200621)

## License

This project is intended for educational purposes and demonstrates the practical use of Data Structures and Algorithms in C++.
