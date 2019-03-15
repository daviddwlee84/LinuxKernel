# Virtual Machine

## Virtual Box

### Increase / Enlarge / Resize Disk Size

First, modify the virtual disk image size.

* modifyhd  -- change the characteristics of a disk image after it has been created

```sh
VBoxManage modifyhd "path/to/VirtualBox VMs/vm_name/disk_name.vdi" --resize <size in MB>
```

Then, enlarge the size in Ubuntu (using Gparted here)

Download Gparted

```sh
sudo apt-get install gparted
```

Open it

```sh
gparted
```

Right click on the current partition and select `Resize/Move` then drag and drop to the end.

Then press the *green check button* (Apply All Operations). And you're done.

* [How To Resize A VirtualBox Disk (Window and Mac)](https://www.ubergizmo.com/how-to/resize-virtualbox-disk/)
