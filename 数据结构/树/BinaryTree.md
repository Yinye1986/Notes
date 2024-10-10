# 本质
一种树形数据结构

# 基本术语

- 根节点（Root Node）：二叉树的顶层节点。
- 叶子节点（Leaf Node）：没有子节点的节点。
- 内部节点（Internal Node）：至少有一个子节点的节点。
- 子树（Subtree）：一个节点及其所有后代构成的树。
- 树的高度（Height of Tree）：树中节点的最大深度。
- 节点的深度（Depth of Node）：从根节点到该节点所经历的边的数量。

# 二叉树类型

- 满二叉树（Full Binary Tree）：每个节点要么没有子节点，要么有两个子节点。
- 完全二叉树（Complete Binary Tree）：每一层除了最后一层外，都是满的，最后一层的节点都尽量向左对齐。
- 平衡二叉树（Balanced Binary Tree）：任何节点的左右子树的高度差不超过1。
- 二叉搜索树（Binary Search Tree, BST）：左子节点的值小于父节点的值，右子节点的值大于父节点的值。

# 二叉树的基本方法

- 插入（Insertion）：在二叉树中添加一个新节点。
- 删除（Deletion）：从二叉树中移除一个节点。
- 查找（Search）：在二叉树中查找一个特定的值。
- 遍历（Traversal）：访问二叉树的所有节点。有几种遍历方式：
    - 前序遍历（Preorder Traversal）：先访问根节点，然后访问左子树，最后访问右子树。
    - 中序遍历（Inorder Traversal）：先访问左子树，然后访问根节点，最后访问右子树。
    - 后序遍历（Postorder Traversal）：先访问左子树，然后访问右子树，最后访问根节点。
    - 层序遍历（Level Order Traversal）：按层次逐层访问节点，从上到下，从左到右。
