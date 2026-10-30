> 本笔记已迁移至：[Git 工具教程](YSYX%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/Linux/Git%E5%B7%A5%E5%85%B7%E6%95%99%E7%A8%8B.md)。以下保留迁移前内容。

下面是使用 git 进行项目开发的规范.
## 1 勤提交
原则:
- 每个 commit 只做一件事(如修复一个Bug, 添加一个功能)
- 避免大杂烩式提交(如 "fix bugs and add features")

下面是错误示范❌
```
git add .
git commit -m "更新用户系统和修复登录bug" # 两个不相关的修改混在一起
```

下面是正确示范✅
```
# 提交1: 用户系统更新
git add user-service.js
git commit -m "feat: 添加用户角色权限系统"

# 提交2: 登录 bug 修复
git add auth-fix.js
git commit -m "fix: 解决登录态丢失问题"
```
## 2 提交信息规范
下面是标准格式:
```
<类型>: <简短描述>

[可选正文]

[可选脚注]
```

下表中是常用类型:

| 类型       | 用途            | 示例                   |
| -------- | ------------- | -------------------- |
| feat     | 新功能           | feat: 添加 PDF 导出功能    |
| fix      | Bug 修复        | fix: 修复首页加载失败        |
| docs     | 文档更新          | docs: 更新 API 接口说明    |
| style    | 代码样式调整(不影响逻辑) | style: 格式化用户模型代码     |
| refactor | 代码重构(非功能修改)   | refactor: 优化订单查询 SQL |
| test     | 测试相关          | test: 增加登录单元测试       |
| chore    | 构建/工具链改动      | chore: 更新 webpack 配置 |
下面是一个优秀实例:
```
git commit -m "feat: 实现购物车批量删除功能

- 添加多选操作 UI 组件
- 新增批量删除 API 接口

Closes #123" # 关联 issue
```
## 3 分支管理策略
下表是分支命名规范:

| 分支类型   | 命名格式        | 示例                   |
| ------ | ----------- | -------------------- |
| 功能开发   | feature/功能名 | feature/user-auth    |
| Bug 修复 | fix/问题描述    | fix/login-error      |
| 发布版本   | release/版本号 | release/v1.2.0       |
| 个人开发   | dev/姓名/功能   | dev/alice/search-opt |

下面是操作示例:
```
# 创建功能分支
git checkout -b feature/payment-integration

# 开发完成后合并到主分支
git checkout main
git merge --no-ff feature/payment-integration # 保留分支历史
git branch -d feature/payment-integration # 删除本地分支
```

## 4 交互式暂存 (精确控制)
当文件包含多个逻辑修改, 但只想提交部分时就可以用到交互式暂存了:
```
git add -p 文件名
```

```
# Git 会显示如下修改, 询问是否暂存:
Stage this hunk [y,n,q,a,d,s,e,?]?
```

| 选项  | 动作            |
| --- | ------------- |
| y   | 暂存当前代码块       |
| n   | 不暂存           |
| s   | 拆分更小的代码块      |
| e   | 手动编辑选择哪些行需要暂存 |

