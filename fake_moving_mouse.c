#include <linux/input.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/delay.h>


static struct input_dev *mouse_dev;

static struct task_struct *move_mouse_kthread;


static int continuously_move_mouse(void *data) {
	for (int i = 0; !kthread_should_stop(); i++) {
		int mouse_diff = 50;
		int mouse_x, mouse_y;
		switch (i % 4) {
			case 0:
				mouse_x = mouse_diff;
				mouse_y = 0;
				break;
			case 1:
				mouse_x = 0;
				mouse_y = mouse_diff;
				break;
			case 2:
				mouse_x = -mouse_diff;
				mouse_y = 0;
				break;
			case 3:
				mouse_x = 0;
				mouse_y = -mouse_diff;
				break;
		}
		input_report_rel(mouse_dev, REL_X, mouse_x);
		input_report_rel(mouse_dev, REL_Y, mouse_y);
		input_sync(mouse_dev);
		msleep(500);
	}
	return 0;
}

static int __init mouse_init(void) {
	int err;
	mouse_dev = input_allocate_device();
	if (!mouse_dev) {
		printk(KERN_ERR "button.c: Not enough memory\n");
		return -ENOMEM;
	}

	mouse_dev->name = "fake_moving_mouse";
	mouse_dev->id.bustype = BUS_USB;
	mouse_dev->id.vendor  = 0x1234;
	mouse_dev->id.product = 0x5678;
	mouse_dev->id.version = 0x0001;

	set_bit(EV_REL, mouse_dev->evbit);
	set_bit(EV_KEY, mouse_dev->evbit);
	set_bit(REL_X, mouse_dev->relbit);
	set_bit(REL_Y, mouse_dev->relbit);
	set_bit(BTN_LEFT, mouse_dev->keybit);

	err = input_register_device(mouse_dev);
	if (err) {
		printk(KERN_ERR "fake_moving_mouse: Failed to register device\n");
		goto err_free_dev;
	}

	move_mouse_kthread = kthread_run(continuously_move_mouse, NULL, "move_fake_moving_mouse");
    if (IS_ERR(move_mouse_kthread)) {
        input_unregister_device(mouse_dev);
        return PTR_ERR(move_mouse_kthread);
    }

	return 0;

err_free_dev:
	input_free_device(mouse_dev);
	return err;
}

static void __exit mouse_exit(void) {
	if (move_mouse_kthread) {
		kthread_stop(move_mouse_kthread);
	}
	input_unregister_device(mouse_dev);
}

module_init(mouse_init);
module_exit(mouse_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("linuskmr");
