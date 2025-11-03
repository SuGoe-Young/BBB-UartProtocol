# BBB-UartProtocol


## Using it with Buildroot
1. Throw the sources into your package folder, e.g.
   ```bash
   cp -r /path/to/BBB-UartProtocol/* /path/to/buildroot/package/uart_interface/
   ```
2. BBB config:
   ```bash
   make beaglebone_defconfig
   ```
3. Turn the package on:
   ```
   make menuconfig
   -> Package --->
       -> uart_interface --->
           -> [*] uart_interface
   ```
4. Build:
   ```bash
   make
   ```

## make
1. Pick the UART in `main.c`:
   ```c
   fd = open("/dev/ttyS4", O_RDWR | O_NOCTTY | O_NDELAY);
   ```
2. Add your own commands in `uart_protocol.c`:
   ```c
   static const CommandInfo command_table[] = {
       { "FOV", "SET", -1, TYPE_FLOAT, fov_set_handler },
       { "FOV", "GET", -1, TYPE_UNDEFINED, fov_get_handler },
       { "FOO", "BAR", 8, TYPE_CHAR, foo_bar_handler },
   };
   ```
   Setting `data_len` to `-1` means “whatever length comes in”; any other number locks it down.

## Sample frames
Format is `<prefix>:<function>:<action>:<len>:<data>@`
```text
VSEC:FOV:SET:9:45.2,33.6@
VSEC:FOV:GET:0:@
```

