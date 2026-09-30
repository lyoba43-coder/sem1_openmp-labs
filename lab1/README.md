# Лабораторная работа №1 (OpenMP) — Задачи 1-3

## Сборка
1. Открыть solution в Visual Studio.
2. Для каждого проекта: Project → Properties → C/C++ → Language →
   OpenMP Support = Yes.
3. Ctrl+Shift+B — сборка.

## Запуск

### Задача 1 — идентификаторы потоков + Hello World
task1.exe <num_threads>
task1.exe 8

Каждый поток печатает свой id, общее число потоков и "Hello World".

### Задача 2 — усреднение соседей
task2.exe <N> <num_threads>
task2.exe 16000 8

Строит массив b[i] = (a[i-1] + a[i] + a[i+1]) / 3.0.
Демонстрирует 5 типов schedule: static, dynamic, guided,
static 64, dynamic 64.

### Задача 3 — обратный порядок id, 5 способов
task3.exe <num_threads> <method>
task3.exe 8 1

Печатает id потоков в обратном порядке. 5 способов:
1 - ordered
2 - critical + counter
3 - barrier + step
4 - master + flags
5 - single + loop
