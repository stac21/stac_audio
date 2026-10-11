# StacAudio
Collection of Audio Related Utilities to read audio files, play them in real-time, and manipulate the properties of their signal

## Dependencies
Requires the portaudio, sndfile, and fftw3 libraries to be installed onto the system

## Build Instructions:
```
mkdir build
cd build
cmake ..
cmake --build .
```

## Installation Instructions:
```
cd build
cmake --install .
```

## To build tests:
```
cd build
cmake .. -DBUILD_TESTS=ON
cmake --build .
```
