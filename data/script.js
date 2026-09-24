// Настройки привязки элементов интерфейса к Tuya Data Points (DPS)
const tuyaMapping = {
    // Вентиляторы
    'fan_in':        { devId: 'dev_fan_01', dpsId: '1' },
    'fan_out':       { devId: 'dev_fan_01', dpsId: '2' },
    // Приточные клапана
    'valve_living':  { devId: 'dev_valves_zone1', dpsId: '101' },
    'valve_bedroom': { devId: 'dev_valves_zone1', dpsId: '102' },
    'valve_kids':    { devId: 'dev_valves_zone1', dpsId: '103' },
    'valve_store':   { devId: 'dev_valves_zone1', dpsId: '104' },
    // Вытяжные клапана
    'valve_toilet':  { devId: 'dev_valves_zone2', dpsId: '201' },
    'valve_bath':    { devId: 'dev_valves_zone2', dpsId: '202' }
};

// Конфигурация API и таймингов
const API_GET_STATUS = '/api/tuya/status';
const API_POST_CONTROL = '/api/tuya/control';
const POLLING_INTERVAL = 4000; // Интервал автообновления в миллисекундах (4 секунды)

// Флаг, блокирующий обновление слайдеров из сети, пока пользователь их двигает
let isUserInteracting = false;
let pollingTimer = null;

/**
 * 1. ПОЛУЧЕНИЕ ДАННЫХ (GET) с защитой от перезаписи пользовательского ввода
 */
async function loadDashboardData() {
    try {
        const response = await fetch(API_GET_STATUS);
        if (!response.ok) throw new Error('Ошибка сети при запросе Tuya API');
        
        const data = await response.json();
        
        // 1.1. Обновление системных датчиков
        if (data.sensors) {
            if (data.sensors.temp) document.getElementById('val-temp').textContent = data.sensors.temp + '°C';
            if (data.sensors.humidity) document.getElementById('val-humidity').textContent = data.sensors.humidity + '%';
            if (data.sensors.voltage) document.getElementById('val-voltage').textContent = data.sensors.voltage + 'V';
            if (data.sensors.current) document.getElementById('val-current').textContent = data.sensors.current + 'A';
            if (data.sensors.power) document.getElementById('val-power').textContent = data.sensors.power + 'W';
        }
        if (data.time) document.getElementById('val-time').textContent = data.time;

        // 1.2. Обновление слайдеров (только если пользователь сейчас ничего не трогает)
        if (!isUserInteracting && data.devices && Array.isArray(data.devices)) {
            for (const [htmlId, mapping] of Object.entries(tuyaMapping)) {
                const deviceStatus = data.devices.find(d => d.devId === mapping.devId);
                if (deviceStatus && deviceStatus.dps && deviceStatus.dps[mapping.dpsId] !== undefined) {
                    const slider = document.getElementById(htmlId);
                    if (slider) {
                        slider.value = deviceStatus.dps[mapping.dpsId];
                    }
                }
            }
        }
    } catch (error) {
        console.error('Ошибка автоматической синхронизации с Tuya:', error);
    }
}

/**
 * 2. ЗАПУСК И УПРАВЛЕНИЕ АВТОМАТИЧЕСКИМ ОПРОСОМ
 */
function startPolling() {
    stopPolling(); // На всякий случай очищаем старый таймер
    pollingTimer = setInterval(async () => {
        if (!isUserInteracting) {
            await loadDashboardData();
        }
    }, POLLING_INTERVAL);
}

function stopPolling() {
    if (pollingTimer) {
        clearInterval(pollingTimer);
        pollingTimer = null;
    }
}

/**
 * 3. ОТПРАВКА ИЗМЕНЕНИЙ (POST)
 */
async function sendTuyaCommand(htmlId, value) {
    const mapping = tuyaMapping[htmlId];
    if (!mapping) return;

    const bodyPayload = {
        devId: mapping.devId,
        dps: {
            [mapping.dpsId]: parseInt(value, 10)
        },
        t: Math.floor(Date.now() / 1000)
    };

    try {
        const response = await fetch(API_POST_CONTROL, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(bodyPayload)
        });

        if (!response.ok) throw new Error('Tuya шлюз отклонил команду');
        console.log(`Tuya OK: ${mapping.devId} [${mapping.dpsId}] -> ${value}`);
    } catch (error) {
        console.error(`Ошибка отправки Tuya DPS для ${htmlId}:`, error);
    }
}

/**
 * 4. ИНИЦИАЛИЗАЦИЯ И СЛУШАТЕЛИ СОБЫТИЙ
 */
document.addEventListener('DOMContentLoaded', () => {
    
    document.querySelectorAll('input[type="range"]').forEach(slider => {
        // Как только пользователь нажал на слайдер (начал двигать)
        slider.addEventListener('input', () => {
            isUserInteracting = true;
        });

        // Как только пользователь отпустил слайдер
        slider.addEventListener('change', async (e) => {
            // 1. Срочно отправляем новое состояние на сервер
            await sendTuyaCommand(e.target.id, e.target.value);
            
            // 2. Снимаем блок и сразу обновляем данные, чтобы подтвердить статус
            isUserInteracting = false;
            await loadDashboardData();
        });
    });

    // Первый ручной запуск при старте
    loadDashboardData();
    
    // Включение циклического автоопроса
    startPolling();
});

// Если вкладка браузера уходит в фон, останавливаем опрос для экономии батареи/трафика
document.addEventListener('visibilitychange', () => {
    if (document.hidden) {
        stopPolling();
    } else {
        loadDashboardData();
        startPolling();
    }
});
