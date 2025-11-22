# Fake Moving Mouse

In times of cooperate tools such as [Microsoft Teams reporting your active status depending on your computer activity](https://learn.microsoft.com/en-us/microsoftteams/presence-admins#automatic-status-settings), it can be a [pressing question how to simulate work](https://www.reddit.com/r/MicrosoftTeams/comments/1aksxbt/whats_the_best_way_to_show_always_available/), for example if you do not particularly like your job. To avoid you to buy a [mouse jiggler](https://www.amazon.com/s?k=mouse+jiggler), this project provides a software solution.

Specifically, a Linux Loadable Kernel Module (LKM) implements a driver for a fake virtual mouse that continuously moves your mouse pointer, thus keeping your status active.


## Installation

1. Install Linux kernel headers (if not already installed):

```
sudo apt update
sudo apt install linux-headers-$(uname -r)
```

2. Compile and insert the kernel module:

```bash
make
sudo insmod fake_moving_mouse.ko
```

3. Your mouse should move automatically now. You can verify the module is loaded and the device is created:

```bash
$ lsmod | grep fake_moving_mouse
fake_moving_mouse      12288  0

$ cat /proc/bus/input/devices
I: Bus=0003 Vendor=1234 Product=5678 Version=0001
N: Name="fake_moving_mouse"
P: Phys=
S: Sysfs=/devices/virtual/input/input21
U: Uniq=
H: Handlers=mouse3 event12 
B: PROP=0
B: EV=7
B: KEY=10000 0 0 0 0
B: REL=3
```

## Uninstallation

Remove the kernel module:

```bash
sudo rmmod fake_moving_mouse
```