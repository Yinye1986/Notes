#Example

```shell
drwxr-xr-x 3 chris chris 4096 May 2 15:00 alacritty
```

Meaning of each part:
    * drwxr-xr-x: This is the file permission block.
        - d: The first character indicates the type of file. d means it is a directory. If it were a file, there would be a -.
        - rwx: The next three characters represent the owner’s permissions. r stands for read, w for write, and x for execute. So, rwx means the owner (in this case, chris) can read, write, and execute the directory.
        - The next r-x represents the group’s permissions. Here, the group (also chris) can read and execute, but not write to the directory.
        - The final r-x represents the permissions for all other users. They also can read and execute, but not write to the directory.
    * 3: This is the number of hard links to the directory.
    * chris: The first chris is the owner of the file or directory. The second chris is the group that owns the file or directory.
    * 4096: This is the size of the file or directory in bytes.
    * May 2 15:00: This is the last modification time of the file or directory.
    * alacritty: This is the name of the file or directory.
