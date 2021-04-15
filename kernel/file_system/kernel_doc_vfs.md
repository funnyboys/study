# directory entry cache(dcache)
dache是对dentry的缓存，通过filename来快速查找dentry。

# inode
dentry结构体包含一个指向inode的指针。
```
struct dentry {
...
    // where the name belongs to, NULL is negative
    struct inode *d_inode;
...
}
```
inode表示一个保存在文件系统中的对象，包括：文件、目录、FIFO等，多个dentry可以指向同一个inode(硬链接)。
vfs通过lookup()查找inode，lookup()函数由保存inode的文件系统具体实现。

# inode operation

# file object
file表示一个进程打开的文件。

# Registering and Mounting a Filesystem
## register
```
#include <linux/fs.h>

extern int register_filesystem(struct file_system_type *);
```
struct file_system_type指针描述了要注册的文件系统，通过 ->mount() 函数完成真正的注册动作。

## struct file_system_type
```
struct file_system_type {
	// 文件系统名，例如ubifs, ext3
	const char *name;
	int fs_flags;
#define FS_REQUIRES_DEV		1 
#define FS_BINARY_MOUNTDATA	2
#define FS_HAS_SUBTYPE		4
#define FS_USERNS_MOUNT		8	/* Can be mounted by userns root */
#define FS_USERNS_DEV_MOUNT	16 /* A userns mount does not imply MNT_NODEV */
#define FS_USERNS_VISIBLE	32	/* FS must already be visible */
#define FS_RENAME_DOES_D_MOVE	32768	/* FS will handle d_move() during rename() internally. */
	/*
	 * function：加载文件系统时，要调用的mount()函数
	 *
	 * arg：
	 * struct file_system_type *fs_type:
	 * int flags: mount flags
	 * const char *dev_name：要mount的设备名
	 * void *data：mount时传递的参数
	 *
	 * return value：
	 * success：mount后的根dentry
	 * fail：ERR_PTR
	 */
	struct dentry *(*mount) (struct file_system_type *, int,
		       const char *, void *);
	// 关闭文件系统实例
	void (*kill_sb) (struct super_block *);
	struct module *owner;
	struct file_system_type * next;
	struct hlist_head fs_supers;

	struct lock_class_key s_lock_key;
	struct lock_class_key s_umount_key;
	struct lock_class_key s_vfs_rename_key;
	struct lock_class_key s_writers_key[SB_FREEZE_LEVELS];

	struct lock_class_key i_lock_key;
	struct lock_class_key i_mutex_key;
	struct lock_class_key i_mutex_dir_key;
};
```

# superblock
superblock代表一个已经moount的文件系统。

# superblock operation(super_operations)
super_operations描述了VFS如何通过superblock管理文件系统。
这些函数在调用期间，除非特别说明，都不持有锁。
```
struct super_operations {
	// 为struct inode分配内存并初始化
   	struct inode *(*alloc_inode)(struct super_block *sb);
	// 释放为struct inode分配的内存
	void (*destroy_inode)(struct inode *);
	// mark inode dirty
   	void (*dirty_inode) (struct inode *, int flags);
	// 将inode数据写回disk
	int (*write_inode) (struct inode *, struct writeback_control *wbc);
	int (*drop_inode) (struct inode *);
	void (*evict_inode) (struct inode *);
	
	// vfs释放superblock时调用，必须持有superblock锁
	void (*put_super) (struct super_block *);
	// 将superblock中所有的脏数据写回disk
	int (*sync_fs)(struct super_block *sb, int wait);
	// vfs将文件系统锁定
	int (*freeze_super) (struct super_block *);
	int (*freeze_fs) (struct super_block *);
	int (*thaw_super) (struct super_block *);
	int (*unfreeze_fs) (struct super_block *);
	int (*statfs) (struct dentry *, struct kstatfs *);
	int (*remount_fs) (struct super_block *, int *, char *);
	void (*umount_begin) (struct super_block *);

	int (*show_options)(struct seq_file *, struct dentry *);
	int (*show_devname)(struct seq_file *, struct dentry *);
	int (*show_path)(struct seq_file *, struct dentry *);
	int (*show_stats)(struct seq_file *, struct dentry *);
#ifdef CONFIG_QUOTA
	ssize_t (*quota_read)(struct super_block *, int, char *, size_t, loff_t);
	ssize_t (*quota_write)(struct super_block *, int, const char *, size_t, loff_t);
	struct dquot **(*get_dquots)(struct inode *);
#endif
	int (*bdev_try_to_free_page)(struct super_block*, struct page*, gfp_t);
	long (*nr_cached_objects)(struct super_block *,
				  struct shrink_control *);
	long (*free_cached_objects)(struct super_block *,
				    struct shrink_control *);
};
```