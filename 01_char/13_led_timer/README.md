## 使用

| **函数**                                                     | **说明**                                                     |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| **timer_setup(timer, callback, flags)**                      | 初始化定时器，主要是初始化 timer_list 结构体， 设置其中的函数、flags。 |
| **DEFINE_TIMER(_name, _function)**                           | 初始化定时器，主要是定义并初始化 timer_list 结构体， 设置其中的函数、flags默认为0。 |
| **void add_timer(struct timer_list \*timer)**                | 向内核添加定时器。timer->expires 表示超时时间。 当超时时间到达，内核就会调用这个函数：timer->function(timer->data)。 |
| **int mod_timer(struct timer_list \*timer, unsigned long expires)** | 修改定时器的超时时间                                         |
| **int timer_pending(const struct timer_list \* timer)**      | 定时器状态查询，如果在系统的定时器列表中则返回1，否则返回0； |
| **int del_timer(struct timer_list \* timer)**                | 删除定时器。                                                 |





当前[最新kernel](https://github.com/Gonglja/linux)源码，二者均能开启定时器, 区别在于使用 `mod_timer` 时传入 `timer` 和`超时时间`，而`add_timer`则使用`timer`自身的超时时间

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

