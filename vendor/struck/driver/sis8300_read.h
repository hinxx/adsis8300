/*
 * SIS8300: read calls header
 */

#ifndef SIS8300_READ_H_
#define SIS8300_READ_H_

ssize_t sis8300_read(struct file *filp, char __user *buff, size_t count, loff_t *offp);

#endif
