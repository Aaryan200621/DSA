# Browser History Navigation and Search

## 1. Project Overview

The Browser History Navigation and Search project includes a native Qt Widgets frontend and a menu-driven C++ console version. Both simulate browser history using a Doubly Linked List and support visiting URLs, backward and forward navigation, searching, clearing, and saving or loading history.

## 2. Objectives

- To implement a Doubly Linked List in C++.
- To visit and store web page URLs.
- To navigate backward and forward through browser history.
- To search for a URL in the stored history.
- To display and clear browser history.
- To save and load history using file handling.
- To handle the beginning and end of history.
- To maintain a maximum history size of 10 pages.

## 3. Technologies Used

- **Programming Language:** C++
- **Graphical Interface:** Qt 5 Widgets
- **Build System:** qmake
- **Data Structure:** Doubly Linked List
- **Concepts:** Classes, Objects, Pointers, Dynamic Memory Allocation
- **File Handling:** `ifstream` and `ofstream`
- **IDE:** Visual Studio Code
- **Compiler:** GCC (g++) for the console version; a compiler kit matching Qt for the GUI.

## 4. Project Structure

```text
BrowserHistorySimulator/
│
├── main_.cpp
├── qt_main.cpp
├── BrowserHistorySimulator.pro
├── README.md
├── main_.exe
└── history.txt
```

- `main_.cpp` — Console version of the simulator.
- `qt_main.cpp` — Qt Widgets graphical frontend and linked-list implementation.
- `BrowserHistorySimulator.pro` — qmake project for building the Qt application.
- `README.md` — Contains project documentation.
- `main_.exe` — Executable generated after compilation.
- `history.txt` — Stores saved browser history.

*Note: The executable is generated during compilation, and the history file is created when history is saved.*

## 5. Data Structure Used

The project uses a **Doubly Linked List**. Each node contains:

- `url` — Stores the web page URL.
- `prev` — Points to the previous node.
- `next` — Points to the next node.

### Example

```text
Google <----> Facebook <----> Amazon
```

The `prev` pointer supports backward navigation, while the `next` pointer supports forward navigation.

The program maintains a maximum of 10 pages. When the limit is exceeded, the oldest page is removed.

## 6. Build and Run the Qt Frontend

Install Qt with the **Qt Widgets** module and a compiler kit that matches the Qt installation. Open `BrowserHistorySimulator.pro` in Qt Creator, select the matching kit, then build and run the project.

To build from a matching Windows developer command prompt:

**MSVC kit:**

```powershell
qmake BrowserHistorySimulator.pro
nmake
```

**MinGW kit:**

```powershell
qmake BrowserHistorySimulator.pro
mingw32-make
```

The Qt application saves and loads `history.txt` beside its executable.

### Browser Frontend

Open `index.html` in a browser to use the HTML/JavaScript frontend. Its **Save**
button keeps the browser's local history and exports the current URLs, one per
line, to `history.txt`. Browsers with the File System Access API let you choose
where to save the file; other browsers download it to the default downloads
folder.

### Console Version

The original console program remains available. Compile and run it with GCC:

```powershell
g++ main_.cpp -o main_
.\main_.exe
```

## 7. Menu Options

When the program starts, the following menu appears:

```text
Browser History Menu
--------------------
1. Visit New Page
2. Go Back
3. Go Forward
4. Show Current Page
5. Display History
6. Search URL
7. Clear History
8. Save History
9. Load History
0. Exit
Enter your choice:
```

## 8. Operations and Usage

### 8.1 Visit New Page

**Purpose:** Adds a new URL to the browser history.

**Steps:**
1. Enter `1`.
2. Enter the URL.
3. The URL is added to the history and becomes the current page.

**Example:**

```text
Enter your choice: 1
Enter URL: www.google.com
Page visited: www.google.com
```

Repeat this operation to add more pages.

### 8.2 Go Back

**Purpose:** Moves to the previous page.

**Steps:**
1. Enter `2`.
2. The current page changes to the previous URL.

**Example output:**

```text
Current page: www.google.com
```

If the current page is the first page, the program displays:

```text
Beginning of history reached.
```

### 8.3 Go Forward

**Purpose:** Moves to the next page.

**Steps:**
1. Enter `3`.
2. The current page changes to the next URL, if available.

If the current page is the last page, the program displays:

```text
End of history reached.
```

### 8.4 Show Current Page

**Purpose:** Displays the currently selected URL.

**Steps:**
1. Enter `4`.
2. The program displays the current page.

**Example output:**

```text
Current page: www.facebook.com
```

If no page is open:

```text
No page is currently open.
```

### 8.5 Display History

**Purpose:** Displays the URLs stored in the doubly linked list.

**Steps:**
1. Enter `5`.
2. The program displays the browsing history in order.

**Example output:**

```text
www.google.com  <---->  www.facebook.com  <---->  www.amazon.com
-------------------------------------
```

If the history is empty:

```text
Browser history is empty.
-------------------------------------
```

### 8.6 Search URL

**Purpose:** Searches for a specific URL in the browser history.

**Steps:**
1. Enter `6`.
2. Enter the URL to search.
3. The program reports whether the URL exists.

**When found:**

```text
URL found: www.facebook.com
```

**When not found:**

```text
URL not found.
```

The search requires an exact URL match.

### 8.7 Clear History

**Purpose:** Deletes all URLs from the linked list and clears the saved history file.

**Steps:**
1. Enter `7`.
2. The program deletes all nodes.
3. The contents of `history.txt` are cleared.
4. Enter `5` to verify that the history is empty.

**Expected output:**

```text
Browser history cleared successfully.
```

### 8.8 Save History

**Purpose:** Saves the current browsing history to `history.txt`.

**Steps:**
1. Visit some pages.
2. Enter `8`.
3. The program writes the URLs to the file.

**Expected output:**

```text
History saved successfully.
```

The file is stored in the program's working directory.

### 8.9 Load History

**Purpose:** Loads previously saved URLs from `history.txt`.

**Steps:**
1. Save the history using option `8`.
2. Restart the program if needed.
3. Enter `9`.
4. Enter `5` to display the loaded history.

**Expected output:**

```text
History loaded successfully.
```

If the file does not exist:

```text
History file not found.
```

Loading replaces the current in-memory history. The last loaded URL becomes the current page. The current-page position is not saved separately.

### 8.10 Exit

**Purpose:** Terminates the program.

**Steps:**
1. Enter `0`.

**Expected output:**

```text
Exiting program.
```

## 9. Special Conditions

### Beginning of History

When the user selects Go Back on the first page, the program displays a message indicating that the beginning of history has been reached.

### End of History

When the user selects Go Forward on the last page, the program displays a message indicating that the end of history has been reached.

### History Limit

The maximum history size is 10 pages. When an additional page is visited after the limit is reached, the oldest page is removed.

### Visiting a New Page After Going Back

If the user moves backward and visits a new page, the forward-history entries are deleted.

**Example before visiting a new page:**

```text
Google <----> Facebook <----> Amazon
```

After moving back to Facebook and visiting YouTube:

```text
Google <----> Facebook <----> YouTube
```

Amazon is removed from the forward history.

## 10. Test Cases

| Test Case | Action | Expected Result |
|---|---|---|
| 1 | Visit a new URL | URL is added to history |
| 2 | Go Back from the first page | Beginning message appears |
| 3 | Go Forward from the last page | End message appears |
| 4 | Search for an existing URL | URL found |
| 5 | Search for a missing URL | URL not found |
| 6 | Clear history | History becomes empty |
| 7 | Save and reload history | Saved URLs are restored |
| 8 | Visit a new page after going back | Forward history is removed |
| 9 | Visit more than 10 pages | Oldest page is removed |

## 11. Advantages

- Demonstrates the practical use of a doubly linked list.
- Supports backward and forward navigation.
- Uses dynamic memory allocation.
- Provides URL searching and history deletion.
- Demonstrates basic file handling.
- Maintains a fixed maximum history size.

## 12. Limitations

- The application simulates browser history; it does not open websites.
- The maximum history size is 10 pages.
- URL searching requires an exact match.
- The current-page position is not stored separately.
- Only URLs are stored, not complete browser data.