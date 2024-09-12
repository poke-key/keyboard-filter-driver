# Enhanced Linux Keyboard Filter and Remapper Driver

This project demonstrates an advanced keyboard filter and remapper driver for Linux, enabling sophisticated interception, filtering, and remapping of keyboard events within the Linux kernel.

## Features

- Configurable key filtering: Filter multiple keys specified via module parameters
- Key remapping: Remap one key to another (e.g., Caps Lock to Esc)
- Dynamic configuration: Use proc filesystem interface to modify settings at runtime
- Easily extendable to implement more advanced filtering and remapping logic

## Technical Concepts and Implementation Details

This project showcases several important concepts in Linux kernel development and low-level systems programming:

1. Kernel Module Development: The driver is implemented as a loadable kernel module, demonstrating the lifecycle of kernel modules (initialization, operation, and cleanup).

2. Input Subsystem Interaction: The driver interacts with the Linux input subsystem, showing how to hook into and modify the behavior of input devices.

3. Data Structures: 
   - Arrays are used for storing filtered keys.
   - Linked lists (implicitly used by the input subsystem) for managing input handles.

4. Memory Management: 
   - Use of kzalloc for kernel space memory allocation.
   - Proper memory cleanup to prevent leaks.

5. Concurrency and Synchronization: 
   - The driver operates in a concurrent environment, handling potential race conditions.

6. Error Handling: Demonstrates proper error checking and handling in kernel space.

7. Proc Filesystem: Utilizes the proc filesystem for dynamic configuration, showcasing user space and kernel space interaction.

8. Kernel Parameters: Implements module parameters for configurable behavior at load time.

9. Debugging Techniques:
   - Use of printk for kernel logging.
   - Strategic placement of debug messages for troubleshooting.

10. Linux Kernel API: Extensive use of kernel APIs for input handling, proc file operations, and module management.

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

## Debugging

For more verbose output, you can add additional `printk` statements in the source code. Use different log levels (e.g., KERN_DEBUG, KERN_INFO) to categorize messages. After modifying and rebuilding the module, use `dmesg` or check `/var/log/kern.log` for debug output.

## License

This project is licensed under the GPL License. For more details, see the [LICENSE](LICENSE) file.

## Contributing

Contributions to enhance the functionality or improve the code are welcome. Please feel free to submit pull requests or open issues to discuss potential improvements.

## Future Enhancements

Potential areas for future development include:
- Implementing a more efficient data structure for storing filtered keys
- Adding support for key combinations or sequences
- Enhancing the proc interface to allow removal of filtered keys
- Implementing more advanced remapping features (e.g., one-to-many key remapping)
