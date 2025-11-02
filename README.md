# BBB-UartProtocol

# usag

1. To use this project, copy all files into your Buildroot project's package directory (for example: package/uart_interface). 
```
cp /(path to BBB-UartProtocol) / (path to your buildroot project)/package/uart_interface 
```
2. select your configuration 
```
make beaglebone_defconfig  #(optional)
```
3. enable uart_interface package
```
make menuconfig
```
```
-> Package  --->
    -> uart_interface  --->
        -> [*] uart_interface
``` 
4. then, build your Buildroot project
```
make
```

# custom 
1. chose your desired UART port in main.c
```
 fd = open("your/uart port path"), O_RDWR | O_NOCTTY | O_NDELAY);  
```
2. add your desired command handler in uart_protocol.c
```
static CommandInfo cmd_table[] = {
    {"FOV", "SET", 0, TYPE_FLOAT, NULL, fov_set_handler},
    {"FOV", "GET", 0, TYPE_UNDEFINED, NULL, fov_get_handler},
    .
    .
    .
};
```