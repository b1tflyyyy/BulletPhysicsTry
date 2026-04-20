## BulletPhysicsTry

### Build

```bash
cmake -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_EXE_LINKER_FLAGS="-fuse-ld=mold" -DCMAKE_SHARED_LINKER_FLAGS="-fuse-ld=mold" -DCMAKE_BUILD_TYPE=Debug ..
ninja
```

### Run

```bash
./myimpl or ./withlib 
```

### Controls

- `Space` - pause/resume simulation
- `R` - reset simulation
- `Esc` - exit application

### Preview

![image](assets/i1.png)
