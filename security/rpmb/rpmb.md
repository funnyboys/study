# 1. RPMB 的背景
## 1.1. 什么是安全存储
OP-TEE 安全存储接口由 GlobalPlatform 在 TEE Internal Core API中定义，是 OP-TEE 必须实现的接口。
安全存储要求保证存储数据的完整性和原子性。

## 1.2. 安全存储的实现方式
目前安全存储有两种实现方式：
1. 将文件加密后存放在REE文件系统；
2. 利用 eMMC 的 Replay Protected Memory Block(RPMB) 分区来实现。
    * RPMB 标准可以参考 JEDEC eMMC spec(JESD84-B51)

# 2. RPMB 介绍
RPMB（Replay Protected Memory Block）Partition 是 eMMC 中的一个具有安全特性的分区。
eMMC 在写入数据到 RPMB 时，会校验数据的合法性，只有指定的 Host 才能够写入，同时在读数据时，也提供了签名机制，保证 Host 读取到的数据是 RPMB 内部数据，而不是攻击者伪造的数据。

RPMB 在实际应用中，通常用于存储一些有防止非法篡改需求的数据，例如手机上指纹支付相关的公钥、序列号等。RPMB 可以对写入操作进行鉴权，但是读取并不需要鉴权，任何人都可以进行读取的操作，因此存储到 RPMB 的数据通常会进行加密后再存储。

# 3. RPMB 原理
使用 eMMC 的产品，在产线生产时，会为每一个产品生产一个唯一的 256 bits 的 Secure Key，烧写到 eMMC 的 OTP 区域（只能烧写一次的区域），同时 Host 在安全区域中（例如：TEE）也会保留该 Secure Key。

在 eMMC 内部，还有一个RPMB Write Counter。RPMB 每进行一次合法的写入操作时，Write Counter 就会自动加一 。

通过 Secure Key 和 Write Counter 的应用，RMPB 可以实现数据读取和写入的 Replay Protect。

# 4. RPMB 读写流程
## 4.1. RPMB 数据读取
RPMB 数据读取的流程如下：
1. Host 向 eMMC 发起读 RPMB 的请求，同时生成一个 16 bytes 的随机数，发送给 eMMC。
2. eMMC 将请求的数据从 RPMB 中读出，并使用 Secure Key 通过 HMAC SHA-256 算法，计算读取到的数据和接收到的随机数拼接到一起后的签名。然后，eMMC 将读取到的数据、接收到的随机数、计算得到的签名一并发送给 Host。
3. Host 接收到 RPMB 的数据、随机数以及签名后，首先比较随机数是否与自己发送的一致，如果一致，再用同样的 Secure Key 通过 HMAC SHA-256 算法对数据和随机数组合到一起进行签名，如果签名与 eMMC 发送的签名是一致的，那么就可以确定该数据是从 RPMB 中读取到的正确数据，而不是攻击者伪造的数据。
4. 通过上述的读取流程，可以保证 Host 正确的读取到 RPMB 的数据。

## 4.2. RPMB 数据写入
RPMB 数据写入的流程如下：
1. Host 按照上面的读数据流程，读取 RPMB 的 Write Counter(在eMMC中保存)。
2. Host 将需要写入的数据和 Write Counter 拼接到一起并计算签名，然后将数据、Write Counter 以及签名一并发给 eMMC。
3. eMMC 接收到数据后，先对比 Write Counter 是否与当前的值相同，如果相同那么再对数据和 Write Counter 的组合进行签名，然后和 Host 发送过来的签名进行比较，如果签名相同则鉴权通过，将数据写入到 RPMB 中。
4. 通过上述的写入流程，可以保证 RPMB 不会被非法篡改。

# 5. RPMB 软件框架
```
|          NORMAL WORLD           :            SECURE WORLD              |
                                  :
U        tee-supplicant           :        Trusted application
S           (rpmb.c)              :        (secure storage API)
E         ^          ^            :                  ^
R         |          |            :                  |
~~~~~~~ ioctl ~~~~~~~|~~~~~~~~~~~~:~~~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~~~~~~
K         |          |            :               OP-TEE
E         v          v            :         (tee_svc_storage.c)
R  MMC/SD subsys.  OP-TEE driver  : (tee_rpmb_fs.c, tee_fs_key_manager.c)
N         ^                 ^     :                  ^
E         |                 |     :                  |
L         v                 |     :                  |
    Controller driver       |     :                  |
~~~~~~~~~~~~~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~~~~~~
                            v                        v
                          Secure monitor / EL3 firmware
```

# 6. OP-TEE 中代码分析
## 6.1. RPMB读写文件结构体
```
const struct tee_file_operations rpmb_fs_ops = {
	.open = rpmb_fs_open,
	.create = rpmb_fs_create,
	.close = rpmb_fs_close,
	.read = rpmb_fs_read,
	.write = rpmb_fs_write,
	.truncate = rpmb_fs_truncate,
	.rename = rpmb_fs_rename,
	.remove = rpmb_fs_remove,
	.opendir = rpmb_fs_opendir,
	.closedir = rpmb_fs_closedir,
	.readdir = rpmb_fs_readdir,
};
```

## 6.2. RPMB 读写代码流程
1. tee_rpmb_init()
    * 设备信息(dev_info_synced) 是否已经读取，如果没有则调用 tee_rpmb_get_dev_info() 初始化
        *  调用 tee_rpmb_alloc() 分配内存
        *  req->cmd = RPMB_CMD_GET_DEV_INFO
        *  调用 tee_rpmb_invoke() 跳转到 REE 侧读取设备信息
    * secure key(key_derived)  是否已经读取，如果没有则调用 tee_rpmb_key_gen() 读取 secure key
        * secure key 保存在 OTP 中，和eMMC对应的 secure key 相同
    * write counter 是否和 RPMB 同步(wr_cnt_synced)
    * secure key 是否验证通过(key_verified)
        * 调用 tee_rpmb_verify_key_sync_counter() 验证 write counter 和 secure key

2. tee_rpmb_alloc() 为 req 分配内存

3. crypto_rng_read() 调用 rng 生成16字节随机数

4. tee_rpmb_req_pack() 打包请求
    * 将要传输的数据打包到 req
    * 如果是写操作，计算 MAC 填充到 req 中
        * 如果是写操作， eMMC 需要验证 MAC

5. tee_rpmb_invoke() 调用到 REE 进行 RPMB

6. tee_rpmb_resp_unpack_verify()对 RPMB 返回的数据进行解包和验证
    * 检查 msg type 是否匹配
    * 如果是写操作，检查 write counter
        * 返回的 counter 必须等于之前的 counter+1
    * 比较返回的随机数和源随机数是否一致
    * 计算 RPMB auth MAC
    * 比较计算得到的 MAC 和消息中的 MAC，如果一致证明数据无篡改

# 7. 参考
https://blog.csdn.net/nie19940803/article/details/114679616
https://optee.readthedocs.io/en/latest/architecture/secure_storage.html#rpmb-secure-storage
https://wowothink.com/8ca78fd8/