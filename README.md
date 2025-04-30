# DECRYPT INSTRUCTIONS  
Date: 17/01/2025  

Authors: Loann KAIKA & Clément SARAZIN  

<br />

<img src="PICTURES/spacetech_orleans_round.png" alt="SpaceTech logo" style="height: 200px; width:200px;" />
<img src="PICTURES/decrypt_round.png" alt="DECRYPT logo" style="height: 200px; width:200px;" align="right"/>
<br /><br />

## What is DECRYPT?

**DECRYPT** stands for *moDular and Embedded platform for Control and telemetRY of aerosPace sysTems*. It is a modular system of electronic boards designed to manage various critical tasks onboard mini and experimental rockets (MINIF and FUSEX), such as flight control, telemetry, power regulation, and human-machine interaction.

The DECRYPT platform was initiated within the **Space’Tech Orléans** student association and is now developed by 4A GPSE students from Polytech Orléans. It complies with **Planète Sciences requirements** for C’Space rocketry events and aims to be both educational and functional for advanced aerospace experimentation.

Each board has a specific function but can also interact with others to ensure flexibility, modularity and safety during flight.

---

## DECRYPT System Overview

The DECRYPT system is composed of the following **three main modules**:

- **Sequencer Board (SEQ)**  
  Responsible for monitoring flight phases (e.g., launch, apogee) and triggering key mechanisms such as parachute deployment. It interfaces with the LED board and supports UART communication with the telemetry module. It also provides control for servo and stepper motors (e.g., for airbrakes or separation systems in FUSEX).

- **Power Board (POW)**  
  Supplies regulated voltages (3.3V, 5V and adjustable up to 24V) to all other modules. It uses multiple DC-DC converters to ensure stable power distribution even under high current or variable loads, especially during FUSEX missions with demanding motor configurations.

- **Telemetry Board (TELEM)**  
  Gathers and stores sensor data (altitude, pressure, orientation, etc.) during the flight. It transmits information in real time via LoRa (RFM98W module) to the ground station. Equipped with a Raspberry Pi Pico, MPU9250, BME680, and GPS NEO-6M, it ensures high-frequency acquisition and communication in compliance with radio frequency regulations.

---

## Naming conventions

To ensure consistency across the repository and future documentation, the following naming rules are applied:

### 📌 PCB Files
```plaintext
DECRYPT_[MOD]_vX
