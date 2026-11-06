/* ==========================================================
   Dashboard IoT : app.js
   - Lit /api/sensors (même format qu'avant)
   - Garde un historique (sauvegardé dans le navigateur)
   - Cartes cliquables + graphique détaillé
   ========================================================== */

/* ---------- Configuration des capteurs ---------- */

const ICONS = {
    temperature: '<path d="M14 14.8V5a2 2 0 0 0-4 0v9.8a4 4 0 1 0 4 0z"/>',
    humidity:    '<path d="M12 3s6 6.2 6 10.5a6 6 0 0 1-12 0C6 9.200 12 3 12 3z"/>',
    light:       '<path d="M9 18h6M10 21h4M12 3a6 6 0 0 0-3.500 10.900c.700.600 1 1.400 1 2.100h5c0-.700.300-1.500 1-2.100A6 6 0 0 0 12 3z"/>',
    sound:       '<path d="M11 5 6 9H3v6h3l5 4V5zM15.500 8.500a5 5 0 0 1 0 7M18.500 5.500a9 9 0 0 1 0 13"/>'
};

const SENSORS = {
    temperature: {
        label: "Température", unit: "°C", digits: 1,
        zone(v) {
            if (v < 18)  return { text: "Frais",        level: "info" };
            if (v <= 26) return { text: "Confortable",  level: "ok" };
            return { text: "Chaud", level: "bad" };
        }
    },
    humidity: {
        label: "Humidité", unit: "%", digits: 1,
        zone(v) {
            if (v < 30)  return { text: "Sec",          level: "warn" };
            if (v <= 60) return { text: "Confortable",  level: "ok" };
            return { text: "Humide", level: "warn" };
        }
    },
    light: {
        label: "Lumière", unit: "", digits: 0, boolean: true,
        zone(v) { return v ? { text: "Allumé", level: "warn" } : { text: "Éteint", level: "" }; }
    },
    sound: {
        label: "Son", unit: "dB", digits: 0,
        zone(v) {
            if (v < 50)  return { text: "Calme",   level: "ok" };
            if (v <= 70) return { text: "Modéré",  level: "warn" };
            return { text: "Bruyant", level: "bad" };
        }
    }
};

const KEYS = Object.keys(SENSORS);
const MAX_TICKS = 900;                 // taille max de l'historique
const reduceMotion = window.matchMedia("(prefers-reduced-motion: reduce)").matches;

/* ---------- État ---------- */

const state = {
    ticks: [],                          // [{t, temperature, humidity, light, sound}]
    latest: {},                         // dernière réponse du serveur (pour les dates)
    selected: "temperature",
    range: 600000,
    paused: false,
    online: true,
    intervalMs: 2000,
    timer: null,
    shown: {}                           // valeur affichée (pour l'animation des nombres)
};

/* ---------- Utilitaires ---------- */

const $ = (sel, root = document) => root.querySelector(sel);

function cssVar(name) {
    return getComputedStyle(document.documentElement).getPropertyValue(name).trim();
}

function fmt(v, digits) {
    return v.toLocaleString("fr-FR", { minimumFractionDigits: digits, maximumFractionDigits: digits });
}

function timeLabel(t) {
    return new Date(t).toLocaleTimeString("fr-FR");
}

function save(key, value) {
    try { localStorage.setItem(key, JSON.stringify(value)); } catch (e) { /* stockage indisponible */ }
}

function load(key, fallback) {
    try {
        const raw = localStorage.getItem(key);
        return raw ? JSON.parse(raw) : fallback;
    } catch (e) { return fallback; }
}

/* Valeurs d'un capteur sur la période demandée */
function series(key, range = state.range) {
    const limit = Date.now() - range;
    return state.ticks.filter(k => k[key] !== undefined && k.t >= limit);
}

function stats(key, range) {
    const pts = series(key, range).map(k => k[key]);
    if (!pts.length) return null;
    const sum = pts.reduce((a, b) => a + b, 0);
    return { min: Math.min(...pts), max: Math.max(...pts), avg: sum / pts.length, n: pts.length };
}

/* ---------- Construction des cartes ---------- */

function buildCards() {
    const container = $("#cards");
    container.innerHTML = KEYS.map(key => {
        const s = SENSORS[key];
        return `
        <button class="sensor" type="button" data-sensor="${key}" style="--accent: var(--c-${key})" aria-pressed="false">
            <span class="sensor-head">
                <span class="sensor-icon">
                    <svg viewBox="0 0 24 24" width="20" height="20" fill="none" stroke="currentColor"
                         stroke-width="2" stroke-linecap="round" stroke-linejoin="round" aria-hidden="true">${ICONS[key]}</svg>
                </span>
                <span class="sensor-name">${s.label}</span>
                <span class="badge" data-role="zone">Aucune mesure</span>
            </span>
            <span class="sensor-value">
                <span data-role="value">--</span><span class="unit">${s.unit}</span>
            </span>
            <span class="sensor-trend" data-role="trend"></span>
            <canvas class="spark" data-role="spark"></canvas>
            <span class="sensor-stats" data-role="stats"></span>
            <span class="sensor-date" data-role="date">Aucune mesure</span>
        </button>`;
    }).join("");

    container.addEventListener("click", e => {
        const card = e.target.closest(".sensor");
        if (card) select(card.dataset.sensor);
    });
}

function select(key) {
    state.selected = key;
    document.querySelectorAll(".sensor").forEach(c =>
        c.setAttribute("aria-pressed", String(c.dataset.sensor === key)));
    $("#detail").style.setProperty("--accent", `var(--c-${key})`);
    updateDetail();
    save("ap4a-prefs", { selected: state.selected, range: String(state.range), intervalMs: state.intervalMs,
                         theme: document.documentElement.dataset.theme });
}

/* ---------- Animation d'un nombre ---------- */

function tweenNumber(el, from, to, digits, duration = 400) {
    if (reduceMotion || from === undefined || from === to) {
        el.textContent = fmt(to, digits);
        return;
    }
    const start = performance.now();
    function step(now) {
        const p = Math.min((now - start) / duration, 1);
        const eased = 1 - Math.pow(1 - p, 3);
        el.textContent = fmt(from + (to - from) * eased, digits);
        if (p < 1) requestAnimationFrame(step);
    }
    requestAnimationFrame(step);
}

/* ---------- Mini-graphique (sparkline) ---------- */

function drawSpark(canvas, values, color, stepped) {
    const dpr = window.devicePixelRatio || 1;
    const w = canvas.clientWidth, h = canvas.clientHeight;
    if (!w || !h) return;
    if (canvas.width !== Math.round(w * dpr)) {
        canvas.width = Math.round(w * dpr);
        canvas.height = Math.round(h * dpr);
    }
    const ctx = canvas.getContext("2d");
    ctx.setTransform(dpr, 0, 0, dpr, 0, 0);
    ctx.clearRect(0, 0, w, h);
    if (values.length < 2) return;

    let min = Math.min(...values), max = Math.max(...values);
    if (min === max) { min -= 1; max += 1; }
    const pad = 4;
    const x = i => pad + i * (w - 2 * pad) / (values.length - 1);
    const y = v => h - pad - (v - min) / (max - min) * (h - 2 * pad);

    ctx.beginPath();
    values.forEach((v, i) => {
        if (i === 0) ctx.moveTo(x(i), y(v));
        else {
            if (stepped) ctx.lineTo(x(i), y(values[i - 1]));
            ctx.lineTo(x(i), y(v));
        }
    });
    ctx.strokeStyle = color;
    ctx.lineWidth = 2;
    ctx.lineJoin = "round";
    ctx.stroke();

    ctx.lineTo(x(values.length - 1), h);
    ctx.lineTo(x(0), h);
    ctx.closePath();
    const grad = ctx.createLinearGradient(0, 0, 0, h);
    grad.addColorStop(0, color + "40");
    grad.addColorStop(1, color + "00");
    ctx.fillStyle = grad;
    ctx.fill();
}

/* ---------- Mise à jour des cartes ---------- */

function updateCards() {
    KEYS.forEach(key => {
        const s = SENSORS[key];
        const card = $(`.sensor[data-sensor="${key}"]`);
        const entry = state.latest[key] || {};
        const pts = series(key, 300000);                // 5 dernières minutes pour la sparkline
        const last = state.ticks.length ? state.ticks[state.ticks.length - 1][key] : undefined;

        const valueEl = $('[data-role="value"]', card);
        const zoneEl  = $('[data-role="zone"]', card);
        const trendEl = $('[data-role="trend"]', card);
        const statsEl = $('[data-role="stats"]', card);

        $('[data-role="date"]', card).textContent = entry.date ? `Mesure : ${entry.date}` : "Aucune mesure";

        if (last === undefined) {
            valueEl.textContent = "--";
            zoneEl.textContent = "Aucune mesure";
            zoneEl.dataset.level = "";
            return;
        }

        /* valeur principale */
        if (s.boolean) {
            valueEl.textContent = last ? "Allumé" : "Éteint";
            card.dataset.on = String(Boolean(last));
        } else {
            const prev = state.shown[key];
            tweenNumber(valueEl, prev, last, s.digits);
            if (prev !== undefined && prev !== last) {
                valueEl.classList.remove("flash");
                void valueEl.offsetWidth;                 // relance l'animation CSS
                valueEl.classList.add("flash");
            }
        }
        state.shown[key] = last;

        /* badge de zone */
        const z = s.zone(last);
        zoneEl.textContent = z.text;
        zoneEl.dataset.level = z.level;

        /* tendance : comparaison avec la moyenne des mesures précédentes */
        if (s.boolean) {
            const changed = [...pts].reverse().findIndex(k => k[key] !== last);
            if (changed === -1) trendEl.textContent = "Inchangé depuis le début de la mesure";
            else trendEl.textContent = `Depuis ${timeLabel(pts[pts.length - changed].t)}`;
            trendEl.dataset.dir = "";
        } else {
            const before = pts.slice(-7, -1).map(k => k[key]);
            if (before.length) {
                const diff = last - before.reduce((a, b) => a + b, 0) / before.length;
                const threshold = Math.pow(10, -s.digits) / 2;
                if (Math.abs(diff) < threshold) { trendEl.textContent = "Stable"; trendEl.dataset.dir = ""; }
                else {
                    trendEl.textContent = `${diff > 0 ? "▲ +" : "▼ "}${fmt(diff, s.digits)} ${s.unit}`;
                    trendEl.dataset.dir = diff > 0 ? "up" : "down";
                }
            } else trendEl.textContent = "";
        }

        /* min / max / moyenne */
        const st = stats(key, Infinity);
        statsEl.innerHTML = s.boolean
            ? `<span>Allumé ${Math.round(st.avg * 100)} % du temps</span>`
            : `<span>Min ${fmt(st.min, s.digits)}</span><span>Max ${fmt(st.max, s.digits)}</span><span>Moy. ${fmt(st.avg, s.digits)}</span>`;

        /* sparkline */
        drawSpark($('[data-role="spark"]', card), pts.map(k => k[key]), cssVar(`--c-${key}`), s.boolean);
    });
}

/* ---------- Graphique détaillé (Chart.js) ---------- */

let chart = null;

function buildChart() {
    chart = new Chart($("#mainChart"), {
        type: "line",
        data: { labels: [], datasets: [{ data: [], borderWidth: 2, pointRadius: 0, pointHoverRadius: 5, tension: 0.3, fill: true }] },
        options: {
            responsive: true,
            maintainAspectRatio: false,
            animation: false,
            interaction: { mode: "index", intersect: false },
            plugins: { legend: { display: false } },
            scales: {
                x: { grid: { display: false }, ticks: { maxTicksLimit: 6, maxRotation: 0 } },
                y: { grace: "10%" }
            }
        }
    });
}

function updateDetail() {
    const key = state.selected;
    const s = SENSORS[key];
    const color = cssVar(`--c-${key}`);
    const pts = series(key);

    $("#detailTitle").textContent = s.label;
    const last = state.ticks.length ? state.ticks[state.ticks.length - 1][key] : undefined;
    $("#detailZone").textContent = last === undefined ? "Aucune mesure" : `État actuel : ${s.zone(last).text.toLowerCase()}`;

    /* chiffres clés */
    const st = stats(key, state.range);
    $("#detailStats").innerHTML = !st ? "" : s.boolean
        ? `<div class="stat"><span>Temps allumé</span><strong>${Math.round(st.avg * 100)} %</strong></div>
           <div class="stat"><span>Mesures</span><strong>${st.n}</strong></div>`
        : `<div class="stat"><span>Minimum</span><strong>${fmt(st.min, s.digits)} ${s.unit}</strong></div>
           <div class="stat"><span>Maximum</span><strong>${fmt(st.max, s.digits)} ${s.unit}</strong></div>
           <div class="stat"><span>Moyenne</span><strong>${fmt(st.avg, s.digits)} ${s.unit}</strong></div>
           <div class="stat"><span>Mesures</span><strong>${st.n}</strong></div>`;

    /* graphique */
    $(".chart-wrap").classList.toggle("is-empty", pts.length < 2);
    const ds = chart.data.datasets[0];
    chart.data.labels = pts.map(k => timeLabel(k.t));
    ds.data = pts.map(k => k[key]);
    ds.borderColor = color;
    ds.backgroundColor = color + "26";
    ds.stepped = s.boolean ? true : false;
    ds.tension = s.boolean ? 0 : 0.3;

    const muted = cssVar("--muted"), line = cssVar("--line");
    const y = chart.options.scales.y;
    y.min = s.boolean ? 0 : undefined;
    y.max = s.boolean ? 1 : undefined;
    y.grid = { color: line };
    y.ticks = {
        color: muted,
        stepSize: s.boolean ? 1 : undefined,
        callback: v => s.boolean ? (v === 1 ? "Allumé" : v === 0 ? "Éteint" : "") : `${v} ${s.unit}`
    };
    chart.options.scales.x.ticks.color = muted;
    chart.options.plugins.tooltip = {
        callbacks: { label: c => s.boolean ? (c.parsed.y ? "Allumé" : "Éteint") : `${fmt(c.parsed.y, s.digits)} ${s.unit}` }
    };
    chart.update("none");
}

/* ---------- Récupération des données ---------- */

function parse(key, raw) {
    if (raw === null || raw === undefined) return undefined;
    if (SENSORS[key].boolean) return String(raw) === "true" ? 1 : 0;
    const n = parseFloat(raw);
    return Number.isNaN(n) ? undefined : n;
}

async function actualiser() {
    try {
        const response = await fetch("/api/sensors");
        if (!response.ok) throw new Error(`Erreur HTTP ${response.status}`);
        const mesures = await response.json();

        const tick = { t: Date.now() };
        KEYS.forEach(key => {
            const v = parse(key, mesures[key] && mesures[key].value);
            if (v !== undefined) tick[key] = v;
        });
        state.latest = mesures;
        state.ticks.push(tick);
        if (state.ticks.length > MAX_TICKS) state.ticks.shift();
        save("ap4a-ticks", state.ticks);

        setStatus(true);
        $("#lastUpdate").textContent = `Dernière mise à jour à ${timeLabel(tick.t)}`;
        updateCards();
        updateDetail();
    } catch (e) {
        console.log(e);
        setStatus(false);
    }
}

/* ---------- Statut, pause, fréquence ---------- */

function setStatus(online) {
    state.online = online;
    renderStatus();
}

function renderStatus() {
    const el = $("#status");
    const mode = state.paused ? "paused" : state.online ? "live" : "offline";
    el.dataset.state = mode;
    $("#statusText").textContent = { live: "En direct", paused: "En pause", offline: "Serveur injoignable" }[mode];
}

function startPolling() {
    clearInterval(state.timer);
    if (!state.paused) state.timer = setInterval(actualiser, state.intervalMs);
}

/* ---------- Boutons ---------- */

function setupControls() {
    $("#pauseBtn").addEventListener("click", () => {
        state.paused = !state.paused;
        $("#pauseBtn").textContent = state.paused ? "Reprendre" : "Pause";
        renderStatus();
        if (!state.paused) actualiser();
        startPolling();
    });

    $("#intervalSelect").addEventListener("change", e => {
        state.intervalMs = Number(e.target.value);
        startPolling();
        select(state.selected);                      // sauvegarde les préférences
    });

    $("#range").addEventListener("click", e => {
        const btn = e.target.closest("button");
        if (!btn) return;
        state.range = Number(btn.dataset.range);
        document.querySelectorAll("#range button").forEach(b => b.classList.toggle("active", b === btn));
        select(state.selected);
    });

    $("#themeBtn").addEventListener("click", () => {
        const root = document.documentElement;
        root.dataset.theme = root.dataset.theme === "dark" ? "light" : "dark";
        select(state.selected);
        updateCards();
    });

    $("#clearBtn").addEventListener("click", () => {
        state.ticks = [];
        state.shown = {};
        save("ap4a-ticks", []);
        updateCards();
        updateDetail();
    });

    $("#exportBtn").addEventListener("click", exportCsv);

    window.addEventListener("resize", updateCards);
}

function exportCsv() {
    const header = ["heure", ...KEYS.map(k => `${k} (${SENSORS[k].unit || "0/1"})`)];
    const rows = state.ticks.map(k =>
        [new Date(k.t).toISOString(), ...KEYS.map(key => k[key] ?? "")].join(";"));
    const blob = new Blob(["\uFEFF" + [header.join(";"), ...rows].join("\n")], { type: "text/csv;charset=utf-8" });
    const a = document.createElement("a");
    a.href = URL.createObjectURL(blob);
    a.download = "mesures-capteurs.csv";
    a.click();
    URL.revokeObjectURL(a.href);
}

/* ---------- Démarrage ---------- */

function init() {
    const prefs = load("ap4a-prefs", {});
    const preferredTheme = window.matchMedia("(prefers-color-scheme: light)").matches ? "light" : "dark";
    document.documentElement.dataset.theme = prefs.theme || preferredTheme;
    if (prefs.selected && SENSORS[prefs.selected]) state.selected = prefs.selected;
    if (prefs.range) state.range = Number(prefs.range);
    if (prefs.intervalMs) state.intervalMs = prefs.intervalMs;

    // on ne garde que l'historique récent (moins de 15 min) pour éviter un graphique avec un grand trou
    state.ticks = load("ap4a-ticks", []).filter(k => Date.now() - k.t < 15 * 60000);

    $("#intervalSelect").value = String(state.intervalMs);
    document.querySelectorAll("#range button").forEach(b =>
        b.classList.toggle("active", Number(b.dataset.range) === state.range));

    buildCards();
    buildChart();
    setupControls();
    select(state.selected);
    updateCards();

    actualiser();
    startPolling();
}

init();