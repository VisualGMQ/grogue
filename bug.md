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