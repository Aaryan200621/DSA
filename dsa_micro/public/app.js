// Talks to the C++ server over HTTP. All history logic lives on the server;
// this file only sends requests and draws the state that comes back.

const $ = (id) => document.getElementById(id);

const els = {
  back: $("btn-back"),
  forward: $("btn-forward"),
  visitForm: $("visit-form"),
  urlInput: $("url-input"),
  currentUrl: $("current-url"),
  chain: $("chain"),
  count: $("count"),
  limit: $("limit"),
  searchForm: $("search-form"),
  searchInput: $("search-input"),
  save: $("btn-save"),
  load: $("btn-load"),
  clear: $("btn-clear"),
  status: $("status"),
  activeTime: $("active-time"),
  logTime: $("log-time"),
};

async function api(path, body) {
  try {
    const res = await fetch(path, {
      method: body === undefined ? "GET" : "POST",
      headers: { "Content-Type": "text/plain" },
      body: body === undefined ? undefined : body,
    });
    return await res.json();
  } catch (err) {
    return null; // server unreachable
  }
}

function setStatus(message, ok) {
  els.status.textContent = message || "";
  els.status.className = "status" + (message ? (ok ? " ok" : " error") : "");
}

function render(state, highlightIndex = -1) {
  // current page
  if (state.current) {
    els.currentUrl.textContent = state.current;
    els.currentUrl.classList.remove("empty");
  } else {
    els.currentUrl.textContent = "No page is open";
    els.currentUrl.classList.add("empty");
  }

  // back / forward buttons
  els.back.disabled = !state.canBack;
  els.forward.disabled = !state.canForward;

  // linked list chain
  els.chain.innerHTML = "";
  if (state.history.length === 0) {
    const p = document.createElement("p");
    p.className = "empty-chain";
    p.textContent = "Browser history is empty. Visit a page to get started.";
    els.chain.appendChild(p);
  } else {
    state.history.forEach((url, i) => {
      if (i > 0) {
        const link = document.createElement("span");
        link.className = "link";
        link.textContent = "\u2194"; // double arrow = prev + next pointers
        link.setAttribute("aria-hidden", "true");
        els.chain.appendChild(link);
      }
      const item = document.createElement("div");
      item.className = "history-item";
      const node = document.createElement("div");
      node.className = "node";
      node.setAttribute("role", "listitem");
      node.title = url;
      node.textContent = url;
      if (i === state.index) node.classList.add("current-node");
      if (i === highlightIndex) {
        node.classList.add("found");
        node.scrollIntoView({ block: "nearest", inline: "center" });
      }
      item.appendChild(node);
      const deleteButton = document.createElement("button");
      deleteButton.className = "delete-btn";
      deleteButton.dataset.index = i;
      deleteButton.textContent = "Delete";
      deleteButton.title = "Delete this history item";
      item.appendChild(deleteButton);
      els.chain.appendChild(item);
    });
  }

  els.count.textContent = state.history.length + " of " + state.limit + " pages";
  els.limit.textContent = state.limit;
  els.activeTime.textContent = state.activeSeconds + " seconds";
  els.logTime.textContent = state.logTime;
}

async function run(path, body) {
  const state = await api(path, body);
  if (!state) {
    setStatus("Cannot reach the C++ server. Make sure it is running.", false);
    return null;
  }
  render(state, state.found);
  setStatus(state.message, state.ok);
  if (state.message) alert(state.message);
  return state;
}

// ---- events ----
els.visitForm.addEventListener("submit", async (e) => {
  e.preventDefault();
  const state = await run("/api/visit", els.urlInput.value);
  if (state && state.ok) els.urlInput.value = "";
});

els.back.addEventListener("click", () => run("/api/back", ""));
els.forward.addEventListener("click", () => run("/api/forward", ""));

els.chain.addEventListener("click", (e) => {
  if (!e.target.matches(".delete-btn")) return;
  const index = e.target.dataset.index;
  if (confirm("Delete this history item?")) run("/api/delete", index);
});

els.searchForm.addEventListener("submit", (e) => {
  e.preventDefault();
  run("/api/search", els.searchInput.value);
});

els.save.addEventListener("click", () => run("/api/save", ""));
els.load.addEventListener("click", () => run("/api/load", ""));

els.clear.addEventListener("click", () => {
  if (confirm("Clear all browser history? This also empties the saved file.")) {
    run("/api/clear", "");
  }
});

// ---- initial load ----
(async function init() {
  const state = await api("/api/state");
  if (!state) {
    setStatus("Cannot reach the C++ server. Make sure it is running.", false);
    return;
  }
  render(state);
})();
