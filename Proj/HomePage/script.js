// 配置搜索引擎
const searchEngines = {
    1: { name: 'Google', url: 'https://www.google.com/search?q=' },
    2: { name: 'Bing', url: 'https://www.bing.com/search?q=' },
    3: { name: 'Baidu', url: 'https://www.baidu.com/s?wd=' }
};

let currentEngine = 1;

// 书签数据（模块化配置）
const bookmarkCategories = {
    '工作': [
        { title: 'GitHub', url: 'https://github.com', icon: '💻' },
        { title: 'Stack Overflow', url: 'https://stackoverflow.com', icon: '❓' },
        { title: 'Notion', url: 'https://notion.so', icon: '📝' },
        { title: 'Slack', url: 'https://slack.com', icon: '💬' }
    ],
    '学习': [
        { title: 'MDN', url: 'https://developer.mozilla.org', icon: '📚' },
        { title: 'Coursera', url: 'https://coursera.org', icon: '🎓' },
        { title: 'W3Schools', url: 'https://www.w3schools.com', icon: '📖' },
        { title: 'Khan Academy', url: 'https://khanacademy.org', icon: '🏫' }
    ],
    '娱乐': [
        { title: 'YouTube', url: 'https://youtube.com', icon: '🎬' },
        { title: 'Netflix', url: 'https://netflix.com', icon: '📺' },
        { title: 'Spotify', url: 'https://spotify.com', icon: '🎵' },
        { title: 'Steam', url: 'https://store.steampowered.com', icon: '🎮' }
    ]
};

// DOM元素引用
const searchInput = document.getElementById('search-input');
const searchForm = document.getElementById('search-form');
const currentEngineEl = document.getElementById('current-engine');
const bookmarksGrid = document.getElementById('bookmarks-grid');

// 初始化页面
function init() {
    renderBookmarks();
    updateEngineDisplay();
    setupEventListeners();
}

// 渲染书签
function renderBookmarks() {
    bookmarksGrid.innerHTML = '';

    Object.entries(bookmarkCategories).forEach(([category, bookmarks]) => {
        // 添加分类标题
        const categoryTitle = document.createElement('div');
        categoryTitle.className = 'bookmark-category';
        categoryTitle.innerHTML = `<h3 style="grid-column: 1/-1; margin: 15px 0; color: #666;">${category}</h3>`;
        bookmarksGrid.appendChild(categoryTitle);

        // 添加书签项
        bookmarks.forEach((bookmark, index) => {
            const bookmarkEl = createBookmarkElement(bookmark, index);
            bookmarksGrid.appendChild(bookmarkEl);
        });
    });
}

// 创建单个书签元素
function createBookmarkElement(bookmark, index) {
    const bookmarkEl = document.createElement('a');
    bookmarkEl.href = bookmark.url;
    bookmarkEl.className = 'bookmark-item';
    bookmarkEl.setAttribute('tabindex', '0');
    bookmarkEl.innerHTML = `
        <div class="bookmark-icon">${bookmark.icon}</div>
        <div class="bookmark-title">${bookmark.title}</div>
    `;

    // 添加键盘导航支持
    bookmarkEl.addEventListener('keydown', (e) => {
        if (e.key === 'Enter' || e.key === ' ') {
            e.preventDefault();
            window.open(bookmark.url, '_blank');
        }
    });

    return bookmarkEl;
}

// 更新搜索引擎显示
function updateEngineDisplay() {
    currentEngineEl.textContent = searchEngines[currentEngine].name;
}

// 切换搜索引擎
function switchSearchEngine(engineNumber) {
    if (searchEngines[engineNumber]) {
        currentEngine = engineNumber;
        updateEngineDisplay();
        searchInput.focus();
    }
}

// 处理搜索提交
function handleSearch(e) {
    e.preventDefault();
    const query = searchInput.value.trim();
    if (query) {
        const searchUrl = searchEngines[currentEngine].url + encodeURIComponent(query);
        window.open(searchUrl, '_blank');
        searchInput.value = '';
    }
}

// 键盘导航书签
let currentBookmarkIndex = -1;
const bookmarkItems = [];

function updateBookmarkItems() {
    bookmarkItems.length = 0;
    document.querySelectorAll('.bookmark-item').forEach(item => {
        bookmarkItems.push(item);
    });
}

function selectBookmark(index) {
    // 移除之前的选中状态
    bookmarkItems.forEach(item => item.classList.remove('keyboard-selected'));

    // 设置新的选中状态
    if (index >= 0 && index < bookmarkItems.length) {
        currentBookmarkIndex = index;
        bookmarkItems[index].classList.add('keyboard-selected');
        bookmarkItems[index].focus();
    }
}

function navigateBookmarks(direction) {
    updateBookmarkItems();

    if (bookmarkItems.length === 0) return;

    if (currentBookmarkIndex === -1) {
        // 首次导航，选择第一个
        selectBookmark(0);
    } else {
        let newIndex;
        if (direction === 'next') {
            newIndex = (currentBookmarkIndex + 1) % bookmarkItems.length;
        } else if (direction === 'prev') {
            newIndex = currentBookmarkIndex === 0 ? bookmarkItems.length - 1 : currentBookmarkIndex - 1;
        }
        selectBookmark(newIndex);
    }
}

// 设置事件监听器
function setupEventListeners() {
    // 搜索表单提交
    searchForm.addEventListener('submit', handleSearch);

    // 全局键盘快捷键
    document.addEventListener('keydown', (e) => {
        // 搜索引擎切换快捷键 (Ctrl+1/2/3)
        if (e.ctrlKey && !e.shiftKey && !e.altKey) {
            if (e.key >= '1' && e.key <= '3') {
                e.preventDefault();
                switchSearchEngine(parseInt(e.key));
                return;
            }
        }

        // 书签键盘导航 (Tab键和方向键)
        if (e.key === 'Tab' && !e.ctrlKey && !e.shiftKey) {
            e.preventDefault();
            navigateBookmarks('next');
        }

        if (e.key === 'ArrowRight' || e.key === 'ArrowDown') {
            e.preventDefault();
            navigateBookmarks('next');
        }

        if (e.key === 'ArrowLeft' || e.key === 'ArrowUp') {
            e.preventDefault();
            navigateBookmarks('prev');
        }

        // 如果在书签上按Enter，打开链接
        if (e.key === 'Enter' && document.activeElement.classList.contains('bookmark-item')) {
            e.preventDefault();
            window.open(document.activeElement.href, '_blank');
        }

        // 快速聚焦搜索框 (Ctrl+K 或 Ctrl+L)
        if ((e.ctrlKey && e.key === 'k') || (e.ctrlKey && e.key === 'l')) {
            e.preventDefault();
            searchInput.focus();
            searchInput.select();
        }
    });

    // 点击书签时更新当前选中索引
    document.addEventListener('click', (e) => {
        if (e.target.classList.contains('bookmark-item')) {
            updateBookmarkItems();
            currentBookmarkIndex = bookmarkItems.indexOf(e.target);
        }
    });
}

// 页面加载完成后初始化
document.addEventListener('DOMContentLoaded', init);

// 导出配置函数（方便后续扩展）
window.addBookmarkCategory = function(categoryName, bookmarks) {
    bookmarkCategories[categoryName] = bookmarks;
    renderBookmarks();
};

window.addSearchEngine = function(key, name, url) {
    if (key >= 1 && key <= 9) {
        searchEngines[key] = { name, url };
    }
};
