#ifndef KBD_FILTER_H
#define KBD_FILTER_H

#include <linux/input.h>

/*prototype functions for driver source, stored in header file for structure's sake*/
static bool kbd_filter_func(struct input_handle *handle, unsigned int type,
                            unsigned int code, int value);
static int kbd_filter_connect(struct input_handler *handler,
                              struct input_dev *dev,
                              const struct input_device_id *id);
static void kbd_filter_disconnect(struct input_handle *handle);

//add consts or func prototypes as needed

#endif