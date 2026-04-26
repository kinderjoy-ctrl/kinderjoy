/**
 * WeatherNow — AJAX Weather App
 * ─────────────────────────────────────────────────────────────
 * APIs used (both 100% FREE — NO API key required):
 *
 *  1. Open-Meteo Geocoding  https://geocoding-api.open-meteo.com
 *     → Converts a city name into latitude/longitude + country info
 *
 *  2. Open-Meteo Weather    https://api.open-meteo.com
 *     → Returns real-time & hourly weather for given coordinates
 *
 * Docs: https://open-meteo.com/en/docs
 * ─────────────────────────────────────────────────────────────
 *
 * AJAX technique used: XMLHttpRequest (readyState / onreadystatechange)
 * Two chained AJAX calls are made for each search:
 *   Step 1 → Geocoding  (city name → lat/lon)
 *   Step 2 → Weather    (lat/lon  → current conditions)
 * ─────────────────────────────────────────────────────────────
 */

// ── API Endpoints ────────────────────────────────────────────
const GEO_URL = 'https://geocoding-api.open-meteo.com/v1/search';
const WEATHER_URL = 'https://api.open-meteo.com/v1/forecast';

// ── DOM references ───────────────────────────────────────────
const cityInput = document.getElementById('cityInput');
const searchBtn = document.getElementById('searchBtn');
const btnSpinner = document.getElementById('btnSpinner');
const weatherCard = document.getElementById('weatherCard');
const messageBox = document.getElementById('messageBox');
const messageIcon = document.getElementById('messageIcon');
const messageText = document.getElementById('messageText');

const cityName = document.getElementById('cityName');
const cityCountry = document.getElementById('cityCountry');
const dateTime = document.getElementById('dateTime');
const weatherIcon = document.getElementById('weatherIcon');
const weatherDesc = document.getElementById('weatherDesc');
const tempValue = document.getElementById('tempValue');
const feelsLike = document.getElementById('feelsLike');
const humidity = document.getElementById('humidity');
const windSpeed = document.getElementById('windSpeed');
const pressure = document.getElementById('pressure');
const visibility = document.getElementById('visibility');
const clouds = document.getElementById('clouds');
const sunrise = document.getElementById('sunrise');
const sunset = document.getElementById('sunset');
const windDir = document.getElementById('windDir');
const lastUpdated = document.getElementById('lastUpdated');

// ── State ────────────────────────────────────────────────────
let currentUnit = 'celsius';   // 'celsius' | 'fahrenheit'
let currentCity = '';          // last searched city name
let lastGeo = null;        // cached geocode result

// ── WMO Weather Code mapping ─────────────────────────────────
// Open-Meteo uses WMO 4677 weather codes
const WMO_CODES = {
    0: { label: 'Clear Sky', icon: '☀️', id: 800 },
    1: { label: 'Mainly Clear', icon: '🌤️', id: 801 },
    2: { label: 'Partly Cloudy', icon: '⛅', id: 802 },
    3: { label: 'Overcast', icon: '☁️', id: 804 },
    45: { label: 'Foggy', icon: '🌫️', id: 741 },
    48: { label: 'Icy Fog', icon: '🌫️', id: 741 },
    51: { label: 'Light Drizzle', icon: '🌦️', id: 300 },
    53: { label: 'Moderate Drizzle', icon: '🌦️', id: 301 },
    55: { label: 'Heavy Drizzle', icon: '🌦️', id: 302 },
    61: { label: 'Slight Rain', icon: '🌧️', id: 500 },
    63: { label: 'Moderate Rain', icon: '🌧️', id: 501 },
    65: { label: 'Heavy Rain', icon: '🌧️', id: 502 },
    71: { label: 'Slight Snow', icon: '🌨️', id: 600 },
    73: { label: 'Moderate Snow', icon: '❄️', id: 601 },
    75: { label: 'Heavy Snow', icon: '❄️', id: 602 },
    77: { label: 'Snow Grains', icon: '🌨️', id: 611 },
    80: { label: 'Slight Rain Showers', icon: '🌦️', id: 520 },
    81: { label: 'Moderate Rain Showers', icon: '🌧️', id: 521 },
    82: { label: 'Violent Rain Showers', icon: '⛈️', id: 522 },
    85: { label: 'Snow Showers', icon: '🌨️', id: 620 },
    86: { label: 'Heavy Snow Showers', icon: '🌨️', id: 621 },
    95: { label: 'Thunderstorm', icon: '⛈️', id: 200 },
    96: { label: 'Thunderstorm w/ Hail', icon: '⛈️', id: 201 },
    99: { label: 'Thunderstorm w/ Heavy Hail', icon: '⛈️', id: 202 },
};

function wmoInfo(code) {
    return WMO_CODES[code] || { label: 'Unknown', icon: '🌡️', id: 0 };
}

// ── Utility helpers ──────────────────────────────────────────

/** Wind degrees → compass direction string */
function degreesToCompass(deg) {
    const dirs = ['N', 'NNE', 'NE', 'ENE', 'E', 'ESE', 'SE', 'SSE', 'S', 'SSW', 'SW', 'WSW', 'W', 'WNW', 'NW', 'NNW'];
    return dirs[Math.round(deg / 22.5) % 16];
}

/** Format a date string (ISO) as 'Weekday, D Mon YYYY\nHH:MM local time' */
function formatDateTime(isoString) {
    const d = new Date(isoString);
    const days = ['Sunday', 'Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday'];
    const months = ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec'];
    const hh = String(d.getHours()).padStart(2, '0');
    const mm = String(d.getMinutes()).padStart(2, '0');
    return `${days[d.getDay()]}, ${d.getDate()} ${months[d.getMonth()]} ${d.getFullYear()}<br>${hh}:${mm} local time`;
}

/** Show error or info banner */
function showMessage(text, type = 'error') {
    messageBox.hidden = false;
    messageBox.className = `message-box ${type === 'info' ? 'info' : ''}`;
    messageIcon.textContent = type === 'info' ? 'ℹ️' : '⚠️';
    messageText.textContent = text;
    weatherCard.hidden = true;
}

function hideMessage() { messageBox.hidden = true; }

/** Toggle loading state on the search button */
function setLoading(on) {
    searchBtn.disabled = on;
    btnSpinner.hidden = !on;
    searchBtn.querySelector('.btn-text').textContent = on ? 'Searching…' : 'Search';
}

// ── AJAX helper — wraps XMLHttpRequest in a clean function ───

/**
 * ajaxGet(url, onSuccess, onError)
 *
 * Sends an asynchronous GET request using XMLHttpRequest.
 * Calls onSuccess(parsedJSON) on HTTP 200, onError(status) otherwise.
 *
 * This is the core AJAX mechanism used for both API calls.
 */
function ajaxGet(url, onSuccess, onError) {
    const xhr = new XMLHttpRequest();           // 1. Create the XHR object

    console.log('[WeatherNow] XHR GET:', url);

    xhr.open('GET', url, true);                 // 2. Open — true = async

    xhr.timeout = 10000;                        // 3. 10-second timeout

    // 4. Track state changes (AJAX hallmark: readyState 0→1→2→3→4)
    xhr.onreadystatechange = function () {
        if (xhr.readyState !== 4) return;       //    Only act when DONE (4)

        console.log('[WeatherNow] Response status:', xhr.status);
        console.log('[WeatherNow] Response text (first 300):', xhr.responseText.substring(0, 300));

        // status 0 can occur on some browsers when opening from file://
        if (xhr.status === 200 || xhr.status === 0) {
            try {
                const json = JSON.parse(xhr.responseText);
                onSuccess(json);
            } catch (e) {
                console.error('[WeatherNow] JSON parse error:', e.message);
                onError('parse_error: ' + xhr.responseText.substring(0, 120));
            }
        } else {
            onError(xhr.status);
        }
    };

    xhr.onerror = () => { console.error('[WeatherNow] XHR network error'); onError('network'); };
    xhr.ontimeout = () => { console.error('[WeatherNow] XHR timeout'); onError('timeout'); };

    xhr.send();                                 // 5. Fire the request!
}


// ── Step 1: Geocode city name → lat/lon ─────────────────────

function geocodeCity(cityQuery) {
    if (!cityQuery.trim()) {
        showMessage('Please enter a city name.', 'error');
        return;
    }

    hideMessage();
    setLoading(true);

    const url = `${GEO_URL}?name=${encodeURIComponent(cityQuery)}&count=1&language=en&format=json`;

    // AJAX Call #1 — Geocoding
    ajaxGet(url,
        function onGeoSuccess(data) {
            if (!data.results || data.results.length === 0) {
                setLoading(false);
                showMessage(`City "${cityQuery}" not found. Check spelling and try again.`, 'error');
                return;
            }
            const geo = data.results[0];
            lastGeo = geo;
            // Proceed to Step 2 with coordinates
            fetchWeatherData(geo);
        },
        function onGeoError(status) {
            setLoading(false);
            if (status === 'network') showMessage('Network error — check your internet connection.', 'error');
            else if (status === 'timeout') showMessage('Request timed out. Please try again.', 'error');
            else showMessage(`Geocoding failed (${status}). Please try again.`, 'error');
        }
    );
}

// ── Step 2: Fetch weather for lat/lon ────────────────────────

function fetchWeatherData(geo) {
    const unitParam = currentUnit === 'celsius' ? 'celsius' : 'fahrenheit';

    // Build the URL manually — URLSearchParams encodes commas as %2C
    // which Open-Meteo rejects, so we use a template literal to keep
    // commas as literal characters in the comma-separated variable lists.
    const currentVars = [
        'temperature_2m', 'relative_humidity_2m', 'apparent_temperature',
        'weather_code', 'wind_speed_10m', 'wind_direction_10m',
        'surface_pressure', 'cloud_cover', 'visibility'
    ].join(',');

    const url = `${WEATHER_URL}?latitude=${geo.latitude}&longitude=${geo.longitude}` +
        `&current=${currentVars}` +
        `&daily=sunrise,sunset` +
        `&temperature_unit=${unitParam}` +
        `&wind_speed_unit=kmh` +
        `&timezone=auto` +
        `&forecast_days=1`;

    // AJAX Call #2 — Weather data
    ajaxGet(url,
        function onWeatherSuccess(data) {
            setLoading(false);
            currentCity = geo.name;
            renderWeather(data, geo);
        },
        function onWeatherError(status) {
            setLoading(false);
            if (status === 'network') showMessage('Network error while fetching weather.', 'error');
            else if (status === 'timeout') showMessage('Weather request timed out. Try again.', 'error');
            else showMessage(`Weather fetch failed (${status}).`, 'error');
        }
    );
}

// ── Render ───────────────────────────────────────────────────

function renderWeather(data, geo) {
    const c = data.current;          // current weather object
    const d = data.daily;            // daily (sunrise/sunset arrays)
    const u = data.current_units;    // unit strings from API

    // City & location
    const parts = [geo.country];
    if (geo.admin1) parts.unshift(geo.admin1);
    cityName.textContent = geo.name;
    cityCountry.textContent = `${parts.join(', ')} · ${geo.latitude.toFixed(2)}°N, ${geo.longitude.toFixed(2)}°E`;

    // Date & time (API returns ISO8601 local time automatically via timezone=auto)
    dateTime.innerHTML = formatDateTime(c.time);

    // Weather condition
    const info = wmoInfo(c.weather_code);
    weatherIcon.src = '';                       // no external icon needed
    weatherIcon.alt = info.label;
    weatherIcon.style.display = 'none';
    weatherDesc.textContent = info.label;
    // Display the big emoji icon instead
    let emojiEl = document.getElementById('weatherEmoji');
    if (!emojiEl) {
        emojiEl = document.createElement('div');
        emojiEl.id = 'weatherEmoji';
        emojiEl.style.cssText = 'font-size:5rem;line-height:1;filter:drop-shadow(0 0 16px rgba(79,158,255,0.5));animation:iconBob 3s ease-in-out infinite;';
        weatherIcon.parentNode.insertBefore(emojiEl, weatherIcon);
    }
    emojiEl.textContent = info.icon;

    // Temperature
    const unitSym = currentUnit === 'celsius' ? '°C' : '°F';
    const windUnit = 'km/h';
    tempValue.textContent = `${Math.round(c.temperature_2m)}${unitSym}`;
    feelsLike.textContent = `${Math.round(c.apparent_temperature)}${unitSym}`;

    // Stats
    humidity.textContent = `${c.relative_humidity_2m}%`;
    windSpeed.textContent = `${c.wind_speed_10m} ${windUnit}`;
    pressure.textContent = `${c.surface_pressure} hPa`;
    visibility.textContent = c.visibility !== undefined
        ? `${(c.visibility / 1000).toFixed(1)} km`
        : 'N/A';
    clouds.textContent = `${c.cloud_cover}%`;

    // Sunrise / Sunset — API returns e.g. "2025-03-09T06:12"
    function fmtTime(iso) {
        if (!iso) return 'N/A';
        const t = iso.split('T')[1];      // "06:12"
        return t ? t.substring(0, 5) : 'N/A';
    }
    sunrise.textContent = fmtTime(d.sunrise[0]);
    sunset.textContent = fmtTime(d.sunset[0]);

    // Wind direction
    windDir.textContent = c.wind_direction_10m !== undefined
        ? `${degreesToCompass(c.wind_direction_10m)} (${c.wind_direction_10m}°)`
        : 'N/A';

    // Last updated
    lastUpdated.textContent = new Date().toLocaleTimeString();

    // Unit button active state
    document.getElementById('btnCelsius').classList.toggle('active', currentUnit === 'celsius');
    document.getElementById('btnFahrenheit').classList.toggle('active', currentUnit === 'fahrenheit');

    // Show card
    weatherCard.hidden = false;
}


// ── Event Listeners ──────────────────────────────────────────

searchBtn.addEventListener('click', () => {
    geocodeCity(cityInput.value.trim());
});

cityInput.addEventListener('keydown', (e) => {
    if (e.key === 'Enter') geocodeCity(cityInput.value.trim());
});

document.querySelectorAll('.city-chip').forEach(chip => {
    chip.addEventListener('click', () => {
        cityInput.value = chip.dataset.city;
        geocodeCity(chip.dataset.city);
    });
});

document.getElementById('btnCelsius').addEventListener('click', () => {
    if (currentUnit === 'celsius') return;
    currentUnit = 'celsius';
    if (lastGeo) fetchWeatherData(lastGeo);
});

document.getElementById('btnFahrenheit').addEventListener('click', () => {
    if (currentUnit === 'fahrenheit') return;
    currentUnit = 'fahrenheit';
    if (lastGeo) fetchWeatherData(lastGeo);
});

// ── Boot ─────────────────────────────────────────────────────
window.addEventListener('DOMContentLoaded', () => {
    cityInput.focus();
});
