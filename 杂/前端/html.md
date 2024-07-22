# HTML5

## 语法

### 起始标签与结束标签

```<p>```这样的被称为**opening tag**，```</p>```这样的被称为**closing tag**

html元素以起始标签开始，以结束标签结束

### 元素的组成和空元素

一个元素，由**起始标签+元素内容+结束标签**组成

空元素：
- 有的元素具有空内容，例如```<br>```换行元素，这样的元素被称为空元素
- 空元素只有开始标签，我们使用```<br/>```来结束空元素

### html常用元素

```html
<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8">
        <title>像素火花</title>
    </head>
    <body>
        <h1>this is a title</h1>
        <p>this is a paragraph</p>
        <hr> <!-- 此dd元素用于创建一条水平线 -->
        <p>这是一个<br>段落</p> <!-- 用<br>来折行 -->
        <a href="pixelspark.cn">这是一个链接使用了 href 属性</a>
        <img src="/images/logo.png" width="258" height="39" />
    <body>
</html>
```
### html元素属性

- 属性一般描述于开始标签
- 属性总是以名称/值对的形式出现，比如：name="value"。

e.g.
```html
<a href="pixelspark.cn">这是一个链接使用了 href 属性</a>
```

解析：在```<a>```标签的开始标签中，为元素内容添加href属性

[html标准属性参考手册](https://www.runoob.com/tags/ref-standardattributes.html )

### 其他元素

[html标签参考手册](https://www.runoob.com/tags/html-reference.html)

