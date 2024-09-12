#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/input.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("An advanced keyboard filter and remapper driver");
MODULE_VERSION("0.2");

#define PROC_NAME "kbd_filter"
#define MAX_FILTERED_KEYS 10

static struct input_handler kbd_filter_handler;

//todo: consider using a more efficient data structure for filtered_keys
static int filtered_keys[MAX_FILTERED_KEYS] = {KEY_A, -1};  // -1 marks end of list

//enhancement: added key remapping functionality
static int remap_key_from = KEY_CAPSLOCK;
static int remap_key_to = KEY_ESC;

//debug: ensure these parameters are set correctly when loading the module
module_param_array(filtered_keys, int, NULL, 0644);
MODULE_PARM_DESC(filtered_keys, "List of keycodes to filter");

module_param(remap_key_from, int, 0644);
MODULE_PARM_DESC(remap_key_from, "Key to remap from");

module_param(remap_key_to, int, 0644);
MODULE_PARM_DESC(remap_key_to, "Key to remap to");

//debug: check if this function is called for every key press
static bool is_key_filtered(int code) {
    int i;
    for (i = 0; i < MAX_FILTERED_KEYS && filtered_keys[i] != -1; i++) {
        if (filtered_keys[i] == code)
            return true;
    }
    return false;
}

static bool kbd_filter_func(struct input_handle *handle, unsigned int type,
                            unsigned int code, int value)
{
    //enhancement: now filters multiple keys based on filtered_keys array
    if (type == EV_KEY && is_key_filtered(code) && value == 1) {
        printk(KERN_INFO "Keyboard Filter: Key %d filtered\n", code);
        return true;
    }
    
    //enhancement: added key remapping
    //todo: consider allowing multiple key remaps
    if (type == EV_KEY && code == remap_key_from) {
        input_event(handle->dev, type, remap_key_to, value);
        return true;
    }
    
    return false;
}

static int kbd_filter_connect(struct input_handler *handler,
                              struct input_dev *dev,
                              const struct input_device_id *id)
{
    struct input_handle *handle;
    int error;

    //allocate memory for the input handle
    //debug: ensure kzalloc doesn't return NULL
    handle = kzalloc(sizeof(struct input_handle), GFP_KERNEL);
    if (!handle)
        return -ENOMEM;

    //setup the handle
    handle->dev = dev;
    handle->handler = handler;
    handle->name = "kbd_filter";

    //register the handle
    //debug: check return value of input_register_handle
    error = input_register_handle(handle);
    if (error)
        goto err_free_handle;

    //open the device
    //debug: check return value of input_open_device
    error = input_open_device(handle);
    if (error)
        goto err_unregister_handle;

    printk(KERN_INFO "Keyboard Filter: Connected to %s\n", dev->name);
    return 0;

//error handling for failed device opening
err_unregister_handle:
    input_unregister_handle(handle);
//error handling for failed handle registration
err_free_handle:
    kfree(handle);
    return error;
}

static void kbd_filter_disconnect(struct input_handle *handle)
{
    //cleanup function: close device, unregister handle, free memory
    printk(KERN_INFO "Keyboard Filter: Disconnected from %s\n", handle->dev->name);
    input_close_device(handle);
    input_unregister_handle(handle);
    kfree(handle);
}

//define which input devices we're interested in
static const struct input_device_id kbd_filter_ids[] = {
    {
        .flags = INPUT_DEVICE_ID_MATCH_EVBIT,
        .evbit = { BIT_MASK(EV_KEY) },
    },
    { },    //terminate with null entry
};

//enhancement: added proc file interface for dynamic configuration
static ssize_t kbd_filter_proc_write(struct file *file, const char __user *buffer,
                                     size_t count, loff_t *ppos)
{
    //debug: ensure buffer size is sufficient for all use cases
    char local_buffer[64];
    int new_key, i;

    if (count > sizeof(local_buffer) - 1)
        return -EINVAL;

    if (copy_from_user(local_buffer, buffer, count))
        return -EFAULT;

    local_buffer[count] = '\0';

    if (sscanf(local_buffer, "%d", &new_key) != 1)
        return -EINVAL;

    //todo: add functionality to remove keys from the filter list
    for (i = 0; i < MAX_FILTERED_KEYS; i++) {
        if (filtered_keys[i] == -1) {
            filtered_keys[i] = new_key;
            break;
        }
    }

    return count;
}

//enhancement: added proc file read function to display current configuration
static int kbd_filter_proc_show(struct seq_file *m, void *v)
{
    int i;
    seq_printf(m, "Filtered keys: ");
    for (i = 0; i < MAX_FILTERED_KEYS && filtered_keys[i] != -1; i++) {
        seq_printf(m, "%d ", filtered_keys[i]);
    }
    seq_printf(m, "\nRemapping: %d -> %d\n", remap_key_from, remap_key_to);
    return 0;
}

static int kbd_filter_proc_open(struct inode *inode, struct file *file)
{
    return single_open(file, kbd_filter_proc_show, NULL);
}

static const struct proc_ops kbd_filter_proc_fops = {
    .proc_open = kbd_filter_proc_open,
    .proc_read = seq_read,
    .proc_lseek = seq_lseek,
    .proc_release = single_release,
    .proc_write = kbd_filter_proc_write,
};

//debug: ensure all error cases are handled in init function
static int __init kbd_filter_init(void)
{
    int error;

    //setup the input handler
    kbd_filter_handler.name = "kbd_filter";
    kbd_filter_handler.event = kbd_filter_func;
    kbd_filter_handler.connect = kbd_filter_connect;
    kbd_filter_handler.disconnect = kbd_filter_disconnect;
    kbd_filter_handler.id_table = kbd_filter_ids;

    error = input_register_handler(&kbd_filter_handler);
    if (error) {
        printk(KERN_ERR "Keyboard Filter: Failed to register input handler\n");
        return error;
    }

    //enhancement: create proc file for runtime configuration
    proc_create(PROC_NAME, 0644, NULL, &kbd_filter_proc_fops);

    printk(KERN_INFO "Keyboard Filter: Module loaded\n");
    return 0;
}

static void __exit kbd_filter_exit(void)
{
    //cleanup: unregister the input handler and remove proc entry
    input_unregister_handler(&kbd_filter_handler);
    remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO "Keyboard Filter: Module unloaded\n");
}

module_init(kbd_filter_init);
module_exit(kbd_filter_exit);