#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/types.h>

struct birthday {
  int day;
  int month;
  int year;
  struct list_head list;
};

static LIST_HEAD(birthday_list);

/* This function is called when the module is loaded. */
int simple_init(void) {
  printk(KERN_INFO "Loading Module\n");

  // Create serveral struct *person(s)
  {
    struct birthday *person_one;
    person_one = kmalloc(sizeof(*person_one), GFP_KERNEL);
    person_one->day = 2;
    person_one->month = 8;
    person_one->year = 1995;
    INIT_LIST_HEAD(&person_one->list);
    list_add_tail(&person_one->list, &birthday_list);

    struct birthday *person_two;
    person_two = kmalloc(sizeof(*person_two), GFP_KERNEL);
    person_two->day = 19;
    person_two->month = 7;
    person_two->year = 2004;
    INIT_LIST_HEAD(&person_two->list);
    list_add_tail(&person_two->list, &birthday_list);
  }
  // print all birthday information
  {
    struct birthday *ptr;
    list_for_each_entry(ptr, &birthday_list, list) {
      printk(KERN_INFO "The person is born at: %d:%d:%d \n", ptr->year,
             ptr->month, ptr->day);
    }
  }

  return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void) {
  printk(KERN_INFO "Removing Module\n");
  // safely delete all elements
  {
    struct birthday *ptr, *next;
    list_for_each_entry_safe(ptr, next, &birthday_list, list) {
      /* on each iteration ptr points */
      /* to the next birthday struct */
      printk(KERN_INFO "Deleting a list ... person is born at: %d:%d:%d \n",
             ptr->year, ptr->month, ptr->day);
      list_del(&ptr->list);
      kfree(ptr);
    }
  }
  printk(KERN_INFO "All Complete!\n");
}

/* Macros for registering module entry and exit points. */
module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");
