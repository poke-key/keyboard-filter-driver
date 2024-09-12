# Enhanced Linux Keyboard Filter and Remapper Driver

This project demonstrates an advanced keyboard filter and remapper driver for Linux, enabling sophisticated interception, filtering, and remapping of keyboard events within the Linux kernel.

## Features

- Configurable key filtering: Filter multiple keys specified via module parameters
- Key remapping: Remap one key to another (e.g., Caps Lock to Esc)
- Dynamic configuration: Use proc filesystem interface to modify settings at runtime
- Easily extendable to implement more advanced filtering and remapping logic

## Building the Module

Ensure you have the necessary kernel headers installed, then build the module by running:

```bash
make
```

## Installing the Module

To install the keyboard filter module with default settings, use:

```bash
sudo insmod kbd_filter.ko
```

To specify custom filtered keys or remapping on load:

```bash
sudo insmod kbd_filter.ko filtered_keys=30,31 remap_key_from=58 remap_key_to=1
```

This example filters keys with codes 30 and 31, and remaps key 58 (typically Caps Lock) to key 1 (typically Esc).

## Runtime Configuration

To add a key to the filter list at runtime:

```bash
echo 32 > /proc/kbd_filter
```

To view current configuration:

```bash
cat /proc/kbd_filter
```

## Uninstalling the Module

To remove the module, run:

```bash
sudo rmmod kbd_filter
```

## Viewing Kernel Logs

To view the kernel logs and check the module's output, use:

```bash
sudo dmesg | tail
```

## License

This project is licensed under the GPL License. For more details, see the [LICENSE](LICENSE) file.

## Contributing

Contributions to enhance the functionality or improve the code are welcome. Please feel free to submit pull requests or open issues to discuss potential improvements.
