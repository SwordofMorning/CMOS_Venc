/**
* @file
* @brief Declaraction of os api and struct
* @details Declaraction of os api and struct
* @author VS software group
* @date 2021-02-04
* @version v1.0
* @Copyright (C) 2020, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_OS_H__
#define __VS_OS_H__

#define VS_OS_VERSION    "1.0"

#define OS_GFP_KERNEL    0
#define OS_GFP_ATOMIC    1

typedef enum {
	VS_OS_FALSE = 0,
	VS_OS_TRUE  = 1,
} vs_os_bool_t;

/*
 * Return the value that has been aligned to an ALIGN boundary.
 * ALIGN must be a power of 2.
 */
#define VS_OS_ALIGN(x,align) \
	(((x) + (align) - 1) & ~((align) - 1))

/* memory API */
void *vs_os_kmalloc(unsigned long size, unsigned int gfp_flag);
void *vs_os_kzalloc(unsigned long size, unsigned int gfp_flag);
void vs_os_kfree(const void *addr);
void *vs_os_vmalloc(unsigned long size);
void vs_os_vfree(const void * addr);
void *vs_os_ioremap(unsigned long phys_addr, unsigned long size);
void *vs_os_ioremap_cache(unsigned long phys_addr, unsigned long size);
void vs_os_iounmap(void *addr);
unsigned long
vs_os_copy_from_user(void *to, const void *from, unsigned long size);
unsigned long
vs_os_copy_to_user(void *to, const void *from, unsigned long size);
void *vs_os_memset(void *str, int c, int count);
void *vs_os_memcpy(void *dest, const void *src, int count);
void *vs_os_memmove(void *dest, const void *src, int count);
int vs_os_memcmp(const void *cs, const void *ct, int count);
void vs_os_mb(void);
void vs_os_rmb(void);
void vs_os_wmb(void);
void vs_os_smp_mb(void);
void vs_os_smp_rmb(void);
void vs_os_smp_wmb(void);

#if !defined(MAP_FAILED)
#define MAP_FAILED	((void *)-1)
#endif
#if !defined(MAP_SHARED)
#define MAP_SHARED	0x01
#endif
#if !defined(PROT_READ)
#define PROT_READ	0x01
#endif
#if !defined(PROT_WRITE)
#define PROT_WRITE	0x02
#endif
unsigned long vs_os_mmap(void *filp, unsigned long addr,
	unsigned long len, unsigned long prot,
	unsigned long flag, unsigned long offset);
int vs_os_munmap(unsigned long addr, unsigned long length);


/* semaphore API */
struct vs_os_sema_s {
	void *sema;
};

int vs_os_sema_init(struct vs_os_sema_s *vs_sema, int val);
int vs_os_sema_cnt_get(struct vs_os_sema_s *vs_sema, int *val);
int vs_os_down(struct vs_os_sema_s *vs_sema);
int vs_os_down_interruptible(struct vs_os_sema_s *vs_sema);
int vs_os_down_trylock(struct vs_os_sema_s *vs_sema);
void vs_os_up(struct vs_os_sema_s *vs_sema);
void vs_os_sema_destroy(struct vs_os_sema_s *vs_sema);

/* mutex API */
struct vs_os_mutex_s {
	void *mutex;
};

/**
 * vs_os_mutex_init - create and init a new mutex
 * vs_os_mutex_destroy should be called if exit
 */
int vs_os_mutex_init(struct vs_os_mutex_s *vs_mutex);
int vs_os_mutex_lock(struct vs_os_mutex_s *vs_mutex);
int vs_os_mutex_lock_killable(struct vs_os_mutex_s *vs_mutex);
int vs_os_mutex_lock_interruptible(struct vs_os_mutex_s *vs_mutex);
int vs_os_mutex_trylock(struct vs_os_mutex_s *vs_mutex);
void vs_os_mutex_unlock(struct vs_os_mutex_s *vs_mutex);
/**
 * vs_os_mutex_destroy - destroy the mutex
 */
void vs_os_mutex_destroy(struct vs_os_mutex_s *vs_mutex);

/* spinlock API */
struct vs_os_spinlock_s {
	void *lock;
};

int vs_os_spin_lock_atomic_init(struct vs_os_spinlock_s *vs_lock);
int vs_os_spin_lock_init(struct vs_os_spinlock_s *vs_lock);
void vs_os_spin_lock(struct vs_os_spinlock_s *vs_lock);
int vs_os_spin_trylock(struct vs_os_spinlock_s *vs_lock);
void vs_os_spin_unlock(struct vs_os_spinlock_s *vs_lock);
void vs_os_spin_lock_irqsave(struct vs_os_spinlock_s *vs_lock,
			unsigned long *flags);
void vs_os_spin_unlock_irqrestore(struct vs_os_spinlock_s *vs_lock,
			unsigned long *flags);
void vs_os_spin_lock_destroy(struct vs_os_spinlock_s *vs_lock);

/* timer API */
#define VS_OS_CLOCK_REALTIME			0
#define VS_OS_CLOCK_MONOTONIC			1
#define VS_OS_CLOCK_BOOTTIME			7

struct vs_os_timer_s {
	void *timer;
};
typedef void (*vs_os_timer_handler_t)(struct vs_os_timer_s *);

struct vs_os_timespec_s {
    long tv_sec;
    long tv_nsec;
};

void vs_os_usleep_range(unsigned long min, unsigned long max);
int vs_os_clock_gettime(unsigned int type, struct vs_os_timespec_s *tp);
unsigned long vs_os_msleep(unsigned int msecs);
void vs_os_udelay(unsigned int usecs);
void vs_os_mdelay(unsigned int msecs);
/*
 * Create timer resource, vs_os_timer_destroy should be called if exit
 */
int vs_os_timer_init(struct vs_os_timer_s *vs_timer, vs_os_timer_handler_t handler);
int vs_os_timer_set(struct vs_os_timer_s *vs_timer, unsigned long interval_ms);
int vs_os_timer_del(struct vs_os_timer_s *vs_timer);
int vs_os_timer_del_sync(struct vs_os_timer_s *vs_timer);
/*
 * free resource allocated for timer
 */
int vs_os_timer_destroy(struct vs_os_timer_s *vs_timer);

/* hrtimer API */
#define VS_OS_HRTIMER_STATE_INACTIVE	0
#define VS_OS_HRTIMER_STATE_ENQUEUED	1
#define VS_OS_HRTIMER_STATE_CALLBACK	2

enum vs_os_hrtimer_mode_e {
	VS_OS_HRTIMER_MODE_ABS		= 0x00,
	VS_OS_HRTIMER_MODE_REAL		= 0x01,
	VS_OS_HRTIMER_MODE_PINNED	= 0x02,
	VS_OS_HRTIMER_MODE_SOFT		= 0x04,
	VS_OS_HRTIMER_MODE_HARD		= 0x08,
};

enum vs_os_hrtimer_restart_e {
	VS_OS_HRTIMER_NORESTART = 0,
	VS_OS_HRTIMER_RESTART,
};

struct vs_os_hrtimer_s {
	void *hrtimer;
};
typedef enum vs_os_hrtimer_restart_e (*vs_os_hrtimer_handler_t)(struct vs_os_hrtimer_s *);

long long vs_os_nsec_gettime(unsigned int type);
long long vs_os_ktime_set(const long long sec, const unsigned long nsec);
long long vs_os_ktime_ms_delta(const long long later, const long long earlier);
long long vs_os_ktime_us_delta(const long long later, const long long earlier);
int vs_os_hrtimer_init(struct vs_os_hrtimer_s *vs_hrtimer,
		vs_os_hrtimer_handler_t hr_handler, unsigned int clock_type,
		enum vs_os_hrtimer_mode_e hrtimer_mode);
int vs_os_hrtimer_cancel(struct vs_os_hrtimer_s *vs_hrtimer);
int vs_os_hrtimer_start(struct vs_os_hrtimer_s *vs_hrtimer,
		const long long sec, const unsigned long nsec,
		enum vs_os_hrtimer_mode_e hrtimer_mode);
int vs_os_hrtimer_forward(struct vs_os_hrtimer_s *vs_hrtimer,
		const long long sec, const unsigned long nsec);
vs_os_bool_t vs_os_hrtimer_state_get(struct vs_os_hrtimer_s *vs_hrtimer,
		unsigned int type);
int vs_os_hrtimer_destroy(struct vs_os_hrtimer_s *vs_hrtimer);

/*
 * Device related API definitions
 */
typedef unsigned short		vs_os_umode_t;

void *vs_os_proc_create_data(const char *name, vs_os_umode_t mode,
		void *parent, const void *proc_fops, void *data);
void *vs_os_proc_mkdir(const char *name, void *parent);
void vs_os_proc_entry_remove(const char *name, void *parent);

int vs_os_platform_driver_register(void *drv);
void vs_os_platform_driver_unregister(void *drv);

void *vs_os_platform_get_resource_byname(void *dev,
					      unsigned int type, const char *name);
void *vs_os_platform_get_resource(void *dev,
					      unsigned int type, unsigned int num);
int vs_os_platform_get_irq(void *dev, unsigned int num);
int vs_os_platform_get_irq_byname(void *dev, const char *name);


/*
 * Task, interrupt and wait queue related API Definitions
 */
struct vs_os_list_head_s {
    struct vs_os_list_head_s *next, *prev;
};

#define VS_OS_LIST_HEAD(name) \
	struct vs_os_list_head_s name = {&(name), &(name)}


static inline void vs_os_init_list_head(struct vs_os_list_head_s *list)
{
    list->next = list;
    list->prev = list;
}

/*
 * Insert a new entry between two known consecutive entries.
 */
static inline void __vs_os_list_add(struct vs_os_list_head_s *new,
			      struct vs_os_list_head_s *prev,
			      struct vs_os_list_head_s *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

/**
 * vs_os_list_add - add a new entry
 * @new: new entry to be added
 * @head: list head to add it after
 *
 * Insert a new entry after the specified head.
 */
static inline void vs_os_list_add(struct vs_os_list_head_s *new, struct vs_os_list_head_s *head)
{
	__vs_os_list_add(new, head, head->next);
}

/**
 * vs_os_list_add_tail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head.
 */
static inline void vs_os_list_add_tail(struct vs_os_list_head_s *new, struct vs_os_list_head_s *head)
{
	__vs_os_list_add(new, head->prev, head);
}

static inline void __vs_os_list_del(struct vs_os_list_head_s * prev, struct vs_os_list_head_s * next)
{
	next->prev = prev;
	prev->next = next;
}

static inline void vs_os_list_del_entry(struct vs_os_list_head_s *entry)
{
	__vs_os_list_del(entry->prev, entry->next);
}

#define VS_OS_LIST_POISON1    ((void *) 0x100 + 0xdead000000000000UL)
#define VS_OS_LIST_POISON2    ((void *) 0x200 + 0xdead000000000000UL)

static inline void vs_os_list_del(struct vs_os_list_head_s *entry)
{
    __vs_os_list_del(entry->prev, entry->next);
    entry->next = VS_OS_LIST_POISON1;
    entry->prev = VS_OS_LIST_POISON2;
}

/**
 * vs_os_list_move - delete from one list and add as another's head
 * @list: the entry to move
 * @head: the head that will precede our entry
 */
static inline void vs_os_list_move(struct vs_os_list_head_s *list, struct vs_os_list_head_s *head)
{
	vs_os_list_del_entry(list);
	vs_os_list_add(list, head);
}

/**
 * vs_os_list_empty - tests whether a list is empty
 * @head: the list to test.
 */
static inline int vs_os_list_empty(const struct vs_os_list_head_s *head)
{
	return head->next == head;
}

/**
 * vs_os_list_is_last - tests whether @list is the last entry in list @head
 * @list: the entry to test
 * @head: the head of the list
 */
static inline int vs_os_list_is_last(const struct vs_os_list_head_s *list,
                                    const struct vs_os_list_head_s *head)
{
    return list->next == head;
}

#undef vs_os_offsetof
#ifdef __compiler_offsetof
#define vs_os_offsetof(TYPE, MEMBER)	__compiler_offsetof(TYPE, MEMBER)
#else
#define vs_os_offsetof(TYPE, MEMBER)	((size_t)&((TYPE *)0)->MEMBER)
#endif

#define vs_os_sizeof_field(TYPE, MEMBER) sizeof((((TYPE *)0)->MEMBER))

#define vs_os_offsetofend(TYPE, MEMBER) \
		(vs_os_offsetof(TYPE, MEMBER) + vs_os_sizeof_field(TYPE, MEMBER))

#define vs_os_container_of(ptr, type, member) ({				\
	void *__mptr = (void *)(ptr);					\
	((type *)(__mptr - vs_os_offsetof(type, member))); })

#define vs_os_list_entry(ptr, type, member) \
	vs_os_container_of(ptr, type, member)

#define vs_os_list_first_entry(ptr, type, member) \
	vs_os_list_entry((ptr)->next, type, member)

#define vs_os_list_next_entry(pos, member) \
	vs_os_list_entry((pos)->member.next, typeof(*(pos)), member)

#define vs_os_list_last_entry(ptr, type, member) \
	vs_os_list_entry((ptr)->prev, type, member)

#define vs_os_list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

#define vs_os_list_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)

/**
 * vs_os_list_for_each_entry - iterate over list of given type
 * @pos:	the type * to use as a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the list_head within the struct.
 */
#define vs_os_list_for_each_entry(pos, head, member)                     \
		for (pos = vs_os_list_entry((head)->next, __typeof__(*pos), member); \
			&pos->member != (head); 				\
		     pos = vs_os_list_entry(pos->member.next, __typeof__(*pos), member))

/**
 * vs_os_list_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @pos:	the type * to use as a loop cursor.
 * @n:		another type * to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the list_head within the struct.
 */
#define vs_os_list_for_each_entry_safe(pos, n, head, member)		\
		for (pos = vs_os_list_first_entry(head, typeof(*pos), member),	\
			n = vs_os_list_next_entry(pos, member); 		\
		     &pos->member != (head);					\
		     pos = n, n = vs_os_list_next_entry(n, member))

/* kthread */
struct vs_os_kthread_worker {
	void *worker;
	void *task;
};

struct vs_os_thread_work_s {
	void *work;
};
typedef void (*vs_os_kthread_work_handler_s)(struct vs_os_thread_work_s *);
typedef int (*vs_os_threadfn_t)(void *data);

void *vs_os_kthread_run(vs_os_threadfn_t vs_thread, void *data, const char *name);
vs_os_bool_t vs_os_kthread_should_stop(void);
int vs_os_kthread_stop(void *task);
int vs_os_kthread_init_worker(struct vs_os_kthread_worker *vs_worker, const char *name);
int vs_os_kthread_init_work(struct vs_os_thread_work_s *vs_work, vs_os_kthread_work_handler_s work_handler);
vs_os_bool_t vs_os_kthread_queue_work(struct vs_os_kthread_worker *vs_worker, struct vs_os_thread_work_s *vs_work);
int vs_os_thread_flush_worker(struct vs_os_kthread_worker *vs_worker);
int vs_os_kthread_work_destroy(struct vs_os_thread_work_s *vs_work);
int vs_os_kthread_worker_destroy(struct vs_os_kthread_worker *vs_worker);

/* Scheduling policies */
#define VS_OS_SCHED_NORMAL		0
#define VS_OS_SCHED_FIFO		1
#define VS_OS_SCHED_RR			2
#define VS_OS_SCHED_BATCH		3
#define VS_OS_SCHED_IDLE		5
#define VS_OS_SCHED_DEADLINE	6

void vs_os_schedule(void);
int vs_os_sched_setscheduler(void *task, int policy, const int sched_priority);

enum vs_os_irqreturn_e {
    VS_OS_IRQ_NONE = (0 << 0),
    VS_OS_IRQ_HANDLED = (1 << 0),
    VS_OS_IRQ_WAKE_THREAD = (1 << 1),
};

typedef int (*vs_os_irq_handler_t)(int, void *);
int vs_os_request_threaded_irq(unsigned int irq, vs_os_irq_handler_t vs_handler,
			vs_os_irq_handler_t vs_thread_fn, unsigned long irqflags,
			const char *name, void *dev);
void vs_os_free_irq(unsigned int irq, void *dev);
int vs_os_in_interrupt(void);


struct vs_os_work_s {
    void *work;
    void (*func)(struct vs_os_work_s *work);
};
typedef void (*vs_os_work_func_t)(struct vs_os_work_s *work);

/* create work resource, vs_os_work_destroy should be called if exit */
int vs_os_work_init(struct vs_os_work_s *vs_work, vs_os_work_func_t vs_func);

#define VS_OS_INIT_WORK(_work, _func)      \
    do {                                  \
        vs_os_work_init((_work), (_func)); \
    } while (0)

vs_os_bool_t vs_os_work_schedule(struct vs_os_work_s *vs_work);
/* free work resource */
void vs_os_work_destroy(struct vs_os_work_s *vs_work);

unsigned long vs_os_msecs_to_jiffies(const unsigned int m);
long vs_os_schedule_timeout(long timeout);

struct vs_os_completion_s {
    void *completion;
};

int vs_os_completion_init(struct vs_os_completion_s *vs_cp);
int vs_os_completion_reinit(struct vs_os_completion_s *vs_cp);
int vs_os_completion_destroy(struct vs_os_completion_s *vs_cp);
unsigned long vs_os_wait_for_completion_timeout(
		struct vs_os_completion_s *vs_cp,
		unsigned long timeout);
int vs_os_wait_for_completion(
		struct vs_os_completion_s *vs_cp);
int vs_os_complete(struct vs_os_completion_s *vs_cp);
int vs_os_complete_all(struct vs_os_completion_s *vs_cp);

/*
 * File definitions
 */
void *vs_os_fopen(const char *filename, int flags, int mode);
int vs_os_fsize_get(void *fp);
void vs_os_fclose(void *fp);
int vs_os_fwrite(const char *buf, int len, void *fp);
int vs_os_fread(char *buf, unsigned int len, void *fp);


/*
 * Utility definitions: atomic, register, string, etc.
 */

struct vs_os_atomic_s {
    void *atomic;
};

int vs_os_atomic_init(struct vs_os_atomic_s *vs_atomic);
void vs_os_atomic_destroy(struct vs_os_atomic_s *vs_atomic);
int vs_os_atomic_read(struct vs_os_atomic_s *vs_atomic);
void vs_os_atomic_set(struct vs_os_atomic_s *vs_atomic, int i);
int vs_os_atomic_inc_return(struct vs_os_atomic_s *vs_atomic);
int vs_os_atomic_dec_return(struct vs_os_atomic_s *vs_atomic);

char *vs_os_strncpy(char *dst, const char *src, int len);
int vs_os_strlcpy(char *dst, const char *src, int len);
char *vs_os_strncat(char *dst, const char *src, int len);
int vs_os_strlcat(char *dst, const char *src, int len);
int vs_os_strncmp(const char *s1, const char *s2, int len);
int vs_os_strcasecmp(const char *s1, const char *s2);
int vs_os_strncasecmp(const char *s1, const char *s2, int len);
char *vs_os_strchr(const char *s, int c);
char *vs_os_strrchr(const char *s, int c);
char *vs_os_strstr(const char *haystack, const char *needle);
unsigned long vs_os_strlen(const char *s);

int vs_os_printk(const char *fmt, ...);

/* register interfaces */
unsigned int vs_os_csr_get(volatile void *addr);
unsigned int vs_os_csr_field_get(volatile void *addr,
	unsigned int offset, unsigned int width);
void vs_os_csr_set(volatile void *addr, unsigned int val);
void vs_os_csr_field_set(volatile void *addr, unsigned int offset,
	unsigned int width, unsigned int val);

/* kref API*/
struct vs_os_kref_s {
	void *kref;
};
typedef void (*vs_os_kref_handler_t)(struct vs_os_kref_s *);

int vs_os_kref_init(struct vs_os_kref_s *vs_kref);
int vs_os_kref_get(struct vs_os_kref_s *vs_kref);
int vs_os_kref_put(struct vs_os_kref_s *vs_kref,
		vs_os_kref_handler_t kref_handler);
int vs_os_kref_read(struct vs_os_kref_s *vs_kref);

#endif /* end of __VS_OS_H__ */
