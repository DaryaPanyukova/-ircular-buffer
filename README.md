# Циклический буфер

## CCircularBuffer
CCircularBufferExt - это STL - совместимый контейнер "Циклический буфер", с поддержкой возможности расширения и фиксированным размером.

[Циклический буфер](https://en.wikipedia.org/wiki/Circular_buffer) - структура данных, которая часто применяется для буферизации в задачах по передачи и обработки потока данных.

## Требования

Контейнер удовлетворяет [требованиям](https://en.cppreference.com/w/cpp/named_req/Container) для stl-контейнера.
А также [требованиям для последовательного контейнера](https://en.cppreference.com/w/cpp/named_req/SequenceContainer)

## Итератор

Класс предоставляет итератор произвольного доступа, соответсвующий следующим [требованиям](https://en.cppreference.com/w/cpp/named_req/RandomAccessIterator).

## CCircularBufferExе

CCircularBufferExt - циклический буфер, обладающий возможностью расширения: 
В случае достижения размера кольцевого буфера максимального возможного своего размера, значение максимального размера удваивается.








