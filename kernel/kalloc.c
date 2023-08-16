// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

uint refcount[PHYSTOP/PGSIZE];


struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  struct run *freelist;
} kmem;

void
kinit()
{
	// Initialize refcount with 1
	for (int i = 0; i < PHYSTOP/PGSIZE; i++) {
		refcount[i] = 1;
	}
  initlock(&kmem.lock, "kmem");
  freerange(end, (void*)PHYSTOP);
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
	{
	//	refcount[PA2INDEX(p)] = 1;
    kfree(p);
	}
}

// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
  struct run *r;
	
	if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP) 
		panic("kfree");

	acquire(&kmem.lock);
	int ref =	--refcount[PA2INDEX(pa)];
	release(&kmem.lock);

	if (ref == 0)
	{
		// Fill with junk to catch dangling refs.
		memset(pa, 1, PGSIZE);
		r = (struct run*)pa;
		acquire(&kmem.lock);
		r->next = kmem.freelist;
		kmem.freelist = r;
		release(&kmem.lock);
	}
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
  struct run *r;
	
  acquire(&kmem.lock);
  r = kmem.freelist;
  if(r){
    kmem.freelist = r->next;
		refcount[PA2INDEX(r)] = 1;
	}
  release(&kmem.lock);

  if(r) {
    memset((char*)r, 5, PGSIZE); // fill with junk
	}
  return (void*)r;
}

// lab5-COW fork
// increment reference count
void increfcount(uint64 pa){
	acquire(&kmem.lock);
	refcount[PA2INDEX(pa)]++;
	release(&kmem.lock);
}
