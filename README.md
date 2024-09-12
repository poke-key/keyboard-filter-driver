# Linux Keyboard Filter Driver

This project demonstrates how to implement a basic keyboard filter driver for Linux, enabling the interception and filtering of keyboard events within the Linux kernel.

## Features

- Filters specific key presses (currently filters the 'A' key)
- Easily extendable to implement more advanced filtering logic

## Building the Module

Ensure you have the necessary kernel headers installed, then build the module by running:

```bash
make
```

## Installing the Module

To install the keyboard filter module, use the following command:

```bash
sudo insmod kbd_filter.ko
```

## Uninstalling the Module

To remove the module, run:

```bash
sudo rmmod kbd_filter
```

## Viewing Kernel Logs

To view the kernel logs and check the module’s output, use:

```bash
sudo dmesg | tail
```

## License

This project is licensed under the GPL License. For more details, see the [LICENSE](LICENSE) file.