// script.js – Front‑end logic mirroring the C++ BrowserHistory

// History data structures (mirrors the C++ doubly linked list)
class Node {
  constructor(url) {
    this.url = url;
    this.prev = null;
    this.next = null;
  }
}

class BrowserHistory {
  constructor(limit = 10) {
    this.head = null; // oldest page
    this.current = null; // current page pointer
    this.count = 0;
    this.LIMIT = limit;
  }

  // Internal: delete forward history when a new page is visited
  _deleteForward() {
    if (!this.current) return;
    let temp = this.current.next;
    while (temp) {
      const next = temp.next;
      temp.prev = null;
      temp.next = null;
      temp = next;
      this.count--;
    }
    this.current.next = null;
  }

  _addPage(url) {
    if (!this.current) {
      this.head = new Node(url);
      this.current = this.head;
    } else {
      this._deleteForward();
      const newNode = new Node(url);
      this.current.next = newNode;
      newNode.prev = this.current;
      this.current = newNode;
    }
    this.count++;
    // enforce limit – drop oldest node
    if (this.count > this.LIMIT) {
      const oldHead = this.head;
      this.head = this.head.next;
      if (this.head) this.head.prev = null;
      oldHead.next = null;
      this.count--;
    }
  }

  visit(url) {
    this._addPage(url);
    showMessage(`Page visited: ${url}`);
    renderHistory();
  }

  back() {
    if (this.current && this.current.prev) {
      this.current = this.current.prev;
      showMessage(`Current page: ${this.current.url}`);
    } else {
      showMessage('Beginning of history reached.');
    }
    renderHistory();
  }

  forward() {
    if (this.current && this.current.next) {
      this.current = this.current.next;
      showMessage(`Current page: ${this.current.url}`);
    } else {
      showMessage('End of history reached.');
    }
    renderHistory();
  }

  showCurrent() {
    if (!this.current) {
      showMessage('No page is currently open.');
    } else {
      showMessage(`Current page: ${this.current.url}`);
    }
  }

  display() {
    if (!this.head) {
      showMessage('Browser history is empty.');
      return;
    }
    let out = '';
    let temp = this.head;
    while (temp) {
      out += `${temp.url}`;
      if (temp.next) out += '  <--->  ';
      temp = temp.next;
    }
    document.getElementById('history').textContent = out;
  }

  search(url) {
    let temp = this.head;
    while (temp) {
      if (temp.url === url) {
        showMessage(`URL found: ${url}`);
        return;
      }
      temp = temp.next;
    }
    showMessage('URL not found.');
  }

  clear() {
    // reset everything and clear stored file
    this.head = this.current = null;
    this.count = 0;
    localStorage.removeItem('browserHistory');
    showMessage('Browser history cleared successfully.');
    renderHistory();
  }

  async save() {
    const urls = [];
    let temp = this.head;
    while (temp) {
      urls.push(temp.url);
      temp = temp.next;
    }

    try {
      localStorage.setItem('browserHistory', JSON.stringify(urls));

      if (typeof window.showSaveFilePicker === 'function') {
        const fileHandle = await window.showSaveFilePicker({
          suggestedName: 'history.txt',
          types: [{
            description: 'Text files',
            accept: { 'text/plain': ['.txt'] }
          }]
        });
        const file = await fileHandle.createWritable();
        await file.write(urls.join('\n'));
        await file.close();
        showMessage('History saved to history.txt.');
        return;
      }

      const blob = new Blob([urls.join('\n')], { type: 'text/plain;charset=utf-8' });
      const downloadUrl = URL.createObjectURL(blob);
      const link = document.createElement('a');
      link.href = downloadUrl;
      link.download = 'history.txt';
      document.body.appendChild(link);
      link.click();
      link.remove();
      setTimeout(() => URL.revokeObjectURL(downloadUrl), 1000);
      showMessage('History downloaded as history.txt.');
    } catch (error) {
      if (error.name === 'AbortError') {
        showMessage('History save cancelled.');
        return;
      }

      console.error('Unable to save browser history:', error);
      showMessage('Unable to save history.txt.');
    }
  }

  load() {
    const data = localStorage.getItem('browserHistory');
    if (!data) {
      showMessage('History file not found.');
      return;
    }
    const urls = JSON.parse(data);
    // rebuild the linked list
    this.head = this.current = null;
    this.count = 0;
    for (const u of urls) {
      this._addPage(u);
    }
    if (this.head) {
      showMessage('History loaded successfully.');
    } else {
      showMessage('History file is empty.');
    }
    renderHistory();
  }
}

// UI helper – show a temporary message
function showMessage(msg) {
  const el = document.getElementById('message');
  el.textContent = msg;
  // fade out after a couple of seconds
  setTimeout(() => { el.textContent = ''; }, 3000);
}

function renderHistory() {
  // reuse the display() method but capture its output into the element
  const historyDiv = document.getElementById('history');
  if (!browser.head) {
    historyDiv.textContent = '';
    return;
  }
  let out = '';
  let temp = browser.head;
  while (temp) {
    out += `${temp.url}`;
    if (temp.next) out += '  <--->  ';
    temp = temp.next;
  }
  historyDiv.textContent = out;
}

// Instantiate the history object
const browser = new BrowserHistory();

// ----- Event listeners -----
document.getElementById('visitBtn').addEventListener('click', () => {
  const url = document.getElementById('urlInput').value.trim();
  if (url) browser.visit(url);
});

document.getElementById('backBtn').addEventListener('click', () => browser.back());

document.getElementById('forwardBtn').addEventListener('click', () => browser.forward());

document.getElementById('currentBtn').addEventListener('click', () => browser.showCurrent());

document.getElementById('clearBtn').addEventListener('click', () => browser.clear());

document.getElementById('saveBtn').addEventListener('click', () => browser.save());

document.getElementById('loadBtn').addEventListener('click', () => browser.load());

document.getElementById('searchBtn').addEventListener('click', () => {
  const query = document.getElementById('searchInput').value.trim();
  if (query) browser.search(query);
});

// Load any persisted history on page load automatically
window.addEventListener('load', () => {
  browser.load();
});
