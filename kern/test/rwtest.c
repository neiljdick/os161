/*
 * All the contents of this file are overwritten during automated
 * testing. Please consider this before changing anything in this file.
 */

#include <types.h>
#include <lib.h>
#include <clock.h>
#include <thread.h>
#include <synch.h>
#include <test.h>
#include <kern/test161.h>
#include <spinlock.h>

/*
 * Use these stubs to test your reader-writer locks.
 */

#define CREATELOOPS 10
#define NTHREADS 5
#define LOOPS 1000

static struct rwlock *testlock;
static struct cv *sync;
static struct lock *sync_lock;
static int test_status = TEST161_FAIL;
static volatile int prot_val = 0;
volatile int done = 0;

static void
rwtestthread(void *junk, unsigned long i)
{
	(void)junk;
	(void)i;
	int local = 0;
	done++;
	lock_acquire(sync_lock);
	cv_broadcast(sync, sync_lock);
	lock_release(sync_lock);
	for (i = 0; i < LOOPS; i++) {
		random_yielder(5);
		rwlock_acquire_read(testlock);
		local = prot_val;
		kprintf("read %d\n", local);
		rwlock_release_read(testlock);
	}
	lock_acquire(sync_lock);
	done--;
	cv_broadcast(sync, sync_lock);
	lock_release(sync_lock);
}



int rwtest(int nargs, char **args)
{
	(void)nargs;
	(void)args;

	int i, result;
	kprintf_n("Starting rwt1...\n");

	sync = cv_create("sync");
	KASSERT(sync);
	sync_lock = lock_create("sync_lock");
	KASSERT(sync);

	for (i=0; i<CREATELOOPS; i++) {
		kprintf_t(".");
		testlock = rwlock_create("testlock");
		if (testlock == NULL) {
			panic("rwlock1: rwlock_create failed\n");
		}
		if (i != CREATELOOPS - 1) {
			rwlock_destroy(testlock);
		}
	}

	lock_acquire(sync_lock);
	for (i=0; i<NTHREADS; i++) {
		kprintf_t(".");
		result = thread_fork("rwtest", NULL, rwtestthread, NULL, i);
		if (result) {
			panic("rw1: thread_fork failed: %s\n",
			      strerror(result));
		}
	}
	cv_wait(sync, sync_lock);
	lock_release(sync_lock);
	for (int i = 0; i < LOOPS; i++) {
		random_yielder(100);
		rwlock_acquire_write(testlock);
		prot_val = i;
		rwlock_release_write(testlock);
	}
	kprintf_n("Writer threads finished\n");

	lock_acquire(sync_lock);
	while (done != 0)
		cv_wait(sync, sync_lock);
	lock_release(sync_lock);

	rwlock_destroy(testlock);
	testlock = NULL;
	if (prot_val == LOOPS-1)
		test_status = TEST161_SUCCESS;
	kprintf("Final value %d\n", prot_val);
	success(test_status, SECRET, "rwt1");

	return 0;
}

int rwtest2(int nargs, char **args) {
	(void)nargs;
	(void)args;

	kprintf_n("rwt2 unimplemented\n");
	success(TEST161_FAIL, SECRET, "rwt2");

	return 0;
}

int rwtest3(int nargs, char **args) {
	(void)nargs;
	(void)args;

	kprintf_n("rwt3 unimplemented\n");
	success(TEST161_FAIL, SECRET, "rwt3");

	return 0;
}

int rwtest4(int nargs, char **args) {
	(void)nargs;
	(void)args;

	kprintf_n("rwt4 unimplemented\n");
	success(TEST161_FAIL, SECRET, "rwt4");

	return 0;
}

int rwtest5(int nargs, char **args) {
	(void)nargs;
	(void)args;

	kprintf_n("rwt5 unimplemented\n");
	success(TEST161_FAIL, SECRET, "rwt5");

	return 0;
}
