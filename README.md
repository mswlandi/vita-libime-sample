# Vita libIme Sample
Simple libIme (SceIme) example using vitasdk's libime.h and vita2d - nothing is done with the text buffer, but the keyboard open correctly.

thanks to SonicMastr for spotting the missing module loading

## Building for debugging
```bash
mkdir Debug
cd Debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```