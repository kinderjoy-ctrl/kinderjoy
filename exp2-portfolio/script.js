function toggleTheme() {
    var body = document.body;
    var btn = document.getElementById('themeToggle');

    if (body.getAttribute('data-theme') === 'dark') {
        body.removeAttribute('data-theme');
        btn.textContent = '🌙';
    } else {
        body.setAttribute('data-theme', 'dark');
        btn.textContent = '☀️';
    }
}
