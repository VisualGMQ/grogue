这里记录现有bug和可以优化的地方

## 可优化部分

这一部分指可以优化的性能部分


* ecs的`Querier`的`Query`函数，现在会遍历所有的entities然后对每个entity进行条件查询。这没利用到`SparseSet`的快速访问优势。

## Bug和未来隐患

这一部分指现在能工作，但未来可能会产生隐患的地方，未来一定会修复


* 在产生`Entity`的时候，`SparseSet`的`density`数组会随着Enitty的ID增大而无止境增大。可以考虑对EntityID进行复用。

## 缺陷

这一部分指现在能工作，但不好用的地方，未来可能不会修复

* 要求所有的Component必须可移动（因为底层的ECS实现中，Lambda不能捕获可变右值）
* 使用`LUA_BIND`标记枚举时，必须使用如下代码：
    ```cpp
    LUA_BIND;
    enum YourEnum {
        // ...
    };
    ```
    而不是放在枚举名称前后（因为`CppHeaderParser`库无法解析这种格式的枚举）。
    这意味着你**不能开启自动格式化**，否则`LUA_BIND`宏可能被移动至与`enum`定义一行，同样也会造成解析失败。