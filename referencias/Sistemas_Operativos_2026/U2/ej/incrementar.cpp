/*
 *   g++ -Wall -O2 -g -o incrementar incrementar.cpp
 *   objdump -S incrementar
 *
 */


#include <iostream>
#include <thread>
#include <atomic>

// Creamos una bandera atómica inicializada en 'false' (libre)
std::atomic_flag lock = ATOMIC_FLAG_INIT;
volatile int shared_counter = 0;

void increment_resource(int id_hilo) {
    for (int i = 0; i < 100000; ++i) {
        
        // 1. INSTRUCCIÓN ATÓMICA + BARRERA (Adquirir el lock)
        // test_and_set() cambia el valor a 'true' y devuelve el valor anterior.
        // Si devuelve 'true', significa que otro hilo ya tenía el lock, así que espera.
        // std::memory_order_acquire actúa como barrera: impide que el código de abajo se ejecute antes.
        while (lock.test_and_set(std::memory_order_acquire)) {
            // Espera activa (proceso similar a Peterson pero seguro por hardware)
        }

        // --- INICIO DE LA ZONA CRÍTICA ---
        shared_counter++;
        // --- FIN DE LA ZONA CRÍTICA -----

        // 2. BARRERA + INSTRUCCIÓN ATÓMICA (Liberar el lock)
        // Cambia el valor de vuelta a 'false'.
        // std::memory_order_release actúa como barrera: garantiza que el incremento 
        // del contador se guarde en memoria antes de liberar el lock.
        lock.clear(std::memory_order_release);
    }
}

int main() {
    // Creamos dos hilos que intentarán modificar la misma variable al mismo tiempo
    std::thread hilo1(increment_resource, 1);
    std::thread hilo2(increment_resource, 2);

    hilo1.join();
    hilo2.join();

    // El resultado siempre será exactamente 200000 gracias a la sincronización
    std::cout << "Resultado del contador: " << shared_counter << std::endl;
    return 0;
}

