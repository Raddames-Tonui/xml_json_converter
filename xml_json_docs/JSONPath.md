# JSONPath Comprehensive Documentation

## 1. Introduction

JSONPath is a query language for JSON, similar to XPath for XML. It allows you to navigate, filter, and extract data from JSON documents using path expressions.

---

## 2. JSONPath Syntax Overview

* `$` : Root object or array
* `@` : Current node being processed
* `.` : Child operator
* `..` : Recursive descent (any level deep)
* `*` : Wildcard (all elements)
* `[index]` : Array index access
* `[start:end:step]` : Array slice
* `[?(expression)]` : Filter expression
* `['key']` or `["key"]` : Access by key

---

## 3. Example JSON Document

We will use this sample JSON for traversal examples:

```json
{
  "store": {
    "book": [
      { "category": "reference", "author": "Nigel Rees", "title": "Sayings of the Century", "price": 8.95 },
      { "category": "fiction", "author": "Evelyn Waugh", "title": "Sword of Honour", "price": 12.99 },
      { "category": "fiction", "author": "Herman Melville", "title": "Moby Dick", "isbn": "0-553-21311-3", "price": 8.99 },
      { "category": "fiction", "author": "J. R. R. Tolkien", "title": "The Lord of the Rings", "isbn": "0-395-19395-8", "price": 22.99 }
    ],
    "bicycle": {
      "color": "red",
      "price": 19.95
    }
  }
}
```

---

## 4. Basic Traversal

### Root Element

* **JSONPath:** `$`
* **Result:** Entire JSON object

### Child Access

* **JSONPath:** `$.store`
* **Result:** `{ "book": [...], "bicycle": {...} }`

### Nested Child

* **JSONPath:** `$.store.bicycle.color`
* **Result:** `"red"`

---

## 5. Array Access

### Specific Index

* **JSONPath:** `$.store.book[0].title`
* **Result:** `"Sayings of the Century"`

### Last Element

* **JSONPath:** `$.store.book[-1].title`
* **Result:** `"The Lord of the Rings"`

### Multiple Indices

* **JSONPath:** `$.store.book[0,2].title`
* **Result:** `["Sayings of the Century", "Moby Dick"]`

### Array Slice

* **JSONPath:** `$.store.book[1:3].title`
* **Result:** `["Sword of Honour", "Moby Dick"]`

---

## 6. Wildcards

### All Properties

* **JSONPath:** `$.store.*`
* **Result:** `[book array, bicycle object]`

### All Elements in Array

* **JSONPath:** `$.store.book[*].author`
* **Result:** `["Nigel Rees", "Evelyn Waugh", "Herman Melville", "J. R. R. Tolkien"]`

---

## 7. Recursive Descent

### All Authors (at any depth)

* **JSONPath:** `$..author`
* **Result:** `["Nigel Rees", "Evelyn Waugh", "Herman Melville", "J. R. R. Tolkien"]`

### All Prices

* **JSONPath:** `$..price`
* **Result:** `[8.95, 12.99, 8.99, 22.99, 19.95]`

---

## 8. Filters

### Books Cheaper than 10

* **JSONPath:** `$.store.book[?(@.price < 10)].title`
* **Result:** `["Sayings of the Century", "Moby Dick"]`

### Books with ISBN

* **JSONPath:** `$.store.book[?(@.isbn)].title`
* **Result:** `["Moby Dick", "The Lord of the Rings"]`

### Fiction Books

* **JSONPath:** `$.store.book[?(@.category == 'fiction')].title`
* **Result:** `["Sword of Honour", "Moby Dick", "The Lord of the Rings"]`

---

## 9. Union of Keys

### Title or Price

* **JSONPath:** `$.store.book[*]['title','price']`
* **Result:**

```json
[
  { "title": "Sayings of the Century", "price": 8.95 },
  { "title": "Sword of Honour", "price": 12.99 },
  { "title": "Moby Dick", "price": 8.99 },
  { "title": "The Lord of the Rings", "price": 22.99 }
]
```

---

## 10. Advanced Filters

### Books with Price Between 9 and 20

* **JSONPath:** `$.store.book[?(@.price >= 9 && @.price <= 20)].title`
* **Result:** `["Sword of Honour"]`

### Authors Whose Name Contains "R"

* **JSONPath:** `$.store.book[?(@.author =~ /.*R.*/)].author`
* **Result:** `["J. R. R. Tolkien"]`

---

## 11. Practical Use Cases

* **Extract all product prices:** `$..price`
* **Find all keys in document:** `$..*`
* **Get all items regardless of nesting:** `$..book[*]`
* **Apply conditions:** `$.store.book[?(@.price > 15)]`

---

## 12. Key Takeaways

* JSONPath is powerful for traversing and extracting data from JSON.
* Combine operators (`.`, `..`, `*`, `[]`) for complex queries.
* Filters enable SQL-like querying on JSON.
* Recursive descent (`..`) is useful for deep extraction without knowing exact structure.

---

Would you like me to extend this doc with **visual diagrams of the JSON tree** showing how each JSONPath query traverses it? That would make the traversal paths even clearer.
