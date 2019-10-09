## Dependências

Para compilar o projeto é necessário:

- Compilador de C++ 17
- CMake >=3.0

O repositório contém o Rapidcheck como submódulo.

## Instruções

Para clonar e compilar o repositório, realize:

```
git clone https://github.com/wuerges/cad_geometry.git --recurse-submodules
mkdir build && cd build && cmake ../cad_geometry
make
```