# Network Protocol Analyzer

[![CI](https://github.com/ayaanvashistha17/network-protocol-analyzer/actions/workflows/ci.yml/badge.svg)](https://github.com/ayaanvashistha17/network-protocol-analyzer/actions/workflows/ci.yml)

A C/C++ project demonstrating embedded systems concepts: custom protocol simulation, packet dissection, unit testing, and CI/CD automation. Designed to showcase skills relevant to firmware development roles.

## Features

- **Device Simulator (C)** – Emulates an embedded device (like a fire panel) sending packets over TCP with a custom binary protocol.
- **Packet Dissector (C++17)** – Parses, validates CRC, and displays packet contents in human-readable form.
- **Unit Test Framework (C++)** – Lightweight custom test runner with assertions.
- **CI/CD Pipeline** – GitHub Actions automatically builds and tests the project on every push.
- **Automation Scripts** – Bash and Python scripts for building, testing, and end-to-end validation.

## Protocol Specification

| Field     | Size (bytes) | Description                    |
|-----------|--------------|--------------------------------|
| Magic     | 2            | `0xAA55`                       |
| Version   | 1            | Protocol version (1)           |
| Type      | 1            | `0x01` HEARTBEAT, `0x02` ALARM, `0x03` STATUS, `0x04` CONFIG |
| Length    | 2            | Payload length (≤ 64)          |
| Timestamp | 4            | Seconds since boot (simulated) |
| Data      | Length       | Payload (ASCII or binary)      |
| CRC       | 2            | CRC-16-CCITT over header + payload |

## Technologies Used

- **C** (simulator, protocol library)
- **C++17** (dissector, test framework)
- **CMake** (build system)
- **GitHub Actions** (CI/CD)
- **Python** (integration test automation)
- **Bash** (build/test scripts)

## Building

```bash
chmod +x automation/*.sh
./automation/build.sh
