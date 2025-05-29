// Variables globales
let sensorUpdateInterval;

// Inicialización cuando el DOM está listo
document.addEventListener('DOMContentLoaded', function() {
    // Configurar botones de LEDs
    setupLedButtons();
    
    // Configurar sliders PWM
    setupPwmSliders();
    
    // Iniciar actualización de sensores
    updateSensorData();
    sensorUpdateInterval = setInterval(updateSensorData, 1000);
});

// Configurar eventos para los botones de LEDs
function setupLedButtons() {
    const ledButtons = document.querySelectorAll('.led-button');
    
    ledButtons.forEach(button => {
        button.addEventListener('click', function() {
            const ledId = this.getAttribute('data-id');
            const ledCircle = this.querySelector('.led-circle');
            const isOn = ledCircle.classList.contains('on');
            const newState = isOn ? 'off' : 'on';
            
            // Actualizar visualmente
            ledCircle.classList.remove('on', 'off');
            ledCircle.classList.add(newState);
            
            // Enviar comando al servidor
            toggleLed(ledId, newState);
        });
    });
}

// Enviar comando para cambiar estado de LED
function toggleLed(ledId, state) {
    const xhr = new XMLHttpRequest();
    xhr.open('GET', `/led?id=${ledId}&state=${state}`, true);
    xhr.send();
}

// Configurar eventos para los sliders PWM
function setupPwmSliders() {
    const sliders = [
        { slider: document.getElementById('led4-slider'), value: document.getElementById('led4-value') },
        { slider: document.getElementById('led5-slider'), value: document.getElementById('led5-value') }
    ];
    
    sliders.forEach((item, index) => {
        item.slider.addEventListener('input', function() {
            const value = this.value;
            const ledId = index + 4; // LEDs 4 y 5
            
            // Actualizar visualización
            item.value.textContent = value;
            
            // Enviar valor al servidor
            updatePwm(ledId, value);
        });
    });
}

// Enviar valor PWM al servidor
function updatePwm(ledId, value) {
    const xhr = new XMLHttpRequest();
    xhr.open('GET', `/slider?id=${ledId}&value=${value}`, true);
    xhr.send();
}

// Obtener y actualizar datos de sensores
function updateSensorData() {
    fetch('/sensors')
        .then(response => {
            if (!response.ok) throw new Error("HTTP error");
            return response.text();
        })
        .then(text => {
            try {
                const data = JSON.parse(text);
                if(data.pot === undefined || data.ldr === undefined) {
                    throw new Error("Datos incompletos");
                }
                document.getElementById('pot-value').textContent = data.pot;
                document.getElementById('ldr-value').textContent = data.ldr;
            } catch (e) {
                console.error("Error parsing JSON:", e, "Response:", text);
            }
        })
        .catch(error => console.error("Fetch error:", error));
}