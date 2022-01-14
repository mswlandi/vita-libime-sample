# Vita libIme Sample
Simple example using vitasdk's libime.h and vita2d

Currently sceImeOpen is throwing a prefetch abort exception - help me fix it please :,)

## Building for debugging
```bash
mkdir Debug
cd Debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```