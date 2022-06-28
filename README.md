# Toy RISC-V OS

## Environment Setup

On macOS

- Install QEMU
  ```
  brew install qemu
  ```

- Install RISC-V toolchain:
  ```
  $ brew tap riscv-software-src/riscv
  $ brew install riscv-tools
  ```

- Install OpenOCD
  ```
  $ brew install riscv-openocd
  ```

Then, verify the installation by `riscv-tools`.


## Make
```
$ make
```

## Run
```
$ make qemu
```
Press Ctrl-A and then X to exit QEMU