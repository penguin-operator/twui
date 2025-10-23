# TWUI
A terminal windowing user interface library in C.

# [Example](https://github.com/penguin-operator/twui/blob/master/test/test.c)
```c name=test.c
#include <twui.h>

int main(int argc, char **argv, char **envp)
{
	struct twin *root = winit();
	wprint(root, "Hello, World!\r\n");
	char key;
	while (wread(root, &key, 1), key != 'q');
	wdel(root);
	return 0;
}
```
