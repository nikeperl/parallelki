# Sin Array Sum

## Сборка и запуск

### Использование CMake
```sh
mkdir build && cd build
cmake -DUSE_FLOAT=ON ..  # Для float
cmake -DUSE_FLOAT=OFF .. # Для double
make
./sin_array_sum