# 路径名查找
## book
《深入理解Linux内核》 -> 12.虚拟文件系统 -> 路径名查找

## path_lookup
1. path_lookup() -> kern_path_parent
```
commit c9c6cac0c2bdbda42e7b804838648d0bc60ddb13
Author: Al Viro <viro@zeniv.linux.org.uk>
Date:   Wed Feb 16 15:15:47 2011 -0500

    kill path_lookup()
    
    all remaining callers pass LOOKUP_PARENT to it, so
    flags argument can die; renamed to kern_path_parent()
    
    Signed-off-by: Al Viro <viro@zeniv.linux.org.uk>

diff --git a/fs/namei.c b/fs/namei.c
index a4689eb2df28..1d6bc8151553 100644
--- a/fs/namei.c
+++ b/fs/namei.c
@@ -1747,10 +1747,9 @@ static int do_path_lookup(int dfd, const char *name,
 	return retval;
 }
 
-int path_lookup(const char *name, unsigned int flags,
-			struct nameidata *nd)
+int kern_path_parent(const char *name, struct nameidata *nd)
 {
-	return do_path_lookup(AT_FDCWD, name, flags, nd);
+	return do_path_lookup(AT_FDCWD, name, LOOKUP_PARENT, nd);
 }
 
 int kern_path(const char *name, unsigned int flags, struct path *path)
@@ -3586,7 +3585,7 @@ EXPORT_SYMBOL(page_readlink);
 EXPORT_SYMBOL(__page_symlink);
 EXPORT_SYMBOL(page_symlink);
 EXPORT_SYMBOL(page_symlink_inode_operations);
-EXPORT_SYMBOL(path_lookup);
+EXPORT_SYMBOL(kern_path_parent);
 EXPORT_SYMBOL(kern_path);
 EXPORT_SYMBOL(vfs_path_lookup);
 EXPORT_SYMBOL(inode_permission);
```

2. kern_path_parent -> kern_path_locked
```
commit 79714f72d3b964611997de512cb29198c9f2dbbb
Author: Al Viro <viro@zeniv.linux.org.uk>
Date:   Fri Jun 15 03:01:42 2012 +0400

    get rid of kern_path_parent()
    
    all callers want the same thing, actually - a kinda-sorta analog of
    kern_path_create().  I.e. they want parent vfsmount/dentry (with
    ->i_mutex held, to make sure the child dentry is still their child)
    + the child dentry.
    
    Signed-off-by Al Viro <viro@zeniv.linux.org.uk>

diff --git a/fs/namei.c b/fs/namei.c
index 5abab9176903..6b29a51bef5d 100644
--- a/fs/namei.c
+++ b/fs/namei.c
@@ -1814,9 +1814,27 @@ static int do_path_lookup(int dfd, const char *name,
 	return retval;
 }
 
-int kern_path_parent(const char *name, struct nameidata *nd)
+/* does lookup, returns the object with parent locked */
+struct dentry *kern_path_locked(const char *name, struct path *path)
 {
-	return do_path_lookup(AT_FDCWD, name, LOOKUP_PARENT, nd);
+	struct nameidata nd;
+	struct dentry *d;
+	int err = do_path_lookup(AT_FDCWD, name, LOOKUP_PARENT, &nd);
+	if (err)
+		return ERR_PTR(err);
+	if (nd.last_type != LAST_NORM) {
+		path_put(&nd.path);
+		return ERR_PTR(-EINVAL);
+	}
+	mutex_lock_nested(&nd.path.dentry->d_inode->i_mutex, I_MUTEX_PARENT);
+	d = lookup_one_len(nd.last.name, nd.path.dentry, nd.last.len);
+	if (IS_ERR(d)) {
+		mutex_unlock(&nd.path.dentry->d_inode->i_mutex);
+		path_put(&nd.path);
+		return d;
+	}
+	*path = nd.path;
+	return d;
 }
 
 int kern_path(const char *name, unsigned int flags, struct path *path)
 ```