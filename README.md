
# My_Project

Embedded C-projekt til **ATmega328PB** (16 MHz), bygget med XC8 i MPLAB for VS Code og debugget via mEDBG.

## Beskrivelse af softwaren

Beskrivelsen af, hvordan softwaren virker, ligger som PDF: [res/Opgave2_Beskrivelse.pdf](res/Opgave2_Beskrivelse.pdf)

## Structure

| Path                          | Purpose                                                                                                                             |
|-------------------------------|-------------------------------------------------------------------------------------------------------------------------------------|
| src                           | C source files (.c): main.c and module implementations (timer.c, uart.c – both UART0 and UART1)                                     |
| include                       | Header files (.h): module interfaces (timer.h, uart.h) and project-wide defines (ProjectDefines.h). Added as include directory.     |
| _build                        | The [CMake build tree](https://cmake.org/cmake/help/latest/manual/cmake.1.html#introduction-to-cmake-buildsystems), can be deleted. |
| cmake                         | Generated [CMake](https://cmake.org/) files. May be deleted if user.cmake has not been added                                        |
| .vscode                       | See [VSCode](https://code.visualstudio.com/docs/getstarted/settings)                                                                |
| .vscode\settings.json         | Workspace specific settings                                                                                                         |
| .vscode\My_Project.mplab.json | The MPLAB project file, should not be deleted                                                                                       |
| out                           | Final build artifacts                                                                                                               |
| res                           | Ressourcer, bl.a. beskrivelsen af softwaren som PDF                                                                                 |
