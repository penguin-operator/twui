# TWUI
A terminal windowing user interface library in C.

# [Example](https://github.com/penguin-operator/twui/blob/master/test/test.c)
```c name=test.c
#include <twui.h>

int main(int argc, char **argv, char **envp)
{
	struct twin *root = twinit();
	twprint(root, "Hello, World!\r\n");
	char key;
	while (twread(root, &key, 1), key != 'q');
	twdel(root);
	return 0;
}
```
