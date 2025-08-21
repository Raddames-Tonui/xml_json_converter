# XML + XPath + XSD Guide

## Example XML (20 lines)

```xml
<?xml version="1.0" encoding="UTF-8"?> <!-- 1: XML declaration: version + encoding -->

<!-- 2: A root element is required in XML -->
<library xmlns:bk="http://example.org/books" category="public"> <!-- 3: Root element with a namespace and an attribute -->

  <bk:book id="b1" available="true"> <!-- 4: Namespaced element with attributes -->
    <title lang="en">XML Fundamentals</title> <!-- 5: Element with attribute (language code) -->
    <author>John Doe</author> <!-- 6: Simple element -->
    <price currency="USD">29.99</price> <!-- 7: Element with numeric text + attribute -->
  </bk:book>

  <bk:book id="b2" available="false">
    <title lang="fr">Chemin XPath</title> <!-- 8: Another element, showing multilingual text -->
    <author>Jean Dupont</author>
    <price currency="EUR">35.50</price>
    <summary><![CDATA[XPath is powerful <and> can handle "weird" text]]></summary> <!-- 9: CDATA section (ignores special chars) -->
  </bk:book>

  <magazine> <!-- 10: Non-namespaced sibling element -->
    <title>XML Monthly</title>
    <issue number="42"/> <!-- 11: Empty element (self-closing) -->
  </magazine>

  <!-- 12: Comments can appear anywhere outside markup -->
  <note priority="high">
    <to>Alice</to>
    <from>Bob</from>
    <message>Don’t forget XML must be well-formed!</message>
  </note>

</library> <!-- 13: Root closes at the end -->
```

### 🔑 Key Takeaways from this XML

1. **XML Declaration**: `<?xml version="1.0" encoding="UTF-8"?>` is optional but recommended.
2. **Root Element**: XML requires a single root (`<library>` here).
3. **Namespaces**: `xmlns:bk="..."` lets you distinguish between vocabularies. Prefix `bk:` is applied to elements.
4. **Attributes vs Elements**: Both are allowed — use attributes for metadata (`id`, `currency`), elements for data (`<title>...`).
5. **Well-formedness**: Every tag must close properly (`</title>`), nesting must be correct.
6. **CDATA**: `<![CDATA[ ... ]]>` allows raw text with special characters `<`, `>`, `"`.
7. **Empty elements**: `<issue number="42"/>` is shorthand for `<issue number="42"></issue>`.
8. **Comments**: `<!-- ... -->` for human-readable notes, ignored by parsers.
9. **Types of content**: Text nodes, attributes, CDATA, nested elements.
10. **Case sensitivity**: `<Title>` ≠ `<title>`. XML is case-sensitive.
11. **Whitespace**: Preserved inside text nodes unless normalized by parser.
12. **Validation**: You can use **DTD** or **XSD schemas** to enforce structure (not shown here).
13. **Namespaces**: Crucial when mixing vocabularies (e.g., SVG inside XHTML).

---

⚡This sample is compact but covers **nearly all the essentials** you’ll encounter in practice.

---

## 📘 XML XPath Practice Sheet

### **1. Select all books**

```
/library/bk:book

```

✅ Gets both `<bk:book>` elements.

---

### **2. Select all book titles**

```
/library/bk:book/title

```

✅ Returns:

* `XML Fundamentals`
* `Chemin XPath`

---

### **3. Select book titles in English (**\`\`**)**

```
/library/bk:book/title[@lang='en']

```

✅ Returns:

* `XML Fundamentals`

---

### **4. Select the price of the French book**

```
/library/bk:book[title[@lang='fr']]/price

```

✅ Returns:

* `35.50`

---

### **5. Select books that are available**

```
/library/bk:book[@available='true']

```

✅ Matches `<bk:book id="b1" available="true"> ...`

---

### **6. Select all authors**

```
//author

```

✅ Returns:

* `John Doe`
* `Jean Dupont`

---

### **7. Select the magazine issue number (attribute)**

```
/library/magazine/issue/@number

```

✅ Returns:

* `42`

---

### **8. Select the CDATA summary text**

```
/library/bk:book/summary

```

✅ Returns:

* `XPath is powerful <and> can handle "weird" text`

---

### **9. Select the note message**

```
/library/note/message

```

✅ Returns:

* `Don’t forget XML must be well-formed!`

---

### 10. Select everything under 

```
/library/*

```

✅ Returns:

* All direct children (`bk:book`, `magazine`, `note`)

---

### ⚡ Key XPath Tricks Learned

* `/` → absolute path (from root).
* `//` → anywhere in the document (descendant search).
* `[@attr='value']` → filter by attribute.
* `@attr` → select an attribute.
* `*` → wildcard for any element.
* Predicates `[condition]` → filter elements (like SQL WHERE).

---

## XSD (Schema for the XML)

```xml
<?xml version="1.0" encoding="UTF-8"?>
<xs:schema 
    xmlns:xs="http://www.w3.org/2001/XMLSchema"
    targetNamespace="http://example.org/books"
    xmlns:bk="http://example.org/books"
    elementFormDefault="qualified"
    attributeFormDefault="unqualified">

  <!-- Root element -->
  <xs:element name="library">
    <xs:complexType>
      <xs:sequence>
        <xs:element ref="bk:book" maxOccurs="unbounded"/>
        <xs:element name="magazine" minOccurs="0">
          <xs:complexType>
            <xs:sequence>
              <xs:element name="title" type="xs:string"/>
              <xs:element name="issue" minOccurs="0">
                <xs:complexType>
                  <xs:attribute name="number" type="xs:positiveInteger" use="required"/>
                </xs:complexType>
              </xs:element>
            </xs:sequence>
          </xs:complexType>
        </xs:element>
        <xs:element name="note" minOccurs="0">
          <xs:complexType>
            <xs:sequence>
              <xs:element name="to" type="xs:string"/>
              <xs:element name="from" type="xs:string"/>
              <xs:element name="message" type="xs:string"/>
            </xs:sequence>
            <xs:attribute name="priority" use="required">
              <xs:simpleType>
                <xs:restriction base="xs:string">
                  <xs:enumeration value="low"/>
                  <xs:enumeration value="medium"/>
                  <xs:enumeration value="high"/>
                </xs:restriction>
              </xs:simpleType>
            </xs:attribute>
          </xs:complexType>
        </xs:element>
      </xs:sequence>
      <xs:attribute name="category" type="xs:string" use="optional"/>
    </xs:complexType>
  </xs:element>

  <!-- Book definition -->
  <xs:element name="book">
    <xs:complexType>
      <xs:sequence>
        <xs:element name="title">
          <xs:complexType mixed="false">
            <xs:simpleContent>
              <xs:extension base="xs:string">
                <xs:attribute name="lang" type="xs:language" use="required"/>
              </xs:extension>
            </xs:simpleContent>
          </xs:complexType>
        </xs:element>
        <xs:element name="author" type="xs:string"/>
        <xs:element name="price">
          <xs:complexType>
            <xs:simpleContent>
              <xs:extension base="xs:decimal">
                <xs:attribute name="currency" type="xs:string" use="required"/>
              </xs:extension>
            </xs:simpleContent>
          </xs:complexType>
        </xs:element>
        <xs:element name="summary" type="xs:string" minOccurs="0"/>
      </xs:sequence>
      <xs:attribute name="id" type="xs:ID" use="required"/>
      <xs:attribute name="available" use="required">
        <xs:simpleType>
          <xs:restriction base="xs:boolean"/>
        </xs:simpleType>
      </xs:attribute>
    </xs:complexType>
  </xs:element>

</xs:schema>
```

### 🔑 What This XSD Ensures

1. **Root**: `<library>` is the only root, must contain at least one `<bk:book>`.
2. **Books**:

    * Have `id` (unique), and `available` (`true`/`false`).
    * Must have `title`, `author`, `price`.
    * `title` must have `lang` attribute (ISO language code).
    * `price` must have a numeric value and a `currency` attribute.
    * `summary` is optional.
3. **Magazine**:

    * Has a `title`.
    * May have an `issue` with an integer `number`.
4. **Note**:

    * Contains `to`, `from`, `message`.
    * Must have `priority` attribute (`low`, `medium`, `high`).
5. **Namespace**: All `<bk:book>` elements belong to `http://example.org/books`.

---

⚡ With this, your XML will **validate** (well-formedness + schema compliance).

---

## Sample XML That Validates Against XSD

```xml
<?xml version="1.0" encoding="UTF-8"?>
<library xmlns:bk="http://example.org/books" category="public">

  <bk:book id="b1" available="true">
    <title lang="en">XML Fundamentals</title>
    <author>John Doe</author>
    <price currency="USD">29.99</price>
    <summary>Good starting point for XML learners</summary>
  </bk:book>

  <bk:book id="b2" available="false">
    <title lang="fr">Chemin XPath</title>
    <author>Jean Dupont</author>
    <price currency="EUR">35.50</price>
  </bk:book>

  <magazine>
    <title>XML Monthly</title>
    <issue number="42"/>
  </magazine>

  <note priority="high">
    <to>Alice</to>
    <from>Bob</from>
    <message>Don’t forget XML must be well-formed!</message>
  </note>

</library>
```
