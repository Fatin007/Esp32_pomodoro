#ifndef POMODORO_DASHBOARD_PAGE_H
#define POMODORO_DASHBOARD_PAGE_H

#include <Arduino.h>

const char MAIN_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>

<html>

<head>

<meta name="viewport"
      content="width=device-width, initial-scale=1">

<title>Pomodoro Timer Fa3</title>

<style>

* {
    box-sizing: border-box;
}

body {

    margin: 0;

    font-family:
        -apple-system,
        BlinkMacSystemFont,
        "Segoe UI",
        Arial,
        sans-serif;

    background:
        linear-gradient(
            135deg,
            #0f172a,
            #111827,
            #1e293b
        );

    color: white;

    min-height: 100vh;

    display: flex;

    justify-content: center;

    align-items: center;

    padding: 20px;

}

.container {

    width: 100%;

    max-width: 480px;

    background:
        rgba(31, 41, 55, 0.92);

    backdrop-filter: blur(20px);

    border: 1px solid
        rgba(255,255,255,0.08);

    border-radius: 28px;

    padding: 28px;

    box-shadow:
        0 25px 70px
        rgba(0,0,0,0.45);

}

.header {

    display: flex;

    align-items: center;

    justify-content: space-between;

    margin-bottom: 25px;

}

.logo h1 {

    font-size: 22px;

    margin: 0;

}

.connection {

    width: 10px;

    height: 10px;

    border-radius: 50%;

    background: #22c55e;

    box-shadow:
        0 0 12px
        rgba(34,197,94,0.8);

}

.tabs {

    display: grid;

    grid-template-columns:
        repeat(3, 1fr);

    background: #111827;

    border-radius: 15px;

    padding: 5px;

    margin-bottom: 28px;

}

.tab {

    border: none;

    background: transparent;

    color: #9ca3af;

    padding: 11px 5px;

    border-radius: 11px;

    cursor: pointer;

    font-size: 13px;

    font-weight: 600;

}

.tab.active {

    background: #374151;

    color: white;

}

.mode {

    text-align: center;

    font-size: 16px;

    color: #9ca3af;

    margin-bottom: 5px;

    text-transform: uppercase;

    letter-spacing: 2px;

    font-weight: 600;

}

.timer {

    text-align: center;

    font-size:
        clamp(58px, 17vw, 82px);

    line-height: 1;

    font-weight: 700;

    letter-spacing: 2px;

    margin: 22px 0;

    font-variant-numeric:
        tabular-nums;

}

.status {

    text-align: center;

    color: #9ca3af;

    font-size: 14px;

    margin-bottom: 22px;

}

.progress {

    height: 7px;

    background: #374151;

    border-radius: 10px;

    overflow: hidden;

    margin-bottom: 25px;

}

.progress-bar {

    height: 100%;

    width: 0%;

    background: #22c55e;

    border-radius: 10px;

    transition:
        width 0.5s ease;

}

.controls {

    display: grid;

    grid-template-columns:
        1fr 1fr;

    gap: 12px;

}

button.control {

    border: none;

    border-radius: 14px;

    padding: 15px;

    font-size: 16px;

    font-weight: 700;

    cursor: pointer;

    color: white;

}

.start {

    background: #22c55e;

}

.reset {

    background: #374151;

}

.skip {

    background: #2563eb;

    grid-column: span 2;

}

.custom-panel {

    display: none;

    margin-bottom: 22px;

}

.custom-panel.show {

    display: block;

}

.input-row {

    display: grid;

    grid-template-columns:
        1fr 1fr;

    gap: 12px;

}

.input-group label {

    display: block;

    color: #9ca3af;

    font-size: 13px;

    margin-bottom: 7px;

}

.input-group input {

    width: 100%;

    padding: 13px;

    border: 1px solid #374151;

    background: #111827;

    color: white;

    border-radius: 12px;

    font-size: 18px;

    text-align: center;

    outline: none;

}

.input-group input:focus {

    border-color: #60a5fa;

}

.set-button {

    width: 100%;

    margin-top: 12px;

    padding: 13px;

    border: none;

    border-radius: 12px;

    background: #3b82f6;

    color: white;

    font-weight: 700;

    cursor: pointer;

}

.stats {

    display: grid;

    grid-template-columns:
        1fr 1fr;

    gap: 12px;

    margin-top: 25px;

}

.stat {

    background: #111827;

    border-radius: 15px;

    padding: 15px;

    text-align: center;

}

.stat-value {

    font-size: 23px;

    font-weight: 700;

}

.stat-label {

    font-size: 12px;

    color: #9ca3af;

    margin-top: 3px;

}

.footer {

    text-align: center;

    margin-top: 20px;

    color: #6b7280;

    font-size: 11px;

}

</style>

</head>

<body>

<div class="container">

    <div class="header">

        <div class="logo">

            <h1>Pomodoro Timer Fa3</h1>

        </div>

        <div
            class="connection"
            id="connection">
        </div>

    </div>


    <div class="tabs">

        <button
            class="tab active"
            id="pomodoroTab"
            onclick="setMode('pomodoro')">

            Pomodoro

        </button>

        <button
            class="tab"
            id="customTab"
            onclick="setMode('custom')">

            Timer

        </button>

        <button
            class="tab"
            id="stopwatchTab"
            onclick="setMode('stopwatch')">

            Stopwatch

        </button>

    </div>


    <div
        class="custom-panel"
        id="customPanel">

        <div class="input-row">

            <div class="input-group">

                <label>Minutes</label>

                <input
                    type="number"
                    id="customMinutes"
                    min="0"
                    max="999"
                    value="1">

            </div>

            <div class="input-group">

                <label>Seconds</label>

                <input
                    type="number"
                    id="customSeconds"
                    min="0"
                    max="59"
                    value="0">

            </div>

        </div>

        <button
            class="set-button"
            onclick="setCustomTime()">

            Set Custom Time

        </button>

    </div>


    <div
        class="mode"
        id="mode">

        FOCUS

    </div>


    <div
        class="timer"
        id="timer">

        25:00

    </div>


    <div
        class="status"
        id="status">

        Paused

    </div>


    <div class="progress">

        <div
            class="progress-bar"
            id="progressBar">
        </div>

    </div>


    <div class="controls">

        <button
            class="control start"
            id="startButton"
            onclick="toggleTimer()">

            Start

        </button>

        <button
            class="control reset"
            onclick="resetTimer()">

            Reset

        </button>

        <button
            class="control skip"
            id="skipButton"
            onclick="skipSession()">

            Skip Session

        </button>

    </div>


    <div class="stats">

        <div class="stat">

            <div
                class="stat-value"
                id="completed">

                0

            </div>

            <div class="stat-label">

                Pomodoros

            </div>

        </div>

        <div class="stat">

            <div
                class="stat-value"
                id="device">

                ESP32

            </div>

            <div class="stat-label">

                Device

            </div>

        </div>

    </div>


    <div class="footer">

        Local ESP32 Timer - Fa3

    </div>

</div>


<script>

let currentMode = 'pomodoro';


// ======================================================
// Get Status
// ======================================================

async function getStatus() {

    try {

        const response =
            await fetch('/status');

        const data =
            await response.json();


        document.getElementById('mode')
            .innerText = data.mode;


        document.getElementById('timer')
            .innerText = data.time;


        document.getElementById('completed')
            .innerText = data.completed;


        document.getElementById('status')
            .innerText =
                data.running
                ? 'Running'
                : 'Paused';


        document.getElementById('startButton')
            .innerText =
                data.running
                ? 'Pause'
                : 'Start';


        let percentage = 0;

        if (data.duration > 0) {

            percentage =
                (
                    (data.duration - data.remaining)
                    / data.duration
                ) * 100;

        }

        document.getElementById('progressBar')
            .style.width =
                percentage + '%';


        document.getElementById('connection')
            .style.background =
                '#22c55e';

    }

    catch (error) {

        document.getElementById('connection')
            .style.background =
                '#ef4444';

    }

}


// ======================================================
// Change Mode
// ======================================================

async function setMode(mode) {

    currentMode = mode;

    await fetch(
        '/mode?value=' + mode
    );


    document
        .querySelectorAll('.tab')
        .forEach(
            tab =>
                tab.classList.remove('active')
        );


    if (mode === 'pomodoro') {

        document
            .getElementById('pomodoroTab')
            .classList.add('active');

        document
            .getElementById('customPanel')
            .classList.remove('show');

        document
            .getElementById('skipButton')
            .style.display = 'block';

    }

    else if (mode === 'custom') {

        document
            .getElementById('customTab')
            .classList.add('active');

        document
            .getElementById('customPanel')
            .classList.add('show');

        document
            .getElementById('skipButton')
            .style.display = 'none';

    }

    else {

        document
            .getElementById('stopwatchTab')
            .classList.add('active');

        document
            .getElementById('customPanel')
            .classList.remove('show');

        document
            .getElementById('skipButton')
            .style.display = 'none';
    }


    getStatus();
}


// ======================================================
// Start / Pause
// ======================================================

async function toggleTimer() {

    await fetch('/toggle');

    getStatus();
}


// ======================================================
// Reset
// ======================================================

async function resetTimer() {

    await fetch('/reset');

    getStatus();
}


// ======================================================
// Skip
// ======================================================

async function skipSession() {

    await fetch('/skip');

    getStatus();
}


// ======================================================
// Set Custom Time
// ======================================================

async function setCustomTime() {

    let minutes =
        parseInt(
            document.getElementById(
                'customMinutes'
            ).value
        ) || 0;


    let seconds =
        parseInt(
            document.getElementById(
                'customSeconds'
            ).value
        ) || 0;


    if (minutes < 0) {
        minutes = 0;
    }

    if (seconds < 0) {
        seconds = 0;
    }

    if (seconds > 59) {
        seconds = 59;
    }


    if (
        minutes === 0 &&
        seconds === 0
    ) {

        alert(
            'Please enter a time greater than 0.'
        );

        return;
    }


    await fetch(
        '/custom?minutes='
        + minutes
        + '&seconds='
        + seconds
    );


    getStatus();
}


// ======================================================
// Auto Refresh
// ======================================================

setInterval(
    getStatus,
    1000
);

getStatus();

</script>

</body>

</html>

)rawliteral";

#endif
