# Bare-Metal Event-Driven Firmware (Cortex-M)

## Overview

This project is a bare-metal, event-driven firmware system built from scratch on a Cortex-M microcontroller (Stellaris LM3S6965).


It implements interrupt-driven peripherals (Timer, GPIO, UART RX/TX) with deferred processing, lock-free ring buffers, and a cooperative event loop — without an RTOS, HAL, or blocking code.

---

## Core Ideas
-	Interrupts signal, main decides
-	No blocking, no polling, no delays
-	ISRs are minimal and bounded
-	Data buffering is separate from event signaling
-	Deterministic, debuggable firmware behavior

This project intentionally stops before RTOSes, DMA, command shells, or complex protocols.

---
## Architecture
### High Level Flow

```text
[Hardware Interrupts]
        ↓
[ISR: minimal work]
        ↓
[event_post_from_isr()]
        ↓
[Event Queue]
        ↓
[Main Event Loop]
        ↓
[Drivers / Application Logic]
```
### Execution Model
-	### ISR context
	-	Reads hardware registers
	-	Writes to buffers
	-	Posts events
	-	Never blocks
-	### Main context
	-	Drains buffers
	-	Implements policy and logic
	-	Owns all decision-making
---
## Implemented Components

1. ### Event System
	-	Lock-free single-producer / single-consumer ring buffer
	-	Used for ISR → main signaling
	-	Events represent state changes, not data floods

2. ### Timer Driver
	-	Hardware GPTM timer
	-	Periodic interrupts
	-	Produces system tick events
	-	No delays or busy loops

3. ### GPIO Driver
	-	Raw register-level configuration
	-	Output controlled from main loop
	-	Used to visualize time via heartbeat toggling

4. ### UART RX (Interrupt-Driven)
	-	RX interrupt drains hardware FIFO
	-	Bytes stored in a dedicated RX ring buffer
	-	ISR posts a single “RX available” event per burst
	-	Main drains buffer without data loss (within buffer limits)

5. ### UART TX (Interrupt-Driven)
	-	Buffered, non-blocking transmission
	-	TX FIFO priming handled correctly
	-	TX interrupt enabled only when needed
	-	RX → TX echo verified under load
---
## Key Design Decisions

### Why Event-Driven?

Polling and delays hide bugs and destroy scalability.
Events make concurrency explicit and observable.

### Why Separate Buffers from Events?

Events signal availability.
Buffers store data.
Conflating the two causes queue flooding and data loss.

### Why No RTOS?

This project intentionally builds the mental model beneath an RTOS:
-	Interrupt latency
-	Deferred processing
-	Backpressure
-	Cooperative scheduling

RTOS concepts map cleanly on top of this foundation.

---

## Debugging & Validation

All behavior was validated using GDB, including:
-	Stepping through ISRs and main context
-	Inspecting peripheral registers directly
-	Observing queue head/tail movement
-	Verifying interrupt timing behavior
-	Diagnosing backpressure and dropped data
-	Confirming UART RX/TX correctness under burst input

No behavior is “assumed”. Everything was observed.

---

## Final Note

This project exists to build systems intuition, not just “working code”.

Every component was written to be:
-	explainable
-	observable
-	debuggable
-	and correct under concurrency

That clarity is a real deliverable.