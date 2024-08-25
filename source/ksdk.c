#include "ksdk.h"

uint64_t cachedKernelBase;

void(*Xfast_syscall)(void);
int(*copyin)(const void* uaddr, void* kaddr, uint64_t len);
int(*copyout)(const void* kaddr, void* uaddr, uint64_t len);
int(*printf)(const char* fmt, ...);
int(*vprintf)(const char* fmt, va_list arg);
void* (*malloc)(uint64_t size, void* type, int flags);
void(*free)(void* addr, void* type);
void* (*memcpy)(void* dest, const void* src, uint64_t num);
void* (*memset)(void* ptr, int value, uint64_t num);
int(*memcmp)(const void* ptr1, const void* ptr2, uint64_t num);
void* (*kmem_alloc)(struct vm_map* map, uint64_t size);
uint64_t(*strlen)(const char* str);
char* (*strcpy)(char* dst, const char* src);
char* (*strncmp)(char* dst, const char* src, uint64_t len);
void(*pause)(const char* wmesg, int timo);
int(*kthread_add)(void (*func)(void*), void* arg, struct proc* procp, struct thread** newtdpp, int flags, int pages, const char* fmt, ...);
void(*kthread_exit)(void);
void(*sched_prio)(struct thread* td, uint16_t prio);
void(*sched_add)(struct thread* td, uint64_t cpuset);
void(*kern_yield)(uint64_t p);
int(*fill_regs)(struct thread* td, struct reg* rg);
int(*set_regs)(struct thread* td, struct reg* rg);
int(*create_thread)(struct thread* td, uint64_t ctx, void (*start_func)(void*), void* arg, char* stack_base, uint64_t stack_size, char* tls_base, long* child_tid, long* parent_tid, uint64_t flags, uint64_t rtp);
int(*kproc_create)(void (*func)(void*), void* arg, struct proc** newpp, int flags, int pages, const char* fmt, ...);
void(*kthread_set_affinity)(const char* tdname, uint64_t prio, uint64_t cpuset, uint64_t unknown);
void(*kthread_suspend_check)(void);
int(*kproc_kthread_add)(void (*func)(void*), void* arg, struct proc** procptr, struct thread** tdptr, int flags, int pages, char* procname, const char* fmt, ...);
void(*sx_init_flags)(struct sx* sx, const char* description, int opts);
void(*sx_xlock)(struct sx* sx);
void(*sx_xunlock)(struct sx* sx);
void(*mtx_init)(struct mtx* mutex, const char* name, const char* type, int opts);
void(*mtx_lock_spin_flags)(struct mtx* mutex, int flags);
void(*mtx_unlock_spin_flags)(struct mtx* mutex, int flags);
void(*mtx_lock_sleep)(struct mtx* mutex, int flags);
void(*mtx_unlock_sleep)(struct mtx* mutex, int flags);
int(*kern_reboot)(int magic);
void(*vm_map_lock_read)(struct vm_map* map);
int(*vm_map_lookup_entry)(struct vm_map* map, uint64_t address, struct vm_map_entry** entries);
void(*vm_map_unlock_read)(struct vm_map* map);
struct vmspace* (*vmspace_acquire_ref)(struct proc* p);
void(*vmspace_free)(struct vmspace* vm);
int(*vm_map_delete)(struct vm_map* map, uint64_t start, uint64_t end);
int(*vm_map_protect)(struct vm_map* map, uint64_t start, uint64_t end, int new_prot, uint64_t set_max);
int(*vm_map_findspace)(struct vm_map* map, uint64_t start, uint64_t length, uint64_t* addr);
int(*vm_map_insert)(struct vm_map* map, uint64_t object, uint64_t offset, uint64_t start, uint64_t end, int prot, int max, int cow);
void(*vm_map_lock)(struct vm_map* map);
void(*vm_map_unlock)(struct vm_map* map);
int(*proc_rwmem)(struct proc* p, struct uio* uio);

uint8_t* disable_console_output;
void* M_TEMP;
void** kernel_map;
void** prison0;
void** rootvnode;
void** allproc;
struct sysent* sysents;

uint64_t get_kbase() {
    uint32_t edx, eax;
    __asm__("rdmsr" : "=d"(edx), "=a"(eax) : "c"(0xC0000082));
    return ((((uint64_t)edx) << 32) | (uint64_t)eax) - __Xfast_syscall;
}

void init_505sdk(uint8_t* kbase) {
    Xfast_syscall = (void*)(kbase + 0x1C0);
    copyin = (void*)(kbase + 0x1EA710);
    copyout = (void*)(kbase + 0x1EA630);
    printf = (void*)(kbase + 0x436040);
    vprintf = (void*)(kbase + 0x4360B0);
    malloc = (void*)(kbase + 0x10E250);
    free = (void*)(kbase + 0x10E460);
    memcpy = (void*)(kbase + 0x1EA530);
    memset = (void*)(kbase + 0x3205C0);
    memcmp = (void*)(kbase + 0x50AC0);
    kmem_alloc = (void*)(kbase + 0xFCC80);
    strlen = (void*)(kbase + 0x3B71A0);
    strcpy = (void*)(kbase + 0x8F250);
    strncmp = (void*)(kbase + 0x1B8FE0);
    pause = (void*)(kbase + 0x3FB920);
    kthread_add = (void*)(kbase + 0x138360);
    kthread_exit = (void*)(kbase + 0x138640);
    sched_prio = (void*)(kbase + 0x2EFF70);
    sched_add = (void*)(kbase + 0x31F150);
    kern_yield = (void*)(kbase + 0x3FBC40);
    fill_regs = (void*)(kbase + 0x234BA0);
    set_regs = (void*)(kbase + 0x235A80);
    create_thread = (void*)(kbase + 0x1BE1F0);
    kproc_create = (void*)(kbase + 0x137DF0);
    kthread_set_affinity = (void*)(kbase + 0x138CC0);
    kthread_suspend_check = (void*)(kbase + 0x138A60);
    kproc_kthread_add = (void*)(kbase + 0x138B70);
    sx_init_flags = (void*)(kbase + 0xF5BB0);
    sx_xlock = (void*)(kbase + 0xF5E10);
    sx_xunlock = (void*)(kbase + 0xF5FD0);
    mtx_init = (void*)(kbase + 0x402780);
    mtx_lock_spin_flags = (void*)(kbase + 0x402100);
    mtx_unlock_spin_flags = (void*)(kbase + 0x4022C0);
    mtx_lock_sleep = (void*)(kbase + 0x401CD0);
    mtx_unlock_sleep = (void*)(kbase + 0x401FA0);
    kern_reboot = (void*)(kbase + 0x10D390);
    vm_map_lock_read = (void*)(kbase + 0x19F140);
    vm_map_lookup_entry = (void*)(kbase + 0x1A3430);
    vm_map_unlock_read = (void*)(kbase + 0x19F190);
    vmspace_acquire_ref = (void*)(kbase + 0x19EF90);
    vmspace_free = (void*)(kbase + 0x19EDC0);
    vm_map_delete = (void*)(kbase + 0x1A19D0);
    vm_map_protect = (void*)(kbase + 0x1A3A50);
    vm_map_findspace = (void*)(kbase + 0x1A1F60);
    vm_map_insert = (void*)(kbase + 0x1A0280);
    vm_map_lock = (void*)(kbase + 0x19EFF0);
    vm_map_unlock = (void*)(kbase + 0x19F060);
    proc_rwmem = (void*)(kbase + 0x30D150);
    disable_console_output = (void*)(kbase + 0x19ECEB0);
    M_TEMP = (void*)(kbase + 0x14B4110);
    kernel_map = (void*)(kbase + 0x1AC60E0);
    prison0 = (void*)(kbase + 0x10986A0);
    rootvnode = (void*)(kbase + 0x22C1A70);
    allproc = (void*)(kbase + 0x2382FF8);
    sysents = (void*)(kbase + 0x107C610);
}

void init_672sdk(uint8_t* kbase) {
    Xfast_syscall = (void*)(kbase + 0x1C0);
    copyin = (void*)(kbase + 0x3C17A0);
    copyout = (void*)(kbase + 0x3C16B0);
    printf = (void*)(kbase + 0x123280);
    vprintf = (void*)(kbase + 0x1232F0);
    malloc = (void*)(kbase + 0xD7A0);
    free = (void*)(kbase + 0xD9A0);
    memcpy = (void*)(kbase + 0x3C15B0);
    memset = (void*)(kbase + 0x1687D0);
    memcmp = (void*)(kbase + 0x207E40);
    kmem_alloc = (void*)(kbase + 0x250730);
    strlen = (void*)(kbase + 0x2433E0);
    strcpy = (void*)(kbase + 0x2390C0);
    strncmp = (void*)(kbase + 0x39B6E0);
    pause = (void*)(kbase + 0x22A080);
    kthread_add = (void*)(kbase + 0x8A600);
    kthread_exit = (void*)(kbase + 0x8A8F0);
    sched_prio = (void*)(kbase + 0x4453C0);
    sched_add = (void*)(kbase + 0x445F20);
    kern_yield = (void*)(kbase + 0x22A3A0);
    fill_regs = (void*)(kbase + 0xA36D0);
    set_regs = (void*)(kbase + 0xA3670);
    create_thread = (void*)(kbase + 0x4A6FB0);
    kproc_create = (void*)(kbase + 0x8A0A0);
    kthread_set_affinity = (void*)(kbase + 0x8AF70);
    kthread_suspend_check = (void*)(kbase + 0x8AD10);
    kproc_kthread_add = (void*)(kbase + 0x8AE20);
    sx_init_flags = (void*)(kbase + 0x42450);
    sx_xlock = (void*)(kbase + 0x426C0);
    sx_xunlock = (void*)(kbase + 0x42880);
    mtx_init = (void*)(kbase + 0x496FE0);
    mtx_lock_spin_flags = (void*)(kbase + 0x496970);
    mtx_unlock_spin_flags = (void*)(kbase + 0x496B30);
    mtx_lock_sleep = (void*)(kbase + 0x496540);
    mtx_unlock_sleep = (void*)(kbase + 0x496810);
    kern_reboot = (void*)(kbase + 0x206D50);
    vm_map_lock_read = (void*)(kbase + 0x44CD40);
    vm_map_lookup_entry = (void*)(kbase + 0x44D330);
    vm_map_unlock_read = (void*)(kbase + 0x44CD90);
    vmspace_acquire_ref = (void*)(kbase + 0x44CB90);
    vmspace_free = (void*)(kbase + 0x44C9C0);
    vm_map_delete = (void*)(kbase + 0x44F8A0);
    vm_map_protect = (void*)(kbase + 0x451BF0);
    vm_map_findspace = (void*)(kbase + 0x44FE60);
    vm_map_insert = (void*)(kbase + 0x44DEF0);
    vm_map_lock = (void*)(kbase + 0x44CBF0);
    vm_map_unlock = (void*)(kbase + 0x44CC60);
    proc_rwmem = (void*)(kbase + 0x10EE10);
    disable_console_output = (void*)(kbase + 0x1A6EB18);
    M_TEMP = (void*)(kbase + 0x1540EB0);
    kernel_map = (void*)(kbase + 0x220DFC0);
    prison0 = (void*)(kbase + 0x113E518);
    rootvnode = (void*)(kbase + 0x2300320);
    allproc = (void*)(kbase + 0x22BBE80);
    sysents = (void*)(kbase + 0x111E000);
}


void init_70Xsdk(uint8_t* kbase) {
    Xfast_syscall = (void*)(kbase + 0x1C0);
    copyin = (void*)(kbase + 0x2F230);
    copyout = (void*)(kbase + 0x2F140);
    printf = (void*)(kbase + 0xBC730);
    vprintf = (void*)(kbase + 0xBC7A0);
    malloc = (void*)(kbase + 0x301840);
    free = (void*)(kbase + 0x301A40);
    memcpy = (void*)(kbase + 0x2F040);
    memset = (void*)(kbase + 0x2DFC20);
    memcmp = (void*)(kbase + 0x207500);
    kmem_alloc = (void*)(kbase + 0x1170F0);
    strlen = (void*)(kbase + 0x93FF0);
    strcpy = (void*)(kbase + 0x2CC70);
    strncmp = (void*)(kbase + 0x3DABE0);
    pause = (void*)(kbase + 0x16EEE0);
    kthread_add = (void*)(kbase + 0xC46D0);
    kthread_exit = (void*)(kbase + 0xC49C0);
    sched_prio = (void*)(kbase + 0x3281F0);
    sched_add = (void*)(kbase + 0x328D50);
    kern_yield = (void*)(kbase + 0x16F210);
    fill_regs = (void*)(kbase + 0x4940A0);
    set_regs = (void*)(kbase + 0x4941D0);
    create_thread = (void*)(kbase + 0x842E0);
    kproc_create = (void*)(kbase + 0xC4170);
    kthread_set_affinity = (void*)(kbase + 0xC5030);
    kthread_suspend_check = (void*)(kbase + 0xC4DD0);
    kproc_kthread_add = (void*)(kbase + 0xC4EE0);
    sx_init_flags = (void*)(kbase + 0x1ADDB0);
    sx_xlock = (void*)(kbase + 0x1AE030);
    sx_xunlock = (void*)(kbase + 0x1AE1F0);
    mtx_init = (void*)(kbase + 0xBBB10);
    mtx_lock_spin_flags = (void*)(kbase + 0x49400);
    mtx_unlock_spin_flags = (void*)(kbase + 0xBB650);
    mtx_lock_sleep = (void*)(kbase + 0xBB060);
    mtx_unlock_sleep = (void*)(kbase + 0xBB330);
    kern_reboot = (void*)(kbase + 0x2CD780);
    vm_map_lock_read = (void*)(kbase + 0x25FB90);
    vm_map_lookup_entry = (void*)(kbase + 0x260190);
    vm_map_unlock_read = (void*)(kbase + 0x25FBE0);
    vmspace_acquire_ref = (void*)(kbase + 0x25F9F0);
    vmspace_free = (void*)(kbase + 0x25F820);
    vm_map_delete = (void*)(kbase + 0x260BD0);
    vm_map_protect = (void*)(kbase + 0x36AE90);
    vm_map_findspace = (void*)(kbase + 0x262CC0);
    vm_map_insert = (void*)(kbase + 0x260D60);
    vm_map_lock = (void*)(kbase + 0x25FA50);
    vm_map_unlock = (void*)(kbase + 0x25FAB0);
    proc_rwmem = (void*)(kbase + 0x43E80);
    disable_console_output = (void*)(kbase + 0x1A6EAA0);
    M_TEMP = (void*)(kbase + 0x1A7AE50);
    kernel_map = (void*)(kbase + 0x21C8EE0);
    prison0 = (void*)(kbase + 0x113E398);
    rootvnode = (void*)(kbase + 0x22C5750);
    allproc = (void*)(kbase + 0x1B48318);
    sysents = (void*)(kbase + 0x1125660);
}

void init_75Xsdk(uint8_t* kbase) {
    Xfast_syscall = (void*)(kbase + 0x1C0);
    copyin = (void*)(kbase + 0x28F9F0);
    copyout = (void*)(kbase + 0x28F900);
    printf = (void*)(kbase + 0x26F740);
    vprintf = (void*)(kbase + 0x26F7B0);
    malloc = (void*)(kbase + 0x1D6680);
    free = (void*)(kbase + 0x1D6870);
    memcpy = (void*)(kbase + 0x28F800);
    memset = (void*)(kbase + 0x8D6F0);
    memcmp = (void*)(kbase + 0x31D250);
    kmem_alloc = (void*)(kbase + 0x1753E0);
    strlen = (void*)(kbase + 0x2E8BC0);
    strcpy = (void*)(kbase + 0x46B0B0);
    strncmp = (void*)(kbase + 0xBF670);
    pause = (void*)(kbase + 0x86E80);
    kthread_add = (void*)(kbase + 0xDE50);
    kthread_exit = (void*)(kbase + 0xE140);
    sched_prio = (void*)(kbase + 0x18FDF0);
    sched_add = (void*)(kbase + 0x190930);
    kern_yield = (void*)(kbase + 0x871B0);
    fill_regs = (void*)(kbase + 0x3DB0D0);
    set_regs = (void*)(kbase + 0x3DB200);
    create_thread = (void*)(kbase + 0x47AB60);
    kproc_create = (void*)(kbase + 0xD8F0);
    kthread_set_affinity = (void*)(kbase + 0xE7C0);
    kthread_suspend_check = (void*)(kbase + 0xE550);
    kproc_kthread_add = (void*)(kbase + 0xE670);
    sx_init_flags = (void*)(kbase + 0xD1380);
    sx_xlock = (void*)(kbase + 0xD1600);
    sx_xunlock = (void*)(kbase + 0xD17C0);
    mtx_init = (void*)(kbase + 0x3105B0);
    mtx_lock_spin_flags = (void*)(kbase + 0x30FF30);
    mtx_unlock_spin_flags = (void*)(kbase + 0x3100F0);
    mtx_lock_sleep = (void*)(kbase + 0x30FB10);
    mtx_unlock_sleep = (void*)(kbase + 0x30FDD0);
    kern_reboot = (void*)(kbase + 0xD28E0);
    vm_map_lock_read = (void*)(kbase + 0x2FC430);
    vm_map_lookup_entry = (void*)(kbase + 0x2FCA70);
    vm_map_unlock_read = (void*)(kbase + 0x2FC480);
    vmspace_acquire_ref = (void*)(kbase + 0x2FC290);
    vmspace_free = (void*)(kbase + 0x2FC0C0);
    vm_map_delete = (void*)(kbase + 0x2FEFA0);
    vm_map_protect = (void*)(kbase + 0x3012F0);
    vm_map_findspace = (void*)(kbase + 0x2FF560);
    vm_map_insert = (void*)(kbase + 0x2FD640);
    vm_map_lock = (void*)(kbase + 0x2FC2E0);
    vm_map_unlock = (void*)(kbase + 0x2FC350);
    proc_rwmem = (void*)(kbase + 0x361310);
    disable_console_output = (void*)(kbase + 0x1564910);
    M_TEMP = (void*)(kbase + 0x1556DA0);
    kernel_map = (void*)(kbase + 0x21405B8);
    prison0 = (void*)(kbase + 0x113B728);
    rootvnode = (void*)(kbase + 0x1B463E0);
    allproc = (void*)(kbase + 0x213C828);
    sysents = (void*)(kbase + 0x1122340);
}

void init_80Xsdk(uint8_t* kbase) {
    Xfast_syscall = (void*)(kbase + 0x1C0);
    copyin = (void*)(kbase + 0x25E3B0);
    copyout = (void*)(kbase + 0x25E2C0);
    printf = (void*)(kbase + 0x430AE0);
    vprintf = (void*)(kbase + 0x430B50);
    malloc = (void*)(kbase + 0x46F7F0);
    free = (void*)(kbase + 0x46F9B0);
    memcpy = (void*)(kbase + 0x25E1C0);
    memset = (void*)(kbase + 0xF6C60);
    memcmp = (void*)(kbase + 0x195A90);
    kmem_alloc = (void*)(kbase + 0x1B3F0);
    strlen = (void*)(kbase + 0x2F6090);
    strcpy = (void*)(kbase + 0x1E0870);
    strncmp = (void*)(kbase + 0x1846D0);
    pause = (void*)(kbase + 0x3381D0);
    kthread_add = (void*)(kbase + 0x46E530);
    kthread_exit = (void*)(kbase + 0x46E820);
    sched_prio = (void*)(kbase + 0x1067D0);
    sched_add = (void*)(kbase + 0x1072D0);
    kern_yield = (void*)(kbase + 0x3384E0);
    fill_regs = (void*)(kbase + 0x4BA00);
    set_regs = (void*)(kbase + 0x4BB30);
    create_thread = (void*)(kbase + 0x26FA50);
    kproc_create = (void*)(kbase + 0x46DFD0);
    kthread_set_affinity = (void*)(kbase + 0x46EE90);
    kthread_suspend_check = (void*)(kbase + 0x46EC30);
    kproc_kthread_add = (void*)(kbase + 0x46ED40);
    sx_init_flags = (void*)(kbase + 0x43A0C0);
    sx_xlock = (void*)(kbase + 0x43A340);
    sx_xunlock = (void*)(kbase + 0x43A500);
    mtx_init = (void*)(kbase + 0x2984A0);
    mtx_lock_spin_flags = (void*)(kbase + 0x297E10);
    mtx_unlock_spin_flags = (void*)(kbase + 0x297FE0);
    mtx_lock_sleep = (void*)(kbase + 0x2979F0);
    mtx_unlock_sleep = (void*)(kbase + 0x297CB0);
    kern_reboot = (void*)(kbase + 0x155560);
    vm_map_lock_read = (void*)(kbase + 0x3E7680);
    vm_map_lookup_entry = (void*)(kbase + 0x3E7CC0);
    vm_map_unlock_read = (void*)(kbase + 0x3E76D0);
    vmspace_acquire_ref = (void*)(kbase + 0x3E74E0);
    vmspace_free = (void*)(kbase + 0x3E7310);
    vm_map_delete = (void*)(kbase + 0x3EA180);
    vm_map_protect = (void*)(kbase + 0x3EC4C0);
    vm_map_findspace = (void*)(kbase + 0x3EA740);
    vm_map_insert = (void*)(kbase + 0x3E8880);
    vm_map_lock = (void*)(kbase + 0x3E7530);
    vm_map_unlock = (void*)(kbase + 0x3E75A0);
    proc_rwmem = (void*)(kbase + 0x173770);
    disable_console_output = (void*)(kbase + 0x155D190);
    M_TEMP = (void*)(kbase + 0x1A77E10);
    kernel_map = (void*)(kbase + 0x1B243E0);
    prison0 = (void*)(kbase + 0x111A7D0);
    rootvnode = (void*)(kbase + 0x1B8C730);
    allproc = (void*)(kbase + 0x1B244E0);
    sysents = (void*)(kbase + 0x10FC4D0);
}

void init_85Xsdk(uint8_t* kbase) {
    Xfast_syscall = (void*)(kbase + 0x1C0);
    copyin = (void*)(kbase + 0x3A42E0);
    copyout = (void*)(kbase + 0x3A41F0);
    printf = (void*)(kbase + 0x15D570);
    vprintf = (void*)(kbase + 0x15D5E0);
    malloc = (void*)(kbase + 0xB5A40);
    free = (void*)(kbase + 0xB5C00);
    memcpy = (void*)(kbase + 0x3A40F0);
    memset = (void*)(kbase + 0x3D6710);
    memcmp = (void*)(kbase + 0x20F280);
    kmem_alloc = (void*)(kbase + 0x2199A0);
    strlen = (void*)(kbase + 0x270C40);
    strcpy = (void*)(kbase + 0x21AD40);
    strncmp = (void*)(kbase + 0x3CF6D0);
    pause = (void*)(kbase + 0x27C590);
    kthread_add = (void*)(kbase + 0x10B70);
    kthread_exit = (void*)(kbase + 0x10E60);
    sched_prio = (void*)(kbase + 0xEE7F0);
    sched_add = (void*)(kbase + 0xEF2F0);
    kern_yield = (void*)(kbase + 0x27C8A0);
    fill_regs = (void*)(kbase + 0x268F70);
    set_regs = (void*)(kbase + 0x2690A0);
    create_thread = (void*)(kbase + 0x392440);
    kproc_create = (void*)(kbase + 0x10610);
    kthread_set_affinity = (void*)(kbase + 0x114D0);
    kthread_suspend_check = (void*)(kbase + 0x11270);
    kproc_kthread_add = (void*)(kbase + 0x11380);
    sx_init_flags = (void*)(kbase + 0x2BAC90);
    sx_xlock = (void*)(kbase + 0x2BAF10);
    sx_xunlock = (void*)(kbase + 0x2BB0D0);
    mtx_init = (void*)(kbase + 0x26FB80);
    mtx_lock_spin_flags = (void*)(kbase + 0x26F4F0);
    mtx_unlock_spin_flags = (void*)(kbase + 0x26F6C0);
    mtx_lock_sleep = (void*)(kbase + 0x26F0D0);
    mtx_unlock_sleep = (void*)(kbase + 0x26F390);
    kern_reboot = (void*)(kbase + 0x40B420);
    vm_map_lock_read = (void*)(kbase + 0x1486D0);
    vm_map_lookup_entry = (void*)(kbase + 0x148D10);
    vm_map_unlock_read = (void*)(kbase + 0x148720);
    vmspace_acquire_ref = (void*)(kbase + 0x148530);
    vmspace_free = (void*)(kbase + 0x148360);
    vm_map_delete = (void*)(kbase + 0x14B1D0);
    vm_map_protect = (void*)(kbase + 0x14D510);
    vm_map_findspace = (void*)(kbase + 0x14B790);
    vm_map_insert = (void*)(kbase + 0x1498D0);
    vm_map_lock = (void*)(kbase + 0x148580);
    vm_map_unlock = (void*)(kbase + 0x1485F0);
    proc_rwmem = (void*)(kbase + 0x131B50);
    disable_console_output = (void*)(kbase + 0x153AE88);
    M_TEMP = (void*)(kbase + 0x1528FF0);
    kernel_map = (void*)(kbase + 0x1C64228);
    prison0 = (void*)(kbase + 0x111A8F0);
    rootvnode = (void*)(kbase + 0x1C66150);
    allproc = (void*)(kbase + 0x1BD72D8);
    sysents = (void*)(kbase + 0x10FC5C0);
}

void init_900sdk(uint8_t* kbase) {
    Xfast_syscall = (void*)(kbase + 0x1C0);
    copyin = (void*)(kbase + 0x2716A0);
    copyout = (void*)(kbase + 0x2715B0);
    printf = (void*)(kbase + 0xB7A30);
    vprintf = (void*)(kbase + 0xB7AA0);
    malloc = (void*)(kbase + 0x301B20);
    free = (void*)(kbase + 0x301CE0);
    memcpy = (void*)(kbase + 0x2714B0);
    memset = (void*)(kbase + 0x1496C0);
    memcmp = (void*)(kbase + 0x271E20);
    kmem_alloc = (void*)(kbase + 0x37BE70);
    strlen = (void*)(kbase + 0x30F450);
    strcpy = (void*)(kbase + 0x189F80);
    strncmp = (void*)(kbase + 0x124750);
    pause = (void*)(kbase + 0x453EA0);
    kthread_add = (void*)(kbase + 0x96F40);
    kthread_exit = (void*)(kbase + 0x97230);
    sched_prio = (void*)(kbase + 0x1CBB90);
    sched_add = (void*)(kbase + 0x1CC690);
    kern_yield = (void*)(kbase + 0x4541B0);
    fill_regs = (void*)(kbase + 0x314870);
    set_regs = (void*)(kbase + 0x3149A0);
    create_thread = (void*)(kbase + 0x1ED670);
    kproc_create = (void*)(kbase + 0x969E0);
    kthread_set_affinity = (void*)(kbase + 0x978A0);
    kthread_suspend_check = (void*)(kbase + 0x97640);
    kproc_kthread_add = (void*)(kbase + 0x97750);
    sx_init_flags = (void*)(kbase + 0x43E390);
    sx_xlock = (void*)(kbase + 0x43E610);
    sx_xunlock = (void*)(kbase + 0x43E7D0);
    mtx_init = (void*)(kbase + 0x2EF960);
    mtx_lock_spin_flags = (void*)(kbase + 0x2EF2D0);
    mtx_unlock_spin_flags = (void*)(kbase + 0x2EF4A0);
    mtx_lock_sleep = (void*)(kbase + 0x2EEEB0);
    mtx_unlock_sleep = (void*)(kbase + 0x2EF170);
    kern_reboot = (void*)(kbase + 0x29A380);
    vm_map_lock_read = (void*)(kbase + 0x7BB80);
    vm_map_lookup_entry = (void*)(kbase + 0x7C1C0);
    vm_map_unlock_read = (void*)(kbase + 0x7BBD0);
    vmspace_acquire_ref = (void*)(kbase + 0x7B9E0);
    vmspace_free = (void*)(kbase + 0x7B810);
    vm_map_delete = (void*)(kbase + 0x7E680);
    vm_map_protect = (void*)(kbase + 0x809C0);
    vm_map_findspace = (void*)(kbase + 0x7EC40);
    vm_map_insert = (void*)(kbase + 0x7CD80);
    vm_map_lock = (void*)(kbase + 0x7BA30);
    vm_map_unlock = (void*)(kbase + 0x7BAA0);
    proc_rwmem = (void*)(kbase + 0x41EB00);
    disable_console_output = (void*)(kbase + 0x152BF60);
    M_TEMP = (void*)(kbase + 0x15621E0);
    kernel_map = (void*)(kbase + 0x2268D48);
    prison0 = (void*)(kbase + 0x111F870);
    rootvnode = (void*)(kbase + 0x21EFF20);
    allproc = (void*)(kbase + 0x1B946E0);
    sysents = (void*)(kbase + 0x1100310);
}

void init_90Xsdk(uint8_t* kbase) {
    Xfast_syscall = (void*)(kbase + 0x1C0);
    copyin = (void*)(kbase + 0x271320);
    copyout = (void*)(kbase + 0x271230);
    printf = (void*)(kbase + 0xB79E0);
    vprintf = (void*)(kbase + 0xB7A50);
    malloc = (void*)(kbase + 0x3017B0);
    free = (void*)(kbase + 0x301970);
    memcpy = (void*)(kbase + 0x271130);
    memset = (void*)(kbase + 0x149670);
    memcmp = (void*)(kbase + 0x271AA0);
    kmem_alloc = (void*)(kbase + 0x37A070);
    strlen = (void*)(kbase + 0x30F0F0);
    strcpy = (void*)(kbase + 0x189F30);
    strncmp = (void*)(kbase + 0x124700);
    pause = (void*)(kbase + 0x451DA0);
    kthread_add = (void*)(kbase + 0x96F40);
    kthread_exit = (void*)(kbase + 0x97230);
    sched_prio = (void*)(kbase + 0x1CBB40);
    sched_add = (void*)(kbase + 0x1CC640);
    kern_yield = (void*)(kbase + 0x4520B0);
    fill_regs = (void*)(kbase + 0x314510);
    set_regs = (void*)(kbase + 0x314640);
    create_thread = (void*)(kbase + 0x1ED620);
    kproc_create = (void*)(kbase + 0x969E0);
    kthread_set_affinity = (void*)(kbase + 0x978A0);
    kthread_suspend_check = (void*)(kbase + 0x97640);
    kproc_kthread_add = (void*)(kbase + 0x97750);
    sx_init_flags = (void*)(kbase + 0x43C2B0);
    sx_xlock = (void*)(kbase + 0x43C530);
    sx_xunlock = (void*)(kbase + 0x43C6F0);
    mtx_init = (void*)(kbase + 0x2EF5E0);
    mtx_lock_spin_flags = (void*)(kbase + 0x2EEF50);
    mtx_unlock_spin_flags = (void*)(kbase + 0x2EF120);
    mtx_lock_sleep = (void*)(kbase + 0x2EEB30);
    mtx_unlock_sleep = (void*)(kbase + 0x2EEDF0);
    kern_reboot = (void*)(kbase + 0x29A000);
    vm_map_lock_read = (void*)(kbase + 0x7BB80);
    vm_map_lookup_entry = (void*)(kbase + 0x7C1C0);
    vm_map_unlock_read = (void*)(kbase + 0x7BBD0);
    vmspace_acquire_ref = (void*)(kbase + 0x7B9E0);
    vmspace_free = (void*)(kbase + 0x7B810);
    vm_map_delete = (void*)(kbase + 0x7E680);
    vm_map_protect = (void*)(kbase + 0x809C0);
    vm_map_findspace = (void*)(kbase + 0x7EC40);
    vm_map_insert = (void*)(kbase + 0x7CD80);
    vm_map_lock = (void*)(kbase + 0x7BA30);
    vm_map_unlock = (void*)(kbase + 0x7BAA0);
    proc_rwmem = (void*)(kbase + 0x41CA70);
    disable_console_output = (void*)(kbase + 0x1527F60);
    M_TEMP = (void*)(kbase + 0x155E1E0);
    kernel_map = (void*)(kbase + 0x2264D48);
    prison0 = (void*)(kbase + 0x111B840);
    rootvnode = (void*)(kbase + 0x21EBF20);
    allproc = (void*)(kbase + 0x1B906E0);
    sysents = (void*)(kbase + 0x10FC310);
}

void init_9XXsdk(uint8_t* kbase) {
    Xfast_syscall = (void*)(kbase + 0x1C0);
    copyin = (void*)(kbase + 0x201EB0);
    copyout = (void*)(kbase + 0x201DC0);
    printf = (void*)(kbase + 0x205470);
    vprintf = (void*)(kbase + 0x2054E0);
    malloc = (void*)(kbase + 0x29D330);
    free = (void*)(kbase + 0x29D4F0);
    memcpy = (void*)(kbase + 0x201CC0);
    memset = (void*)(kbase + 0xC1720);
    memcmp = (void*)(kbase + 0x47CB80);
    kmem_alloc = (void*)(kbase + 0x1889D0);
    strlen = (void*)(kbase + 0x3F1980);
    strcpy = (void*)(kbase + 0x21F500);
    strncmp = (void*)(kbase + 0x1360B0);
    pause = (void*)(kbase + 0x62120);
    kthread_add = (void*)(kbase + 0x455390);
    kthread_exit = (void*)(kbase + 0x455680);
    sched_prio = (void*)(kbase + 0x456710);
    sched_add = (void*)(kbase + 0x457210);
    kern_yield = (void*)(kbase + 0x62430);
    fill_regs = (void*)(kbase + 0x34EF50);
    set_regs = (void*)(kbase + 0x34F080);
    create_thread = (void*)(kbase + 0x1EC430);
    kproc_create = (void*)(kbase + 0x454E30);
    kthread_set_affinity = (void*)(kbase + 0x455CF0);
    kthread_suspend_check = (void*)(kbase + 0x455A90);
    kproc_kthread_add = (void*)(kbase + 0x455BA0);
    sx_init_flags = (void*)(kbase + 0x42B8C0);
    sx_xlock = (void*)(kbase + 0x42BB40);
    sx_xunlock = (void*)(kbase + 0x42BD00);
    mtx_init = (void*)(kbase + 0x38C460);
    mtx_lock_spin_flags = (void*)(kbase + 0x38BE00);
    mtx_unlock_spin_flags = (void*)(kbase + 0x38BFA0);
    mtx_lock_sleep = (void*)(kbase + 0x38B9F0);
    mtx_unlock_sleep = (void*)(kbase + 0x38BCA0);
    kern_reboot = (void*)(kbase + 0x331DF0);
    vm_map_lock_read = (void*)(kbase + 0x191D30);
    vm_map_lookup_entry = (void*)(kbase + 0x192370);
    vm_map_unlock_read = (void*)(kbase + 0x191D80);
    vmspace_acquire_ref = (void*)(kbase + 0x191BA0);
    vmspace_free = (void*)(kbase + 0x1919D0);
    vm_map_delete = (void*)(kbase + 0x194830);
    vm_map_protect = (void*)(kbase + 0x196B70);
    vm_map_findspace = (void*)(kbase + 0x194DF0);
    vm_map_insert = (void*)(kbase + 0x192F30);
    vm_map_lock = (void*)(kbase + 0x191BE0);
    vm_map_unlock = (void*)(kbase + 0x191C50);
    proc_rwmem = (void*)(kbase + 0x479620);
    disable_console_output = (void*)(kbase + 0x1A50BE0);
    M_TEMP = (void*)(kbase + 0x1A4ECB0);
    kernel_map = (void*)(kbase + 0x2147830);
    prison0 = (void*)(kbase + 0x11137D0);
    rootvnode = (void*)(kbase + 0x21A6C30);
    allproc = (void*)(kbase + 0x221D2A0);
    sysents = (void*)(kbase + 0x10F92F0);
}

void init_100Xsdk(uint8_t* kbase) {
    Xfast_syscall = (void*)(kbase + 0x1C0);
    copyin = (void*)(kbase + 0x472F10);
    copyout = (void*)(kbase + 0x472E20);
    printf = (void*)(kbase + 0xC50F0);
    vprintf = (void*)(kbase + 0xC5160);
    malloc = (void*)(kbase + 0x109A60);
    free = (void*)(kbase + 0x109C20);
    memcpy = (void*)(kbase + 0x472D20);
    memset = (void*)(kbase + 0x3E6F0);
    memcmp = (void*)(kbase + 0x109940);
    kmem_alloc = (void*)(kbase + 0x33B040);
    strlen = (void*)(kbase + 0x2E0340);
    strcpy = (void*)(kbase + 0x466E30);
    strncmp = (void*)(kbase + 0x1219B0);
    pause = (void*)(kbase + 0x286350);
    kthread_add = (void*)(kbase + 0x2082F0);
    kthread_exit = (void*)(kbase + 0x2085E0);
    sched_prio = (void*)(kbase + 0x1D5D50);
    sched_add = (void*)(kbase + 0x1D6850);
    kern_yield = (void*)(kbase + 0x286660);
    fill_regs = (void*)(kbase + 0x7D090);
    set_regs = (void*)(kbase + 0x7D1C0);
    create_thread = (void*)(kbase + 0x182F0);
    kproc_create = (void*)(kbase + 0x207D90);
    kthread_set_affinity = (void*)(kbase + 0x208C50);
    kthread_suspend_check = (void*)(kbase + 0x2089F0);
    kproc_kthread_add = (void*)(kbase + 0x208B00);
    sx_init_flags = (void*)(kbase + 0xA9800);
    sx_xlock = (void*)(kbase + 0xA9A80);
    sx_xunlock = (void*)(kbase + 0xA9C40);
    mtx_init = (void*)(kbase + 0x1E05C0);
    mtx_lock_spin_flags = (void*)(kbase + 0x1DFF60);
    mtx_unlock_spin_flags = (void*)(kbase + 0x1E0100);
    mtx_lock_sleep = (void*)(kbase + 0x1DFB50);
    mtx_unlock_sleep = (void*)(kbase + 0x1DFE00);
    kern_reboot = (void*)(kbase + 0x480CE0);
    vm_map_lock_read = (void*)(kbase + 0x38D070);
    vm_map_lookup_entry = (void*)(kbase + 0x38D6B0);
    vm_map_unlock_read = (void*)(kbase + 0x38D0C0);
    vmspace_acquire_ref = (void*)(kbase + 0x38CEE0);
    vmspace_free = (void*)(kbase + 0x38CD10);
    vm_map_delete = (void*)(kbase + 0x38FB70);
    vm_map_protect = (void*)(kbase + 0x391EB0);
    vm_map_findspace = (void*)(kbase + 0x390130);
    vm_map_insert = (void*)(kbase + 0x38E270);
    vm_map_lock = (void*)(kbase + 0x38CF20);
    vm_map_unlock = (void*)(kbase + 0x38CF90);
    proc_rwmem = (void*)(kbase + 0x44DC40);
    disable_console_output = (void*)(kbase + 0x1A78A78);
    M_TEMP = (void*)(kbase + 0x1532C00);
    kernel_map = (void*)(kbase + 0x227BEF8);
    prison0 = (void*)(kbase + 0x111B8B0);
    rootvnode = (void*)(kbase + 0x1B25BD0);
    allproc = (void*)(kbase + 0x22D9B40);
    sysents = (void*)(kbase + 0x1102D90);
}

void init_10XXsdk(uint8_t* kbase) {
    Xfast_syscall = (void*)(kbase + 0x1C0);
    copyin = (void*)(kbase + 0xD7560);
    copyout = (void*)(kbase + 0xD7470);
    printf = (void*)(kbase + 0x450E80);
    vprintf = (void*)(kbase + 0x450EF0);
    malloc = (void*)(kbase + 0x36E120);
    free = (void*)(kbase + 0x36E2E0);
    memcpy = (void*)(kbase + 0xD7370);
    memset = (void*)(kbase + 0xD090);
    memcmp = (void*)(kbase + 0x2A020);
    kmem_alloc = (void*)(kbase + 0x428960);
    strlen = (void*)(kbase + 0x160DA0);
    strcpy = (void*)(kbase + 0x3F0BA0);
    strncmp = (void*)(kbase + 0x1DDDA0);
    pause = (void*)(kbase + 0x3FD7C0);
    kthread_add = (void*)(kbase + 0x3AB9F0);
    kthread_exit = (void*)(kbase + 0x3ABCE0);
    sched_prio = (void*)(kbase + 0x4432A0);
    sched_add = (void*)(kbase + 0x443DA0);
    kern_yield = (void*)(kbase + 0x3FDAD0);
    fill_regs = (void*)(kbase + 0x1431D0);
    set_regs = (void*)(kbase + 0x143300);
    create_thread = (void*)(kbase + 0x3384E0);
    kproc_create = (void*)(kbase + 0x3AB490);
    kthread_set_affinity = (void*)(kbase + 0x3AC350);
    kthread_suspend_check = (void*)(kbase + 0x3AC0F0);
    kproc_kthread_add = (void*)(kbase + 0x3AC200);
    sx_init_flags = (void*)(kbase + 0x97520);
    sx_xlock = (void*)(kbase + 0x977A0);
    sx_xunlock = (void*)(kbase + 0x97960);
    mtx_init = (void*)(kbase + 0x293CB0);
    mtx_lock_spin_flags = (void*)(kbase + 0x293650);
    mtx_unlock_spin_flags = (void*)(kbase + 0x2937F0);
    mtx_lock_sleep = (void*)(kbase + 0x293240);
    mtx_unlock_sleep = (void*)(kbase + 0x2934F0);
    kern_reboot = (void*)(kbase + 0x45D6D0);
    vm_map_lock_read = (void*)(kbase + 0x4762D0);
    vm_map_lookup_entry = (void*)(kbase + 0x476910);
    vm_map_unlock_read = (void*)(kbase + 0x476320);
    vmspace_acquire_ref = (void*)(kbase + 0x476140);
    vmspace_free = (void*)(kbase + 0x475F70);
    vm_map_delete = (void*)(kbase + 0x478DD0);
    vm_map_protect = (void*)(kbase + 0x47B110);
    vm_map_findspace = (void*)(kbase + 0x479390);
    vm_map_insert = (void*)(kbase + 0x4774D0);
    vm_map_lock = (void*)(kbase + 0x476180);
    vm_map_unlock = (void*)(kbase + 0x4761F0);
    proc_rwmem = (void*)(kbase + 0x4244A0);
    disable_console_output = (void*)(kbase + 0x1A3BCA0);
    M_TEMP = (void*)(kbase + 0x1A5FE30);
    kernel_map = (void*)(kbase + 0x22A9250);
    prison0 = (void*)(kbase + 0x111B910);
    rootvnode = (void*)(kbase + 0x1BF81F0);
    allproc = (void*)(kbase + 0x2269F30);
    sysents = (void*)(kbase + 0x11029C0);
}

void init_1100sdk(uint8_t* kbase) {
    Xfast_syscall = (void*)(kbase + 0x1C0);
    copyin = (void*)(kbase + 0x2DDFE0);
    copyout = (void*)(kbase + 0x2DDEF0);
    printf = (void*)(kbase + 0x2FCBD0);
    vprintf = (void*)(kbase + 0x2FCC40);
    malloc = (void*)(kbase + 0x1A4220);
    free = (void*)(kbase + 0x1A43E0);
    memcpy = (void*)(kbase + 0x2DDDF0);
    memset = (void*)(kbase + 0x482D0);
    memcmp = (void*)(kbase + 0x948B0);
    kmem_alloc = (void*)(kbase + 0x245E10);
    strlen = (void*)(kbase + 0x21DC40);
    strcpy = (void*)(kbase + 0x1AA590);
    strncmp = (void*)(kbase + 0x313B10);
    pause = (void*)(kbase + 0x3663E0);
    kthread_add = (void*)(kbase + 0xC36A0);
    kthread_exit = (void*)(kbase + 0xC3990);
    sched_prio = (void*)(kbase + 0x2BA470);
    sched_add = (void*)(kbase + 0x2BAF70);
    kern_yield = (void*)(kbase + 0x3666F0);
    fill_regs = (void*)(kbase + 0x25DB10);
    set_regs = (void*)(kbase + 0x25DC40);
    create_thread = (void*)(kbase + 0x295170);
    kproc_create = (void*)(kbase + 0xC3140);
    kthread_set_affinity = (void*)(kbase + 0xC4000);
    kthread_suspend_check = (void*)(kbase + 0xC3DA0);
    kproc_kthread_add = (void*)(kbase + 0xC3EB0);
    sx_init_flags = (void*)(kbase + 0xE2F80);
    sx_xlock = (void*)(kbase + 0xE3200);
    sx_xunlock = (void*)(kbase + 0xE33C0);
    mtx_init = (void*)(kbase + 0x10F110);
    mtx_lock_spin_flags = (void*)(kbase + 0x10EAB0);
    mtx_unlock_spin_flags = (void*)(kbase + 0x10EC50);
    mtx_lock_sleep = (void*)(kbase + 0x10E6A0);
    mtx_unlock_sleep = (void*)(kbase + 0x10E950);
    kern_reboot = (void*)(kbase + 0x198060);
    vm_map_lock_read = (void*)(kbase + 0x3578B0);
    vm_map_lookup_entry = (void*)(kbase + 0x357EF0);
    vm_map_unlock_read = (void*)(kbase + 0x357900);
    vmspace_acquire_ref = (void*)(kbase + 0x357720);
    vmspace_free = (void*)(kbase + 0x357550);
    vm_map_delete = (void*)(kbase + 0x35A3B0);
    vm_map_protect = (void*)(kbase + 0x35C710);
    vm_map_findspace = (void*)(kbase + 0x35A970);
    vm_map_insert = (void*)(kbase + 0x358AB0);
    vm_map_lock = (void*)(kbase + 0x357760);
    vm_map_unlock = (void*)(kbase + 0x3577D0);
    proc_rwmem = (void*)(kbase + 0x3838A0);
    disable_console_output = (void*)(kbase + 0x152CFF8);
    M_TEMP = (void*)(kbase + 0x15415B0);
    kernel_map = (void*)(kbase + 0x21FF130);
    prison0 = (void*)(kbase + 0x111F830);
    rootvnode = (void*)(kbase + 0x2116640);
    allproc = (void*)(kbase + 0x22D0A98);
    sysents = (void*)(kbase + 0x1101760);
}

void init_ksdk() {
    uint64_t kbase = get_kbase();
    cachedKernelBase = kbase;
    unsigned short firmwareVersion = kget_firmware_from_base(kbase);
    switch (firmwareVersion) {
    case 505:
        init_505sdk((uint8_t*)kbase);
        break;
    case 672:
        init_672sdk((uint8_t*)kbase);
        break;
    case 700:
        init_70Xsdk((uint8_t*)kbase);
        break;
    case 701:
        init_70Xsdk((uint8_t*)kbase);
        break;
    case 702:
        init_70Xsdk((uint8_t*)kbase);
        break;
    case 750:
        init_75Xsdk((uint8_t*)kbase);
        break;
    case 751:
        init_75Xsdk((uint8_t*)kbase);
        break;
    case 755:
        init_75Xsdk((uint8_t*)kbase);
        break;
    case 800:
        init_80Xsdk((uint8_t*)kbase);
        break;
    case 801:
        init_80Xsdk((uint8_t*)kbase);
        break;
    case 803:
        init_80Xsdk((uint8_t*)kbase);
        break;
    case 850:
        init_85Xsdk((uint8_t*)kbase);
        break;
    case 852:
        init_85Xsdk((uint8_t*)kbase);
        break;
    case 900:
        init_900sdk((uint8_t*)kbase);
        break;
    case 903:
        init_90Xsdk((uint8_t*)kbase);
        break;
    case 904:
        init_90Xsdk((uint8_t*)kbase);
        break;
    case 950:
        init_9XXsdk((uint8_t*)kbase);
        break;
    case 951:
        init_9XXsdk((uint8_t*)kbase);
        break;
    case 960:
        init_9XXsdk((uint8_t*)kbase);
        break;
    case 1000:
        init_100Xsdk((uint8_t*)kbase);
        break;
    case 1001:
        init_100Xsdk((uint8_t*)kbase);
        break;
    case 1050:
        init_10XXsdk((uint8_t*)kbase);
        break;
    case 1070:
        init_10XXsdk((uint8_t*)kbase);
        break;
    case 1071:
        init_10XXsdk((uint8_t*)kbase);
        break;
    case 1100:
        init_1100sdk((uint8_t*)kbase);
        break;
    }
}
