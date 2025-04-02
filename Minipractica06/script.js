// Array de colores de fondo posibles
const coloresFondo = [
    "#f0f8ff", "#f8f0ff", "#fff0f8", "#f0fff8", "#fff8f0", "#e6f7ff",
    "#ffe6f7", "#f7ffe6", "#e6fffb", "#fbe6ff", "#ffebf0", "#ebfff2",
    "#f0f4ff", "#fff4f0", "#e6ffe6", "#fff9e6", "#e6f2ff", "#f9e6ff",
    "#e6ffe8", "#ffe6e6", "#e6e6ff", "#ffebcd", "#cdffeb", "#ffd9eb",
    "#d9ffd9", "#d9d9ff", "#fff0e6", "#e6fff0", "#f0e6ff", "#e6f8ff"
];

// Obtener elementos del DOM
const boton = document.getElementById("cambiarColorBtn");
const body = document.body;

// Variable para almacenar el último color usado
let ultimoColor = null;

// Función para obtener un color aleatorio diferente al actual
function obtenerColorAleatorio() {
    let colorAleatorio;
    do {
        const indiceAleatorio = Math.floor(Math.random() * coloresFondo.length);
        colorAleatorio = coloresFondo[indiceAleatorio];
    } while (colorAleatorio === ultimoColor && coloresFondo.length > 1);
    
    return colorAleatorio;
}

// Función para cambiar el color de fondo
function cambiarColorFondo() {
    const nuevoColor = obtenerColorAleatorio();
    ultimoColor = nuevoColor;
    
    // Aplicar el nuevo color
    body.style.backgroundColor = nuevoColor;
    
}

// Añadir evento click al botón
boton.addEventListener("click", cambiarColorFondo);