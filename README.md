# Linux Keyboard Filter Driver

This project implements a basic keyboard filter driver for Linux. It demonstrates how to intercept and filter keyboard events in the Linux kernel.

## Features

- Filters out specific key presses (currently set to filter the 'A' key)
- Easily extendable to implement more complex filtering logic

## Building the Module

To build the module, ensure you have the necessary kernel headers installed, then run:

```
make
```

## Installing the Module

To install the module, run:

```
sudo insmod kbd_filter.ko
```

## Uninstalling the Module

To remove the module, run:

```
sudo rmmod kbd_filter
```

## Viewing Kernel Logs

To view the kernel logs and see the module's output, use:

```
sudo dmesg | tail
```

## License

This project is licensed under the GPL License - see the LICENSE file for details.#   k e y b o a r d - f i l t e r - d r i v e r  
 #   k e y b o a r d - f i l t e r - d r i v e r  
 