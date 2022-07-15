## 使用

https://stackoverflow.com/questions/14953871/how-to-pass-custom-argument-to-linux-timers-function


当前最新kernel源码，二者均能开启定时器, 区别在于使用 `mod_timer` 时传入 `timer` 和`超时时间`，而`add_timer`则使用`timer`自身的超时时间
```c
// kernel/timer/timer.c
int mod_timer(struct timer_list *timer, unsigned long expires)
{
	return __mod_timer(timer, expires, 0);
}
EXPORT_SYMBOL(mod_timer);
...
void add_timer(struct timer_list *timer)
{
	BUG_ON(timer_pending(timer));
	__mod_timer(timer, timer->expires, MOD_TIMER_NOTPENDING);
}
EXPORT_SYMBOL(add_timer);

```


## 参考

1. [Linux5.4.70内核定时器的使用](https://blog.csdn.net/ZHONGCAI0901/article/details/120484815)

2. [from_timer 用法](https://stackoverflow.com/questions/14953871/how-to-pass-custom-argument-to-linux-timers-function)