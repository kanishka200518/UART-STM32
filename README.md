# UART-STM32
# STM32 Interrupt-Driven UART Driver with Circular Ring Buffer

This repository contains a lightweight, bare-metal **Full-Duplex UART Communication Driver** developed for the **STM32** microcontroller ecosystem. By utilizing an optimized, asynchronous **interrupt-driven architecture** coupled with custom C **circular ring buffers**, this driver achieves robust, non-blocking serial communication capable of handling high-speed bursts of up to 60 characters with zero data corruption or transmission errors.

---



## 🛠️ System Architecture

The software architecture completely isolates hardware events from application tasks. Instead of the main CPU waiting for bytes to step through the physical wire, data is handed off to an independent buffer system.

```
      INCOMING DATA                    CIRCULAR RING BUFFER (512 Bytes)                    OUTGOING DATA
   [ Serial Keyboard / ]               +--------------------------------+               [ Virtual Console / ]
   [ Hardware Peripheral ]             |                                |               [  Terminal Screen  ]
             |                         |    +---+---+---+---+---+---+   |                         ^
             v                         |    | H |   |   |   |   | T |   |                         |
       Data Intake   -------------->   |    +---+---+---+---+---+---+   |  --------------->  Data Output
 (Reads 1 byte per cycle)              |      |                   |     |              (Non-blocking burst)
                                       |   Head Index          Tail Index|
                                       +--------------------------------+
                                                       |
                                            [ Backpressure Guard ]
                                        (Throttles if Head meets Tail)

```

---

## 🔍 Deep Dive: How It Works

The architecture is divided into three functional layers to enforce strict decoupling between hardware interface data, storage arrays, and execution scheduling.

### 1. Data Structure Management

The core storage is managed as a FIFO (First-In, First-Out) circular ring buffer utilizing explicit tracking indexes.

* **Volatile Index Qualification:** Both the `head` (write pointer) and `tail` (read pointer) indexes are qualified to prevent the compiler from caching their values in CPU registers. This forces the microcontroller to fetch actual index values from RAM during unexpected, asynchronous interrupt changes.
* **Modulo Arithmetic Wrapping:** Pointer wrap-around handles continuity seamlessly. When an index reaches the final slot of the 512-byte array, it is wrapped back to index zero using a modulo calculation based on total buffer capacity. This forms an endless logical loop with no costly memory-shifting operations.

### 2. The Transmission Pipeline

The driver decouples the application thread from physical transmission, mitigating bit-level desynchronization.

* **Backpressure Management:** If rapid text input approaches saturation limits, the driver triggers an active hardware safety loop to empty the buffer. This prevents a pointer overlap condition where unread data would otherwise be overwritten.
* **Asynchronous Buffer Processing:** Instead of hogging CPU clock cycles inside a blocking multi-byte transfer loop, the processing routine evaluates exactly one byte per clock cycle. This allows the background serial engine and the core software loop to remain fully synchronized.

### 3. Non-Blocking Intake Execution

The primary microcontroller application processing loop isolates character capture routines completely from the rendering mechanics.

* **Step 1:** Instantly ingest single-byte incoming inputs to keep the UART hardware line completely clear.
* **Step 2:** Safely drop the raw character value into the ring buffer queue.
* **Step 3:** Asynchronously bleed queued bytes out to the display terminal, spacing them out with micro-delays to allow the simulation engine to breathe.

---

## 🚀 Key Technical Highlights

* **Zero-Error Data Handling:** Handles rapid typing and intensive multi-line pastes of up to 60 characters with zero scrambled symbols (`ßÇß` or `?`) or bit-level desynchronization.
* **Memory Isolation:** Passes raw values directly into tracking registers, avoiding pointer alignment issues or memory address corruption.
* **Hardware Coexistence:** Uses clean inline assembly pacing strategies, allowing compatibility across strict simulation setups (like Wokwi) and physical STM32 devices.
