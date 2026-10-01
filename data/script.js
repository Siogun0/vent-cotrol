// Настройки привязки элементов интерфейса к Tuya Data Points (DPS)
const tuyaMapping = {
    // Вентиляторы
    'fan_in':        { devId: 'dev_fan_01', dpsId: '1' },
    'fan_out':       { devId: 'dev_fan_01', dpsId: '2' },
    
    // Сценарии системы
    'vent-scenario': { devId: 'dev_system', dpsId: '10' },

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
const POLLING_INTERVAL = 1000; // Интервал автообновления в миллисекундах

// Флаг, блокирующий обновление слайдеров из сети, пока пользователь их двигает
let isUserInteracting = false;
let pollingTimer = null;
let isOffline = false;

/**
 * 1. ПОЛУЧЕНИЕ ДАННЫХ (GET) с защитой от перезаписи пользовательского ввода
 */
/**
 * Карта соответствия чисел enum status_color и CSS классов
 */
const STATUS_COLORS = {
    0: 'status-color-grey',
    1: 'status-color-green',
    2: 'status-color-yellow',
    3: 'status-color-red',
    4: 'status-color-blue',
    5: 'status-color-white',
    6: 'status-color-clear'
};

/**
 * Вспомогательная функция для безопасной смены цветового класса у элемента
 */
function updateNetworkIndicator(elementId, colorEnumIndex) {
    const element = document.getElementById(elementId);
    if (!element || colorEnumIndex === undefined) return;

    // Удаляем любые старые цветовые классы
    Object.values(STATUS_COLORS).forEach(className => element.classList.remove(className));

    // Добавляем новый класс, если индекс корректный
    const targetClass = STATUS_COLORS[colorEnumIndex];
    if (targetClass) {
        element.classList.add(targetClass);
    }
}

async function loadDashboardData() {
    const linkElement = document.getElementById('net-link');
    
    try {
        const response = await fetch(API_GET_STATUS);
        if (!response.ok) throw new Error('Ошибка сети при запросе Tuya API');
        
        const data = await response.json();
        
        // --- ОБНОВЛЕНИЕ СТАТУСА СТРАНИЦЫ: Успешное подключение ---
        if (linkElement) {
            linkElement.classList.remove('status-color-link-offline');
            linkElement.classList.add('status-color-link-online');
        }
        
        // 1.1. Обновление системных датчиков
        if (data.sensors) {
            if (data.sensors.temp) document.getElementById('val-temp').textContent = data.sensors.temp + '°C';
            if (data.sensors.humidity) document.getElementById('val-humidity').textContent = data.sensors.humidity + '%';
            if (data.sensors.voltage) document.getElementById('val-voltage').textContent = data.sensors.voltage + 'V';
            if (data.sensors.current) document.getElementById('val-current').textContent = data.sensors.current + 'A';
            if (data.sensors.power) document.getElementById('val-power').textContent = data.sensors.power + 'W';
        }
        
        // Чтение структуры объекта network (цвета внутренних шин BT, WiFi, CAN)
        if (data.network) {
            updateNetworkIndicator('net-bt', data.network.bt);
            updateNetworkIndicator('net-wifi', data.network.wifi);
            updateNetworkIndicator('net-can', data.network.can);
        }
        
        if (data.time) document.getElementById('val-time').textContent = data.time;

        // 1.2. Обновление слайдеров (только если пользователь сейчас ничего не трогает)
        if (!isUserInteracting && data.devices && Array.isArray(data.devices)) {
            for (const [htmlId, mapping] of Object.entries(tuyaMapping)) {
                const deviceStatus = data.devices.find(d => d.devId === mapping.devId);
                if (deviceStatus && deviceStatus.dps && deviceStatus.dps[mapping.dpsId] !== undefined) {
                    const element = document.getElementById(htmlId);
                    if (element) {
                        // Универсальная установка значения для input[type=range] и select
                        element.value = deviceStatus.dps[mapping.dpsId];
                    }
                }
            }
        }
    } catch (error) {
        console.error('Ошибка автоматической синхронизации с Tuya:', error);
        
        // --- ОБНОВЛЕНИЕ СТАТУСА СТРАНИЦЫ: Авария / Связь потеряна ---
        if (linkElement) {
            linkElement.classList.remove('status-color-link-online');
            linkElement.classList.add('status-color-link-offline');
        }
        
        // Гасим индикаторы внутренних протоколов, так как актуальных данных о них нет
        updateNetworkIndicator('net-bt', 0);
        updateNetworkIndicator('net-wifi', 0);
        updateNetworkIndicator('net-can', 0);
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
        
        // Если при отправке команды отвалилась сеть
        const linkElement = document.getElementById('net-link');
        if (linkElement) {
            linkElement.classList.remove('status-color-link-online');
            linkElement.classList.add('status-color-link-offline');
        }
    }
}

/**
 * 4. ИНИЦИАЛИЗАЦИЯ И СЛУШАТЕЛИ СОБЫТИЙ
 */
document.addEventListener('DOMContentLoaded', () => {
    
    // 4.1. Слушатели для всех слайдеров управления (вентиляторы и клапаны)
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

    // 4.2. СЛУШАТЕЛИ ДЛЯ ВЫПАДАЮЩЕГО СПИСКА СЦЕНАРИЕВ
    const scenarioSelect = document.getElementById('vent-scenario');
    if (scenarioSelect) {
        // Когда пользователь кликнул на список (открыл его) — блокируем обновление из сети
        scenarioSelect.addEventListener('focus', () => {
            isUserInteracting = true;
        });

        // Если пользователь просто кликнул мимо, не меняя значение
        scenarioSelect.addEventListener('blur', () => {
            isUserInteracting = false;
        });

        // Когда пользователь выбрал новый сценарий
        scenarioSelect.addEventListener('change', async (e) => {
            isUserInteracting = true;
            
            // 1. Отправляем выбранный сценарий через стандартный Tuya POST метод
            await sendTuyaCommand(e.target.id, e.target.value);
            
            // 2. Убираем фокус со списка, снимаем блок и принудительно обновляем статус
            scenarioSelect.blur(); 
            isUserInteracting = false;
            await loadDashboardData();
        });
    }

    // Первый ручной запуск при старте страницы
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
