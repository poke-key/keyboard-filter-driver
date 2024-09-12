#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/input.h>

//standard module information, required for all kernel modules
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple keyboard filter driver");
MODULE_VERSION("0.1");

//declare the input handler structure
static struct input_handler kbd_filter_handler;

static bool kbd_filter_func(struct input_handle *handle, unsigned int type,
                            unsigned int code, int value)
{
    //filter function: currently only filters 'A' key
    //todo: expand to filter more keys or key combinations
    if (type == EV_KEY && code == KEY_A && value == 1) {
        printk(KERN_INFO "Keyboard Filter: 'A' key pressed and filtered\n");
        return true; //filter event out
    }
    return false; //allow all other events
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

static int __init kbd_filter_init(void)
{
    int error;

    //setup the input handler
    kbd_filter_handler.name = "kbd_filter";
    kbd_filter_handler.event = kbd_filter_func;
    kbd_filter_handler.connect = kbd_filter_connect;
    kbd_filter_handler.disconnect = kbd_filter_disconnect;
    kbd_filter_handler.id_table = kbd_filter_ids;

    //register the input handler
    //debug: check return value of input_register_handler
    error = input_register_handler(&kbd_filter_handler);
    if (error) {
        printk(KERN_ERR "Keyboard Filter: Failed to register input handler\n");
        return error;
    }

    printk(KERN_INFO "Keyboard Filter: Module loaded\n");
    return 0;
}

static void __exit kbd_filter_exit(void)
{
    //cleanup: unregister the input handler
    input_unregister_handler(&kbd_filter_handler);
    printk(KERN_INFO "Keyboard Filter: Module unloaded\n");
}

//specify the init and exit functions
module_init(kbd_filter_init);
module_exit(kbd_filter_exit);